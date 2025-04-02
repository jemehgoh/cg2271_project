#include "cg2271.h"

// Front LED control functions

/* 
  GPIO pins used for LEDs
	- PTB0
	- PTB1
	- PTB2
	- PTB3
	- PTC2
	- PTE29
	- PTC1
	- PTB30
	
	Refer to the FRMD-KL25Z datasheet for details on the pin positions on the board.
	In running mode, the LEDs light up in the order listed above.
*/

/* Sets up the green LEDs */
void setupGreenLED(void)
{
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;

	PORTB -> PCR[0] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[0] |= PORT_PCR_MUX(1);
	PORTB -> PCR[1] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[1] |= PORT_PCR_MUX(1);
	PORTB -> PCR[2] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[2] |= PORT_PCR_MUX(1);
	PORTB -> PCR[3] &= ~(PORT_PCR_MUX_MASK);
	PORTB -> PCR[3] |= PORT_PCR_MUX(1);
	PORTC -> PCR[2] &= ~(PORT_PCR_MUX_MASK);
	PORTC -> PCR[2] |= PORT_PCR_MUX(1);
	PORTE -> PCR[29] &= ~(PORT_PCR_MUX_MASK);
	PORTE -> PCR[29] |= PORT_PCR_MUX(1);
	PORTC -> PCR[1] &= ~(PORT_PCR_MUX_MASK);
	PORTC -> PCR[1] |= PORT_PCR_MUX(1);
	PORTE -> PCR[30] &= ~(PORT_PCR_MUX_MASK);
	PORTE -> PCR[30] |= PORT_PCR_MUX(1);

	PTB -> PDDR |= (1 << 0);
	PTB -> PDDR |= (1 << 1);
	PTB -> PDDR |= (1 << 2);
	PTB -> PDDR |= (1 << 3);
	PTC -> PDDR |= (1 << 2);
	PTE -> PDDR |= (1 << 29);
	PTC -> PDDR |= (1 << 1);
	PTE -> PDDR |= (1 << 30);
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
		 PTB -> PDOR &= ~(1 << 3);
	   PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;

	 case 1:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR |= (1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 PTB -> PDOR &= ~(1 << 3);
	 	 PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;

	 case 2:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR |= (1 << 2);
	   PTB -> PDOR &= ~(1 << 3);
	   PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;
	 
	 case 3:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
	   PTB -> PDOR |= (1 << 3);
	   PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;
	 
	 case 4:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 PTB -> PDOR &= ~(1 << 3);
	   PTC -> PDOR |= (1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;
	 
	 case 5:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 PTB -> PDOR &= ~(1 << 3);
		 PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR |= (1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;
		 
   case 6:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 PTB -> PDOR &= ~(1 << 3);
	 	 PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR |= (1 << 1);
		 PTE -> PDOR &= ~(1 << 30);
		 break;
			 
	 case 7:
		 PTB -> PDOR &= ~(1 << 0);
		 PTB -> PDOR &= ~(1 << 1);
		 PTB -> PDOR &= ~(1 << 2);
		 PTB -> PDOR &= ~(1 << 3);
	   PTC -> PDOR &= ~(1 << 2);
		 PTE -> PDOR &= ~(1 << 29);
		 PTC -> PDOR &= ~(1 << 1);
		 PTE -> PDOR |= (1 << 30);
		 break;
	 
	 default:
		 PTB -> PDOR |= (1 << 0);
		 PTB -> PDOR |= (1 << 1);
		 PTB -> PDOR |= (1 << 2);
	   PTB -> PDOR |= (1 << 3);
	 	 PTC -> PDOR |= (1 << 2);
		 PTE -> PDOR |= (1 << 29);
		 PTC -> PDOR |= (1 << 1);
		 PTE -> PDOR |= (1 << 30);
		 break;
	}
}

// Red LED control functions

/*
* Setup the red LEDs
*/
void setupRedLED(void)
{
	// Enable clock signal to PORTD and PORTE
	SIM -> SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Set up PTE1, PTE0, PTD6, PTD7 as GPIO
	PORTE -> PCR[0] |= PORT_PCR_MUX(1);
	PORTE -> PCR[1] |= PORT_PCR_MUX(1);	
	PORTD -> PCR[6] |= PORT_PCR_MUX(1);
	PORTD -> PCR[7] |= PORT_PCR_MUX(1);
	PORTB -> PCR[8] |= PORT_PCR_MUX(1);
	PORTB -> PCR[9] |= PORT_PCR_MUX(1);	
	PORTB -> PCR[10] |= PORT_PCR_MUX(1);
	PORTB -> PCR[11] |= PORT_PCR_MUX(1);
	
	// Configure direction of pins
	PTE -> PDDR |= (1 << 0);
	PTE -> PDDR |= (1 << 1);
	PTD -> PDDR |= (1 << 6);
	PTD -> PDDR |= (1 << 7);
	PTB -> PDDR |= (1 << 8);
	PTB -> PDDR |= (1 << 9);
	PTB -> PDDR |= (1 << 10);
	PTB -> PDDR |= (1 << 11);	
}

/*
* Toggles the red LEDs (switches them ON when they are OFF, and vice versa)
*/
void toggleRedLED(void)
{
	PTE -> PTOR |= (1 << 0);
	PTE -> PTOR |= (1 << 1);
	PTD -> PTOR |= (1 << 6);
	PTD -> PTOR |= (1 << 7);
	PTB -> PTOR |= (1 << 8);
	PTB -> PTOR |= (1 << 9);
	PTB -> PTOR |= (1 << 10);
	PTB -> PTOR |= (1 << 11);	
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
//		case 0:
//			// Stationary
//			TPM0_C0V = 0;
//			TPM0_C1V = 0;
//			TPM0_C2V = 0;
//			TPM0_C3V = 0;
//			break;

		case 0:
			// Forward motion
			TPM0_C0V = 600;
			TPM0_C1V = 0;
			TPM0_C2V = 600;
			TPM0_C3V = 0;
			break;

		case 1:
			// Backward motion
			TPM0_C0V = 0;
			TPM0_C1V = 600;
			TPM0_C2V = 0;
			TPM0_C3V = 600;
			break;

		case 2:
			// Left turn
			TPM0_C0V = 0;
			TPM0_C1V = 500;
			TPM0_C2V = 500;
			TPM0_C3V = 0;
			break;

		case 3:
			// Right turn
			TPM0_C0V = 500;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 500;
			break;

		case 4:
			// Front left curved turn
			TPM0_C0V = 70;
			TPM0_C1V = 0;
			TPM0_C2V = 700;
			TPM0_C3V = 0;
			break;

		case 5:
			// Front right curved turn
			TPM0_C0V = 700;
			TPM0_C1V = 0;
			TPM0_C2V = 70;
			TPM0_C3V = 0;
			break;

		case 6:
			// Back left curved turn
			TPM0_C0V = 0;
			TPM0_C1V = 70;
			TPM0_C2V = 0;
			TPM0_C3V = 700;
			break;
		
		case 7:
			// Back right curved turn
			TPM0_C0V = 0;
			TPM0_C1V = 700;
			TPM0_C2V = 0;
			TPM0_C3V = 70;
			break;

		default:
			TPM0_C0V = 0;
			TPM0_C1V = 0;
			TPM0_C2V = 0;
			TPM0_C3V = 0;
			break;
	}
}

/*
* Enables UART2 to receive messages at the specified baudrate at PTE23.
*
* @param baudrate the baudrate of the UART messages to be received.
*/
void init_UART2(uint32_t baudrate)
{
	// Enable clock signal to UART2 and PORTC (UART TX/RX pins)
	SIM -> SCGC4  |= SIM_SCGC4_UART2_MASK;
	SIM -> SCGC5 |= SIM_SCGC5_PORTE_MASK;
	
	// Disable UART2
	UART2 -> C2 &= ~(UART_C2_TE_MASK);
	UART2 -> C2 &= ~(UART_C2_RE_MASK);
	
	// Clear all previous settings
	UART2 -> S2 = 0;
	UART2 -> C1 = 0;
	UART2 -> C3 = 0;
	
	// Set up pins PTE23 for UART RX
	PORTE -> PCR[23] |= PORT_PCR_MUX(4);
	
	// Set baudrate
	uint32_t bus_clock = DEFAULT_SYSTEM_CLOCK / 2;
	uint32_t divisor = bus_clock / (baudrate * 16);
	UART2 -> BDH = UART_BDH_SBR(divisor >> 8);
	UART2 -> BDL = UART_BDL_SBR(divisor);

	// Enable RX interrupts
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	UART2 -> C2 |= UART_C2_RIE_MASK;
	
	// Enable RX on UART1
	UART2 -> C2 |= UART_C2_RE_MASK;
}

// Buzzer functions

// Sets up buzzer without playing any tone
void setupBuzzer(void)
{
	// Set up PTA12 as TPM1 output
	SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK;
	
	PORTA -> PCR[12] &= ~PORT_PCR_MUX_MASK;
	PORTA -> PCR[12] |= PORT_PCR_MUX(3);
	
	SIM -> SCGC6 |= SIM_SCGC6_TPM1_MASK;
	
	SIM -> SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;
	SIM -> SOPT2 |= SIM_SOPT2_TPMSRC(1);
	
	TPM1 -> MOD = 1000;
	
	// Clear prescaler and clock setting for TPM1
	TPM1 -> SC &= ~(TPM_SC_CMOD_MASK);
	TPM1 -> SC &= ~(TPM_SC_PS_MASK);

	// Set clock source to TPM1 counter clock and prescaler to 128
	TPM1 -> SC |= TPM_SC_PS(7);
	
	TPM1 -> SC &= ~(TPM_SC_CPWMS_MASK);
	
	TPM1 -> SC |= TPM_SC_CMOD(1);
	
	// Set up TPM1 Ch0 to output high-true edge-aligned PWM
	TPM1_C0V = 0;
	TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) | (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));
	TPM1_C0SC |= (TPM_CnSC_MSB(1)|TPM_CnSC_ELSB(0b10 >> 1)); 
}

// Plays a tone on the buzzer with the specified mod value
void playBuzzer(uint32_t mod)
{
	TPM1 -> MOD = mod;
	TPM1_C0V = (mod / 2);
}


// Disables the buzzer
void disableBuzzer(void)
{
	TPM1_C0V = 0;
}
