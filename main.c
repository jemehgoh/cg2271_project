/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include "cg2271.h"
#include "cmsis_os2.h"

static volatile uint32_t runningLED = 0;
static volatile uint32_t redBlinkInterval = 250;
static volatile uint8_t motorDirection = 0;

const osThreadAttr_t thread1_attr = {
	.priority = osPriorityHigh
};

const osThreadAttr_t thread2_attr = {
	.priority = osPriorityNormal
};

/*----------------------------------------------------------------------------
 * Thread for blinking green LED
 *---------------------------------------------------------------------------*/
__NO_RETURN static void led_green_thread(void *argument) {
  (void)argument;
  for (;;) {
		runningLED = (runningLED == 2) ? 0 : (runningLED + 1);
		flashGreenLED(runningLED);
		osDelay(500);
	}
}
 
/*----------------------------------------------------------------------------
 * Thread for motor control
 *---------------------------------------------------------------------------*/
__NO_RETURN static void motor_thread(void *argument) {
  (void)argument;
  for (;;) {
		motorDirection = (motorDirection == 8) ? 0 : (motorDirection + 1);
		runMotor(motorDirection);
		osDelay(2000);
	}
}

int main (void) {
 
  // System Initialization
  SystemCoreClockUpdate();
  setupGreenLED();
	initMotors();
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
	osThreadNew(led_green_thread, NULL, &thread1_attr); // Create thread for green LED
  osThreadNew(motor_thread, NULL, &thread2_attr);     // Create thread for motor
  osKernelStart();                      // Start thread execution
  for (;;) {}
}
