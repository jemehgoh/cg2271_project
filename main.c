/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include "cg2271.h"
#include "cmsis_os2.h"

#define QUEUE_SIZE 16 // Macro for queue size - will use 16 bytes (for now)
#define BAUD_RATE 9600 // Baudrate for UART (for connection to ESP32)

// Green LED flag setting
#define FLAG_SET 0x1

// Red LED flag settings
#define RED_LED_STOP_OFF 0x00000000
#define RED_LED_STOP_ON 0x00000001
#define RED_LED_MOVE_OFF 0x00000002
#define RED_LED_MOVE_ON 0x00000003

// Red LED movement flag
#define RED_LED_MOVE_FLAGS 0x00000002

#define FLAG_ERROR_MASK 0x80000000

static volatile uint32_t runningLED = 0;

// Message packet
typedef struct
{
	uint8_t data;
	uint8_t buffer[3];
} msgPkt;

// Event flag id
 osEventFlagsId_t redLEDRunFlag;
 osEventFlagsId_t redLEDStopFlag;

 osEventFlagsId_t greenLEDRunFlag;
 osEventFlagsId_t greenLEDStopFlag;

// Message queue ids
 osMessageQueueId_t command_MsgQueue; // Queue for commands from UART
 osMessageQueueId_t motorDirection_MsgQueue; // Queue for the motor direction

// Thread parameters
const osThreadAttr_t thread1_attr = {
	.priority = osPriorityHigh
};

const osThreadAttr_t thread2_attr = {
	.priority = osPriorityNormal
};

// UART ISR
void UART2_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(UART2_IRQn);
	if (UART2_S1 & UART_S1_RDRF_MASK)
	{
		msgPkt command;
		command.data = UART2 -> D;
		osMessageQueuePut(command_MsgQueue, &command, 0U, 0U);
	}
}

/*----------------------------------------------------------------------------
 * Thread for decoding command (brain thread)
 *---------------------------------------------------------------------------*/
__NO_RETURN static void brain_thread(void *argument) {
  (void)argument;
  for (;;) {
		// Get command from command queue
		msgPkt command;
		msgPkt motorDirection;
		osStatus_t command_status = osMessageQueueGet(command_MsgQueue, &command, NULL, 0U);
		
		if (command_status == osOK)
		{
			if (command.data & 0x10)
			{
				
				motorDirection.data = command.data & 0x0F; // set motorDirection to the last 4 bits of the command
				osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 10U);
				osEventFlagsSet(greenLEDRunFlag, FLAG_SET);
				osEventFlagsClear(greenLEDStopFlag, FLAG_SET);
				osEventFlagsSet(redLEDRunFlag, FLAG_SET);
				osEventFlagsClear(redLEDStopFlag, FLAG_SET);
			}
			else
			{
				motorDirection.data = 0x08; // set motorDirection to 0 (stop motors)
				osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
				osEventFlagsSet(greenLEDStopFlag, FLAG_SET);
				osEventFlagsClear(greenLEDRunFlag, FLAG_SET);			
				osEventFlagsSet(redLEDStopFlag, FLAG_SET);
				osEventFlagsClear(redLEDRunFlag, FLAG_SET);
			}
		}
		else
		{
			motorDirection.data = 0x08; // set motorDirection to 0 (stop motors)
			osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
			osEventFlagsSet(greenLEDStopFlag, FLAG_SET);
			osEventFlagsClear(greenLEDRunFlag, FLAG_SET);						
			osEventFlagsSet(redLEDStopFlag, FLAG_SET);
			osEventFlagsClear(redLEDRunFlag, FLAG_SET);
		}			
	}
}

/*----------------------------------------------------------------------------
 * Thread for running green LED
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_green_run_thread(void *argument) {
  (void)argument;
  for (;;) {
		osEventFlagsWait(greenLEDRunFlag, FLAG_SET, osFlagsNoClear, osWaitForever);
		runningLED = (runningLED == 7) ? 0 : (runningLED + 1);
		flashGreenLED(runningLED);
		osDelay(500);
	}
}

/*----------------------------------------------------------------------------
 * Thread for flashing all green LED
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_green_stop_thread(void *argument) {
  (void)argument;
  for (;;) {
		osEventFlagsWait(greenLEDStopFlag, FLAG_SET, osFlagsNoClear, osWaitForever);
		flashGreenLED(9);
		osDelay(500);
	}
}

/*----------------------------------------------------------------------------
 * Thread for toggling red LEDs when moving.
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_red_run_thread(void *argument) {
  (void)argument;
  for (;;) {
		osEventFlagsWait(redLEDRunFlag, FLAG_SET, osFlagsNoClear, osWaitForever);
		toggleRedLED();
		osDelay(500);
	}
}

/*----------------------------------------------------------------------------
 * Thread for toggling red LEDs when stationary.
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_red_stop_thread(void *argument) {
  (void)argument;
  for (;;) {
		osEventFlagsWait(redLEDStopFlag, FLAG_SET, osFlagsNoClear, osWaitForever);
		toggleRedLED();
		osDelay(250);
	}
}
 
/*----------------------------------------------------------------------------
 * Thread for motor control
 *---------------------------------------------------------------------------*/
__NO_RETURN static void motor_thread(void *argument) {
  (void)argument;
  for (;;) {
		msgPkt motorDirection;
		osStatus_t messageStatus = osMessageQueueGet(motorDirection_MsgQueue, &motorDirection, NULL, osWaitForever);
		if (messageStatus == osOK)
		{
			runMotor(motorDirection.data);
		}
		else
		{
			// Motors stationary if no specified direction received
			runMotor(0x08);
		}
		
		osDelay(500);
	}
}

int main(void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  setupGreenLED();
	setupRedLED();
	initMotors();
		
	init_UART2(BAUD_RATE);
	
  osKernelInitialize();   	// Initialize CMSIS-RTOS
	
	// Initialize event flags
	redLEDRunFlag = osEventFlagsNew(NULL);
	redLEDStopFlag = osEventFlagsNew(NULL);
	greenLEDRunFlag = osEventFlagsNew(NULL);
	greenLEDStopFlag = osEventFlagsNew(NULL);
	
	// Initalize message queues
	command_MsgQueue = osMessageQueueNew(16, 4, NULL); 
	motorDirection_MsgQueue = osMessageQueueNew(16, 4, NULL);
	
	osThreadNew(motor_thread, NULL, NULL); // Create thread for motor
	osThreadNew(led_green_run_thread, NULL, NULL); // Create thread for running green LED  
	osThreadNew(led_green_stop_thread, NULL, NULL);  // Create thread for stopping green LED
	osThreadNew(led_red_run_thread, NULL, NULL); // Create thread for running red LED  
	osThreadNew(led_red_stop_thread, NULL, NULL);  // Create thread for stopping red LED	
	osThreadNew(brain_thread, NULL, NULL); // Create thread for decoding commands
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
