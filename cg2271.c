#include "cg2271.h"

// Front LED control functions

/* Sets up the green LEDs */
void setupGreenLED(void)
{
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;

	PORTB -> PCR[0] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[0] |= PORT_PCR_MUX(1);
	PORTB -> PCR[1] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[1] |= PORT_PCR_MUX(1);
	PORTB -> PCR[2] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[2] |= PORT_PCR_MUX(1);
	PORTB -> PCR[3] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[3] |= PORT_PCR_MUX(1);

	PTB -> PDDR |= (1 << 0);
	PTB -> PDDR |= (1 << 1);
	PTB -> PDDR |= (1 << 2);
	PTB -> PDDR |= (1 << 3);
}

/* Flashes the green LEDs in a running order */
void flashGreenLED(uint32_t index)
{
	switch (index)
	{
	 case 0:
		 PTB -> PDOR |= (1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 break;

	 case 1:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR |= (1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 break;

	 case 2:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR |= (1 << 2);
		 break;

	 default:
		 PTB -> PDOR |= (1 << 0);
		 PTB -> PDOR |= (1 << 1);
		 PTB -> PDOR |= (1 << 2);
		 break;
	}
}

// Motor control functions

/*
* Initialises motor control pins for motor control.
*/
void initMotors(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	
	PORTD -> PCR[0] &= ~PORT_PCR_MUX_MASK;
	PORTD -> PCR[0] |= PORT_PCR_MUX(4);
	
	PORTD -> PCR[1] &= ~PORT_PCR_MUX_MASK;
	PORTD -> PCR[1] |= PORT_PCR_MUX(4);	
	
	PORTD -> PCR[2] &= ~PORT_PCR_MUX_MASK;
	PORTD -> PCR[2] |= PORT_PCR_MUX(4);
	
	PORTD -> PCR[3] &= ~PORT_PCR_MUX_MASK;
	PORTD -> PCR[3] |= PORT_PCR_MUX(4);	
	
	SIM -> SCGC6 |= SIM_SCGC6_TPM0_MASK;
	
	SIM -> SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM -> SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM0 -> MOD = 1000;
	
	// Clear prescaler and clock setting for TPM1
	TPM0 -> SC &= ~(TPM_SC_CMOD_MASK);
	TPM0 -> SC &= ~(TPM_SC_PS_MASK);

	// Set clock source to TPM1 counter clock and prescaler to 128
	TPM0 -> SC |= TPM_SC_PS(7);
	
	TPM0 -> SC |= (TPM_SC_CPWMS_MASK);
	
	TPM0 -> SC |= TPM_SC_CMOD(1);
	
	// Initialise TPM0 channels 0-3 for high-true centre-aligned PWM
	TPM0_C0V = 0;
	TPM0_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C0SC |= (TPM_CnSC_MSB(1)|TPM_CnSC_ELSB(0b10 >> 1)); 
	
	TPM0_C1V = 0;
	TPM0_C1SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C1SC |= (TPM_CnSC_MSB(1)|TPM_CnSC_ELSB(0b10 >> 1)); 
	
	TPM0_C2V = 0;
	TPM0_C2SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C2SC |= (TPM_CnSC_MSB(1)|TPM_CnSC_ELSB(0b10 >> 1)); 
	
	TPM0_C3V = 0;
	TPM0_C3SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM0_C3SC |= (TPM_CnSC_MSB(1)|TPM_CnSC_ELSB(0b10 >> 1)); 
}

/*
* Runs the motors in a way to get the robot to move a specified direction.
* 
* @param direction the direction the robot is to move in.
*/
void runMotor(uint8_t direction)
{
	switch (direction)
	{
		case 0:
			// Stationary
			TPM0_C0V = 0;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 0;
			break;

		case 1:
			// Forward motion
			TPM0_C0V = 750;
			TPM0_C1V = 0;
			TPM0_C2V = 750;
			TPM0_C3V = 0;
			break;

		case 2:
			// Backward motion
			TPM0_C0V = 0;
			TPM0_C1V = 750;
			TPM0_C2V = 0;
			TPM0_C3V = 750;
			break;

		case 3:
			// Left turn
			TPM0_C0V = 0;
			TPM0_C1V = 750;
			TPM0_C2V = 750;
			TPM0_C3V = 0;
			break;

		case 4:
			// Right turn
			TPM0_C0V = 750;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 750;
			break;

		case 5:
			// Front left curved turn
			TPM0_C0V = 400;
			TPM0_C1V = 0;
			TPM0_C2V = 800;
			TPM0_C3V = 0;
			break;

		case 6:
			// Front right curved turn
			TPM0_C0V = 800;
			TPM0_C1V = 0;
			TPM0_C2V = 400;
			TPM0_C3V = 0;
			break;

		case 7:
			// Back left curved turn
			TPM0_C0V = 0;
			TPM0_C1V = 400;
			TPM0_C2V = 0;
			TPM0_C3V = 800;
			break;
		
		case 8:
			// Back right curved turn
			TPM0_C0V = 0;
			TPM0_C1V = 800;
			TPM0_C2V = 0;
			TPM0_C3V = 400;
			break;

		default:
			TPM0_C0V = 0;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 0;
			break;
	}
}
