/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include "cg2271.h"
#include "cmsis_os2.h"

#define QUEUE_SIZE 16 // Macro for queue size - will use 16 bytes (for now)
#define BAUD_RATE 9600 // Baudrate for UART (for connection to ESP32)

static volatile uint32_t runningLED = 0;

// Event flag id
osEventFlagsId_t redLEDFlags;
osEventFlagsId_t greenLEDFlags;

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
void UART1_IRQHandler(void)
{
	NVIC_ClearPendingIRQ(UART1_IRQn);
	if (UART1_S1 & UART_S1_RDRF_MASK)
	{
		uint8_t command = UART1 -> D;
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
		uint8_t command;
		osStatus_t command_status = osMessageQueueGet(command_MsgQueue, &command, NULL, 0U);
		
		if (command_status == osOK)
		{
			if (command & 0x10)
			{
				uint8_t motorDirection = command & 0x0F; // set motorDirection to the last 4 bits of the command
				osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
				osEventFlagsSet(greenLEDFlags, 0x00000001);
				osEventFlagsSet(redLEDFlags, 0x00000002);
			}
			else
			{
				uint8_t motorDirection = 0x00; // set motorDirection to 0 (stop motors)
				osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
				osEventFlagsClear(greenLEDFlags, 0x00000001);				
				osEventFlagsClear(redLEDFlags, 0x00000002);				
			}
		}
		else
		{
			uint8_t motorDirection = 0x00; // set motorDirection to 0 (stop motors)
			osMessageQueuePut(motorDirection_MsgQueue, &motorDirection, 0U, 0U);
			osEventFlagsClear(greenLEDFlags, 0x00000001);				
			osEventFlagsClear(redLEDFlags, 0x00000002);					
		}
	}
}

/*----------------------------------------------------------------------------
 * Thread for blinking green LED
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_green_thread(void *argument) {
  (void)argument;
  for (;;) {
		uint32_t flagStatus = osEventFlagsWait(greenLEDFlags, 0x00000001, osFlagsNoClear, 0U);
		if (flagStatus & 0x80000000) 
		{
			// Flash all LEDS if flag is not set / an error has occurred
			flashGreenLED(9);
		}
		else
		{
			runningLED = (runningLED == 2) ? 0 : (runningLED + 1);
			flashGreenLED(runningLED);
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
		uint8_t motorDirection;
		osStatus_t messageStatus = osMessageQueueGet(motorDirection_MsgQueue, &motorDirection, NULL, 0U);
		if (messageStatus == osOK)
		{
			runMotor(motorDirection);
		}
		else
		{
			// Motors stationary if no specified direction received
			runMotor(0x00);
		}
		osDelay(2000);
	}
}

int main(void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  setupGreenLED();
	initMotors();
	init_UART1(BAUD_RATE);
	
	// Initialize event flags
	redLEDFlags = osEventFlagsNew(NULL);
	greenLEDFlags = osEventFlagsNew(NULL);
	
	// Initalize message queues
	command_MsgQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(uint8_t), NULL); 
	motorDirection_MsgQueue = osMessageQueueNew(QUEUE_SIZE, sizeof(uint8_t), NULL);
	
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(brain_thread, NULL, &thread2_attr); // Create thread for decoding commands
	osThreadNew(led_green_thread, NULL, &thread1_attr); // Create thread for green LED
  osThreadNew(motor_thread, NULL, &thread2_attr);     // Create thread for motor
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
