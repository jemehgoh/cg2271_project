#ifndef __CG2271_H
#define __CG2271_H

#include "MKL25Z4.h"

/*---------------------------------------------------------------------------
* Header file with functions used in the program
*---------------------------------------------------------------------------*/

// Green LED control functions
void setupGreenLED(void);
void flashGreenLED(uint32_t index);

// Motor control functions
void initMotors(void);
void runMotor(uint8_t direction);

#endif
