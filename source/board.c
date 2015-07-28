
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include "board.h"
#include "config.h"
#include "typedefs.h"




/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

static BOOL ledState;

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void BRD_init(void)
{
	
	ADCON1= 0x0E;
	
	OSCCON = 0x72;  	



	TX1_EN_DIR = PORT_OUT;		// Rs485 Direction Control
	TX1_EN = 0;					//initialized for RX 

	
	SER_TX1_DIR  = PORT_OUT;		//USART Tx 
	SER_RX1_DIR	= PORT_IN;		//USART Rx

	TX2_EN_DIR = PORT_OUT;		// Rs485 Direction Control
	TX2_EN = 0;					//initialized for RX 

	
	SER_TX2_DIR  = PORT_OUT;		//USART Tx 
	SER_RX2_DIR	= PORT_IN;		//USART Rx

//Display Port Direction
	DISPLAY_PORT_LINE_1_2_DIRECTION  = PORT_OUT;
	DISPLAY_PORT_LINE_3_4_DIRECTION	 = PORT_OUT;
	DISPLAY_PORT_LINE_5_DIRECTION	 = PORT_OUT;	    
	DIGIT_SEL_LINE_1_2_DIRECTION     = PORT_OUT; 
	DIGIT_SEL_LINE_3_4_DIRECTION     = PORT_OUT;
	DIGIT_SEL_LINE_5_DIRECTION       = PORT_OUT;
	

//Satus Indicator And Buzzer
	LAMP_1_DIRECTION   = PORT_OUT;
	LAMP_2_DIRECTION   = PORT_OUT;
	LAMP_3_DIRECTION   = PORT_OUT;
	LAMP_4_DIRECTION   = PORT_OUT;
	LAMP_5_DIRECTION   = PORT_OUT;
	BUZZER_DIRECTION   = PORT_OUT;


	HEART_BEAT_DIRECTION = 0;

}



/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*  End of device.c
*/
