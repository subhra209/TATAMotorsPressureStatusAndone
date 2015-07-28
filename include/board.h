#ifndef DEVICE_H
#define DEVICE_H

/*
*------------------------------------------------------------------------------
* device.h
*
* Include file for port pin assignments
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.h
*------------------------------------------------------------------------------
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/
#ifdef __18F4520
#include <p18f4520.h>
#elif __18F4620
#include <p18f4620.h>
#elif __18F8722
#endif
#include <p18f8722.h>
#include <delays.h>
#include <timers.h>
#include "typedefs.h"
#include "config.h"


/*
*------------------------------------------------------------------------------
* Hardware Port Allocation
*------------------------------------------------------------------------------
*/
// Rs485 / RS232 Serial commnunicaton port
#define		TX1_EN					PORTGbits.RG3			// TX control for RS485 communication
#define		TX1_EN_DIR				TRISGbits.TRISG3

#define 	SER_TX1					PORTCbits.RC6   		// serial transmit
#define		SER_TX1_DIR				TRISCbits.TRISC6
#define 	SER_RX1					PORTCbits.RC7			// serial receive
#define		SER_RX1_DIR				TRISCbits.TRISC7

#define		TX2_EN					PORTGbits.RG4			// TX control for RS485 communication
#define		TX2_EN_DIR				TRISGbits.TRISG4

#define 	SER_TX2					PORTGbits.RG1   		// serial transmit
#define		SER_TX2_DIR				TRISGbits.TRISG1
#define 	SER_RX2					PORTGbits.RG2			// serial receive
#define		SER_RX2_DIR				TRISGbits.TRISG2

// Display Data port
#define		DISPLAY_PORT_LINE_1_2				PORTA	//LATD	 				// 7seg display data (DB0-DB7)
#define 	DISPLAY_PORT_LINE_1_2_DIRECTION		TRISA
#define		DISPLAY_PORT_LINE_3_4               PORTD
#define 	DISPLAY_PORT_LINE_3_4_DIRECTION		TRISD
#define		DISPLAY_PORT_LINE_5                 PORTF
#define 	DISPLAY_PORT_LINE_5_DIRECTION		TRISF


// Display digit select lines
#define		DIGIT_SEL_LINE_1_2		        	PORTB                   // digit sel LINE 1 & LINE 2
#define		DIGIT_SEL_LINE_1_2_DIRECTION        TRISB					
#define		DIGIT_SEL_LINE_3_4					PORTE					// digit sel LINE 3 & LINE 4
#define		DIGIT_SEL_LINE_3_4_DIRECTION        TRISE
#define		DIGIT_SEL_LINE_5					PORTH					// digit sel LINE 5
#define		DIGIT_SEL_LINE_5_DIRECTION			TRISH

//Satus Indicator And Buzzer
#define		LAMP_1						PORTJbits.RJ0					//LINE_1 Satus Indicator
#define		LAMP_1_DIRECTION			TRISJbits.TRISJ0
#define		LAMP_2						PORTJbits.RJ1					//LINE_2 Satus Indicator
#define		LAMP_2_DIRECTION			TRISJbits.TRISJ1
#define		LAMP_3						PORTJbits.RJ2					//LINE_3 Satus Indicator
#define		LAMP_3_DIRECTION			TRISJbits.TRISJ2
#define		LAMP_4						PORTJbits.RJ3					//LINE_4 Satus Indicator
#define		LAMP_4_DIRECTION			TRISJbits.TRISJ3
#define		LAMP_5						PORTJbits.RJ4					//LINE_5 Satus Indicator
#define		LAMP_5_DIRECTION			TRISJbits.TRISJ4
#define		BUZZER						PORTJbits.RJ5
#define		BUZZER_DIRECTION			TRISJbits.TRISJ5

//Heart Beatl
#define 	HEART_BEAT					PORTGbits.RG0
#define 	HEART_BEAT_DIRECTION		TRISGbits.TRISG0
	
/*
*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/

#define DEVICE_ID			(0x01)

#define SYSTEM_CLOCK			(8000000UL)	// Hz		// 20MHz external crystal  used

//#define SYSTEM_CLOCK			(40000000UL)	// Hz		// 10MHz external crystal x4 PLL

#define PERIPHERA_CLOCK			(SYSTEM_CLOCK / 4)UL

#define PERIPHERA_CLOCK_PERIOD 	(1 / PERIPHERA_CLOCK)UL



// Direction controle bit is processor specific ,
#define PORT_OUT				(BOOL)(0)
#define PORT_IN					(0XFF)

#define OFF_FOREVER				(BOOL)(0)
#define LOOP_FOREVER			(BOOL)(1)

#define SWITCH_OFF				(BOOL)(0)
#define SWITCH_ON				(BOOL)(1)


#define DISPLAY_DISABLE			(BOOL)(1)
#define DISPLAY_ENABLE			(BOOL)(0)
void EnableInterrupts(void);

/*
#ifdef __74LS374__
#undef DISPLAY_DISABLE
#undef DISPLAY_ENABLE

#define DISPLAY_DISABLE			(BOOL)(0)
#define DISPLAY_ENABLE			(BOOL)(1)

#endif 			//__74LS374__
*/


#define GetSystemClock()		(SYSTEM_CLOCK)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define GetPeripheralClock()	GetInstructionClock()


#define BUZZER_ON()			BUZ_OP = SWITCH_ON;
#define BUZZER_OFF()		BUZ_OP = SWITCH_OFF;

// Disable global interrupt bit.
#define ENTER_CRITICAL_SECTION()	INTCONbits.GIE = 0;

// Enable global interrupt bit.
#define EXIT_CRITICAL_SECTION()		INTCONbits.GIE = 1;

#define ENABLE_GLOBAL_INT()			EXIT_CRITICAL_SECTION()


#define DISABLE_ADC_INTERRUPT()		PIE1bits.ADIE = 0
#define ENABLE_ADC_INTERRUPT()		PIE1bits.ADIE = 1

#define DISABLE_INT0_INTERRUPT()	INTCONbits.INT0IE = 0
#define ENABLE_INT0_INTERRUPT()		INTCONbits.INT0IE = 1
#define CLEAR_INTO_INTERRUPT()		INTCONbits.INT0IF = 0


#define DISABLE_TMR0_INTERRUPT()	INTCONbits.TMR0IE = 0
#define ENABLE_TMR0_INTERRUPT()		INTCONbits.TMR0IE = 1

#define DISABLE_TMR1_INTERRUPT()	PIE1bits.TMR1IE = 0
#define ENABLE_TMR1_INTERRUPT()		PIE1bits.TMR1IE = 1


#define DISABLE_UART_TX_INTERRUPT()	PIE1bits.TXIE = 0
#define ENABLE_UART_TX_INTERRUPT()	PIE1bits.TXIE = 1

#define DISABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 0
#define ENABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 1


#define HOLD_ATKBD_CLOCK()		    ATKBD_CLK_DIR= PORT_OUT;\
									ATKBD_CLK = 0;

#define RELEASE_ATKBD_CLOCK()	  	ATKBD_CLK_DIR = PORT_IN;\
									ATKBD_CLK = 1;

#define SET_INT0_FALLING_EDGE_TRIGGER()	INTCON2bits.INTEDG0 = 0

#define	ATKBD_INTERRUPT_EDGE	INTCON2.INTEDG0		

#define ENB_485_TX()	TX_EN = 1;
#define ENB_485_RX()	TX_EN = 0

#define Delay10us(us)		Delay10TCYx(((GetInstructionClock()/1000000)*(us)))
#define DelayMs(ms)												\
	do																\
	{																\
		unsigned int _iTemp = (ms); 								\
		while(_iTemp--)												\
			Delay1KTCYx((GetInstructionClock()+999999)/1000000);	\
	} while(0)

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

extern void BRD_init(void);

#endif
/*
*  End of device.h
*/