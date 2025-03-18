/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include "cg2271.h"
#include "cmsis_os2.h"

#define QUEUE_SIZE 16 // Macro for queue size - will use 16 bytes (for now)
#define BAUD_RATE 9600 // Baudrate for UART (for connection to ESP32)

// Green LED flag setting
#define GREEN_LED_FLAG 0x00000001

// Red LED flag settings
#define RED_LED_STOP_OFF 0x00000000
#define RED_LED_STOP_ON 0x00000001
#define RED_LED_MOVE_OFF 0x00000002
#define RED_LED_MOVE_ON 0x00000003

// Red LED movement flag
#define RED_LED_MOVE_FLAGS 0x00000002

#define FLAG_ERROR_MASK 0x80000000

static volatile uint8_t msgReceived = 0;
static volatile uint8_t command = 0x00;

static volatile uint32_t runningLED = 0;
static volatile uint8_t ledToRun = 0;

static volatile uint8_t direction = 0x00;

// Message packet
typedef struct
{
	uint8_t data;
	uint8_t buffer[3];
} msgPkt;

// Event flag id
// osEventFlagsId_t redLEDFlags;
// osEventFlagsId_t greenLEDFlags;

// Message queue ids
// osMessageQueueId_t command_MsgQueue; // Queue for commands from UART
// osMessageQueueId_t motorDirection_MsgQueue; // Queue for the motor direction

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
//		msgPkt command;
//		command.data = UART2 -> D;
//		osMessageQueuePut(command_MsgQueue, &command, 0U, 0U);
		command = UART2 -> D;
		msgReceived = 1;
	}
}

/*----------------------------------------------------------------------------
 * Thread for decoding command (brain thread)
 *---------------------------------------------------------------------------*/
__NO_RETURN static void brain_thread(void *argument) {
  (void)argument;
  for (;;) {
		// Get command from command queue
//		msgPkt command;
//		msgPkt motorDirection;
//		osStatus_t command_status = osMessageQueueGet(command_MsgQueue, &command, NULL, osWaitForever);
//		
//		if (command_status == osOK)
//		{
//			if (command.data & 0x10)
//			{
//				
//				motorDirection.data = command.data & 0x0F; // set motorDirection to the last 4 bits of the command
//				osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
//				osEventFlagsSet(greenLEDFlags, GREEN_LED_FLAG);
//				osEventFlagsSet(redLEDFlags, RED_LED_MOVE_FLAGS);
//			}
//			else
//			{
//				motorDirection.data = 0x00; // set motorDirection to 0 (stop motors)
//				osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
//				osEventFlagsClear(greenLEDFlags, GREEN_LED_FLAG);				
//				osEventFlagsClear(redLEDFlags, RED_LED_MOVE_FLAGS);				
//			}
//		}
//		else
//		{
//			motorDirection.data = 0x00; // set motorDirection to 0 (stop motors)
//			osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
//			osEventFlagsClear(greenLEDFlags,GREEN_LED_FLAG);				
//			osEventFlagsClear(redLEDFlags, RED_LED_MOVE_FLAGS);					
//		if (msgReceived == 1)
		{
			if (command & 0x10)
			{
				direction = command & 0x0F;
				ledToRun = 1;
			}
			else
			{
				direction = 0x00;
				ledToRun = 0;
			}
			
			msgReceived = 0;
		}
	}
}

/*----------------------------------------------------------------------------
 * Thread for blinking green LED
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_green_thread(void *argument) {
  (void)argument;
  for (;;) {
//		uint32_t flagStatus = osEventFlagsWait(greenLEDFlags, GREEN_LED_FLAG, osFlagsNoClear, osWaitForever);
//		if (flagStatus & FLAG_ERROR_MASK) 
//		{
//			// Flash all LEDS if flag is not set / an error has occurred
//			flashGreenLED(9);
//		}
//		else
//		{
//			runningLED = (runningLED == 2) ? 0 : (runningLED + 1);
//			flashGreenLED(runningLED);
//		}
		if (ledToRun)
		{
			runningLED = (runningLED == 2) ? 0 : (runningLED + 1);
			flashGreenLED(runningLED);
		}
		else
		{
			flashGreenLED(9);
		}
		osDelay(500);
	}
}
 
/*----------------------------------------------------------------------------
 * Thread for motor control
 *---------------------------------------------------------------------------*/
__NO_RETURN static void motor_thread(void *argument) {
  (void)argument;
  for (;;) {
//		msgPkt motorDirection;
//		osStatus_t messageStatus = osMessageQueueGet(motorDirection_MsgQueue, &motorDirection, NULL, osWaitForever);
//		if (messageStatus == osOK)
//		{
//			runMotor(motorDirection.data);
//		}
//		else
//		{
//			// Motors stationary if no specified direction received
//			runMotor(0x01);
//		}
		runMotor(direction);
		osDelay(2000);
	}
}

int main(void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  setupGreenLED();
	initMotors();
	
	// Initialize event flags
//	redLEDFlags = osEventFlagsNew(NULL);
//	greenLEDFlags = osEventFlagsNew(NULL);
	
	// Initalize message queues
//	command_MsgQueue = osMessageQueueNew(16, 4, NULL); 
//	motorDirection_MsgQueue = osMessageQueueNew(16, 4, NULL);
	
	init_UART2(BAUD_RATE);
	
  osKernelInitialize();   	// Initialize CMSIS-RTOS
	NVIC_SetPriority(UART2_IRQn, 2);
	NVIC_SetPriorityGrouping(3);
	osThreadNew(motor_thread, NULL, NULL); // Create thread for motor
	osThreadNew(led_green_thread, NULL, NULL); // Create thread for green LED    
	osThreadNew(brain_thread, NULL, NULL); // Create thread for decoding commands
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
