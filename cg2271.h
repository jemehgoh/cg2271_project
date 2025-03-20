#ifndef __CG2271_H
#define __CG2271_H

#include "MKL25Z4.h"

/*---------------------------------------------------------------------------
* Header file with functions used in the program
*---------------------------------------------------------------------------*/

// Green LED control functions
void setupGreenLED(void);
void flashGreenLED(uint32_t index);

// Red LED control functions
void setupRedLED(void);
void toggleRedLED(void);

// Motor control functions
void initMotors(void);
void runMotor(uint8_t direction);

// UART functions
void init_UART2(uint32_t baudrate);

// Buzzer functions
void setupBuzzer(void);
void playBuzzer(uint32_t mod);
void disableBuzzer(void);

#endif
