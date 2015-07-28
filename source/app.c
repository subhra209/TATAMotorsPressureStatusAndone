#include "app.h"
#include "eep.h" 
#include "digitdisplay.h"
#include "mb.h"

#define ACTIVE  1
#define INACTIVE 0
#define REG_INPUT_START 1
#define REG_INPUT_NREGS 17



#define EEPROM_ADDRESS (0)
#define EEPROM_LIMITS_START (4)
#define EEPROM_LINESTART (26)


void updateDisplay (UINT8 line);
void updatestatus(void);
void updateIndicator(void);


typedef struct _LINE
{
	UINT16 upper_limit;
	UINT16 lower_limit;
	UINT16 current_value;
	STATUS status;
	UINT8 intialized;
	UINT8 line_Active;
}LINE;

LINE lines[MAX_LINES] = {0};



// APP Structure
typedef struct _APP
{
	UINT8	baudRate[2];			  // to store baud rate
	BOOL	baudRateUpdated;		  // to store baud rate flag

	UINT8	address[2];				  // to store slave address
	UINT8	addressUpdated;			  // to store slave address flag

	UINT8	limitBuffer[MAX_LINES*4]; // to store higher and lower limits
	UINT8	limitUpdated;			  // to store limits flag

	UINT8	valueBuffer[MAX_LINES*2]; // to store current values of line
	UINT8	valueUpdated[MAX_LINES];  // to store value flag
	UINT8	lineActive[MAX_LINES*2];		//to store line control value.
	UINT8	lineUpdated[MAX_LINES];

	BOOL	lampOn;			  // to store alarm flag
   
}APP;

APP app = {0};

//APP Initialization
void APP_init(void)
{
	UINT8 i;
	UINT16 sbaudrate , saddress;
	eMBErrorCode    eStatus;
	app.baudRate[0] = Read_b_eep(0);
	Busy_eep();
	app.baudRate[1] = Read_b_eep(1);
	Busy_eep();
	sbaudrate = app.baudRate[0];
	sbaudrate <<= 8;
	sbaudrate |= app.baudRate[1]; 
	app.address[0] = Read_b_eep(2);
	Busy_eep();
	app.address[1] = Read_b_eep(3);
	Busy_eep();
	saddress = app.address[0];
	saddress <<= 8;
	saddress |= app.address[1];

	for(i = 0; i < MAX_LINES; i++)
	{
		lines[i].upper_limit = app.limitBuffer[i*2] = Read_b_eep ((i*2)+EEPROM_LIMITS_START);  	//Reads eeprom data: higher byte of Higher limit  
		Busy_eep();
		lines[i].upper_limit <<= 8;
		
		lines[i].upper_limit |=app.limitBuffer[i*2+1]= Read_b_eep ((i*2)+EEPROM_LIMITS_START+1) ;
		Busy_eep();
		

		lines[i].lower_limit = app.limitBuffer[(i*2)+(MAX_LINES*2)] = Read_b_eep ((i*2)+((MAX_LINES*2)+EEPROM_LIMITS_START)) ; //Reads eeprom data: higher byte of lower limit  
		Busy_eep();
		lines[i].lower_limit <<= 8;

		lines[i].lower_limit |= app.limitBuffer[(i*2)+(MAX_LINES*2)+1]= Read_b_eep ((i*2)+((MAX_LINES*2)+EEPROM_LIMITS_START +1)) ; 
		Busy_eep();
		

		
		lines[i].line_Active = app.lineActive[(2*i)+1] = Read_b_eep (EEPROM_LINESTART  + i);
		Busy_eep();

		lines[i].intialized = FALSE;

		updateDisplay(i);   // Initialize display to 0

		 
	}
	
	//modbus configuration
	eStatus = eMBInit( MB_RTU, ( UCHAR )saddress, 0, sbaudrate, MB_PAR_NONE);
	eStatus = eMBEnable(  );	/* Enable the Modbus Protocol Stack. */


	app.lampOn = FALSE;
	// Turn on Dot in the display
	DigitDisplay_DotOn(17, 1);
	DigitDisplay_DotOn(13, 1);
	DigitDisplay_DotOn(9, 1);
	DigitDisplay_DotOn(5, 1);
	DigitDisplay_DotOn(1, 1);


//turn off all lamps
	LAMP_1 = 1;
	LAMP_2 = 1;
	LAMP_3 = 1;
	LAMP_4 = 1;
	LAMP_5 = 1;
	BUZZER = 1;
}

// APP task
void APP_task(void)
{
	UINT8 i, j;
	UINT16 baudRate = 0;
	UINT16 address = 0;

	// If baud rate has been updated store it to EEPROM and Re-initialize Modbus
	if( app.baudRateUpdated == TRUE )							
	{
		Write_b_eep( EEPROM_ADDRESS , app.baudRate[0] );
		Busy_eep( );

		Write_b_eep( EEPROM_ADDRESS+1 , app.baudRate[1] );
		Busy_eep( );

		baudRate = app.baudRate[0];
		baudRate <<= 8;
		baudRate |= app.baudRate[1];

		address = app.address[0];
		address <<= 8;
		address |= app.address[1];

		eMBClose();
		eMBDisable();
		eMBInit( MB_RTU, ( UCHAR ) address, 0, baudRate, MB_PAR_NONE );
		eMBEnable();

		app.baudRateUpdated = FALSE;
	}

	// If address has been updated store it to EEPROM and Re-initialize Modbus
	if( app.addressUpdated == TRUE )							
	{
		Write_b_eep( EEPROM_ADDRESS+2 , app.address[0] );
		Busy_eep( );

		Write_b_eep( EEPROM_ADDRESS+3 , app.address[1] );
		Busy_eep( );

		address = app.address[0];
		address <<= 8;
		address |= app.address[1];

		baudRate = app.baudRate[0];
		baudRate <<= 8;
		baudRate |= app.baudRate[1];

		eMBClose();
		eMBDisable();
		eMBInit( MB_RTU, ( UCHAR )address, 0, baudRate, MB_PAR_NONE );
		eMBEnable();

		app.addressUpdated = FALSE;
	}

	// If limits has been updated store it to EEPROM  
	if( app.limitUpdated == TRUE )							
	{
		UINT8 j;
		for( j = 0, i = 0; i < MAX_LINES; i++, j += 2 )
		{
			
			Write_b_eep( (EEPROM_LIMITS_START+j) , app.limitBuffer[j] );
			Busy_eep( );

			Write_b_eep( (EEPROM_LIMITS_START+j+1) , app.limitBuffer[j+1] );
			Busy_eep( );

			lines[i].upper_limit = app.limitBuffer[j];
			lines[i].upper_limit <<= 8;
			lines[i].upper_limit |= app.limitBuffer[j+1];

			Write_b_eep( (EEPROM_LIMITS_START+(MAX_LINES*2)+j) , app.limitBuffer[j+(MAX_LINES*2)] );
			Busy_eep( );

			Write_b_eep( (EEPROM_LIMITS_START+(MAX_LINES*2)+j+1) , app.limitBuffer[j+(MAX_LINES*2)+1] );
			Busy_eep( );	
	
			lines[i].lower_limit = app.limitBuffer[j+(MAX_LINES * 2)];
			lines[i].lower_limit <<= 8;
			lines[i].lower_limit |= app.limitBuffer[j+(MAX_LINES * 2)+1];
		}

		app.limitUpdated = FALSE;
	}

//if current value update store in LINE struture
	for(i = 0; i< MAX_LINES; i++)
	{
		
		if(app.valueUpdated[i] == TRUE)
		{
			lines[i].current_value = app.valueBuffer[(2*i)];
			lines[i].current_value <<=8;
			lines[i].current_value |= app.valueBuffer[(2*i)+1];
			lines[i].intialized = TRUE;
			updateDisplay(i);                //display with new current value .
		
	

			app.valueUpdated[i] = FALSE;
			
		}

	}


	for(i = 0; i<MAX_LINES; i++)
	{
		if(app.lineUpdated[i] == TRUE)
		{
			Write_b_eep( (EEPROM_LINESTART+i) , app.lineActive[(2*i)+1] );
			Busy_eep( );

			lines[i].line_Active = app.lineActive[(2*i)+1];
			if(app.lineActive[(2*i)+1] == INACTIVE)
			{
				lines[i].current_value = 0;
				updateDisplay(i);
			}
			app.lineUpdated[i] = FALSE;
			
					
		}
}
	
			
	DigitDisplay_DotOn(17, 1);
	DigitDisplay_DotOn(13, 1);
	DigitDisplay_DotOn(9, 1);
	DigitDisplay_DotOn(5, 1);
	DigitDisplay_DotOn(1, 1);

		updatestatus();
		updateIndicator();
	
	

}

void updateDisplay (UINT8 line)
{
	UINT8 i;
	UINT16 currentvalue = lines[line].current_value ;
	UINT8 temp[NO_DIGITS]={0};
	for(i = 0;i < NO_DIGITS;i++)
	{
		temp[NO_DIGITS - i - 1] = currentvalue  % 10 + '0';
		currentvalue  /= 10;
	}

	 DigitDisplay_updateBufferPartial(temp, NO_DIGITS*line, NO_DIGITS);
}


eMBErrorCode
eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{

    eMBErrorCode    eStatus = MB_ENOERR;

    int             iRegIndex;
/*
    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - usRegInputStart );
        while( usNRegs > 0         {
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ =
                ( unsigned char )( usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
 
*/
    return eStatus;

}

eMBErrorCode
eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                 eMBRegisterMode eMode )
{

	UINT8	starting_add = usAddress;
	UINT8	no_regs		 = usNRegs;
	eMBErrorCode    eStatus = MB_ENOERR;

	switch(eMode)
	{
	case MB_REG_WRITE:

    
	while(no_regs > 0)
	{
		if(starting_add == BAUDRATEADD)
		{
			app.baudRate[0] = * pucRegBuffer++;
			app.baudRate[1] = *pucRegBuffer++;
			app.baudRateUpdated = TRUE;
			
		
		}


		else if(starting_add == SLAVEADD)
		{
			app.address[0] = * pucRegBuffer++;
			app.address[1] = *pucRegBuffer++;
			app.addressUpdated = TRUE;
			
		}
		else if(starting_add >= LIMITSTART && starting_add <= LIMITEND )
		{
			app.limitBuffer[2*(starting_add-LIMITSTART)] = *pucRegBuffer++;
			app.limitBuffer[2*(starting_add-LIMITSTART)+1] = *pucRegBuffer++;
			
			app.limitUpdated = TRUE;
			
		}

		else if(starting_add >= CURRENTVALUESTART && starting_add <= CURRENTVALUEEND )
		{
			app.valueBuffer[2*(starting_add-CURRENTVALUESTART)] = *pucRegBuffer++;
			app.valueBuffer[2*(starting_add-CURRENTVALUESTART)+1] = *pucRegBuffer++;
			
		
			app.valueUpdated[starting_add-CURRENTVALUESTART]= TRUE;
			
		}

		else if(starting_add >= LINESTART && starting_add <= LINEEND)
		{
			app.lineActive[2*(starting_add-LINESTART)] = *pucRegBuffer++;
			app.lineActive[2*(starting_add-LINESTART)+1]= *pucRegBuffer++;
			app.lineUpdated[starting_add-LINESTART] = TRUE;
		}


		starting_add++;
		no_regs	--;
	}
    break;

 	case MB_REG_READ: 

	while(no_regs > 0)
	{
		if(starting_add == BAUDRATEADD)
		{
			* pucRegBuffer++ =	app.baudRate[0];
			* pucRegBuffer++ =	app.baudRate[1];		
		
		}


		else if(starting_add == SLAVEADD)
		{
			
			* pucRegBuffer++ = app.address[0];
			* pucRegBuffer++ = app.address[1];
				
		}
		else if(starting_add >= LIMITSTART && starting_add <= LIMITEND )
		{
			*pucRegBuffer++ = app.limitBuffer[2*(starting_add-LIMITSTART)];
			*pucRegBuffer++ = app.limitBuffer[2*(starting_add-LIMITSTART)+1];  
			
		}

		else if(starting_add >= CURRENTVALUESTART && starting_add <= CURRENTVALUEEND )
		{
			 *pucRegBuffer++ = app.valueBuffer[2*(starting_add-CURRENTVALUESTART)];
			 *pucRegBuffer++ = app.valueBuffer[2*(starting_add-CURRENTVALUESTART)+1];
						
		}

		else if(starting_add >= LINESTART && starting_add <= LINEEND)
		{
			*pucRegBuffer++ = app.lineActive[2*(starting_add-LINESTART)];
			*pucRegBuffer++ = app.lineActive[2*(starting_add-LINESTART)+1]; 
		}

		starting_add++;
		no_regs	--;	
	}
   	 break;
	}
	return eStatus;
  }


eMBErrorCode
eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils,
               eMBRegisterMode eMode )
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
    return MB_ENOREG;
}






// to check the status when a new data updated.
void updatestatus(void)
{
	UINT8 i;
	for(i = 0; i < MAX_LINES; i++)
	{
			if(lines[i].line_Active != ACTIVE)
 			{
				lines[i].status = NORMAL;
				continue;
			}
			if((lines[i].current_value <= lines[i].lower_limit) ||  (lines[i].current_value >= lines[i].upper_limit))
				lines[i].status = ABNORMAL;
			else
				lines[i].status = NORMAL;


			
			if(lines[i].intialized == FALSE)
 			{
				lines[i].status = NORMAL;
				continue;
			}
			if((lines[i].current_value <= lines[i].lower_limit) ||  (lines[i].current_value >= lines[i].upper_limit))
				lines[i].status = ABNORMAL;
			else
				lines[i].status = NORMAL;
	
	}
}



void updateIndicator(void)
{
	UINT8 i, abnormal = FALSE;
	app.lampOn = (app.lampOn == TRUE) ? (FALSE) : (TRUE);

	for(i = 0; i < MAX_LINES; i++)
	{
      	
	    switch(i)
		{
			case 0 : LAMP_1 = (lines[i].status == ABNORMAL)?(abnormal=TRUE,app.lampOn):(0);  //for switch on lamp 1 for line 1.
            break;
    
			case 1 : LAMP_2 = (lines[i].status == ABNORMAL)?(abnormal=TRUE,app.lampOn):(0);   //for switch on lamp 2 for line 2.
            break;
    		
			case 2 : LAMP_3 = (lines[i].status == ABNORMAL)?(abnormal=TRUE,app.lampOn):(0);   //for switch on lamp 3 for line 3.
            break;
    
			case 3 : LAMP_4 = (lines[i].status == ABNORMAL)?(abnormal=TRUE,app.lampOn):(0);   //for switch on lamp 4 for line 4.
            break;
    
			case 4 : LAMP_5 = (lines[i].status == ABNORMAL)?(abnormal=TRUE,app.lampOn):(0);    //for switch on lamp 5 for line 5.
            break;
		}
		
	}
	BUZZER = (abnormal == TRUE)?(1):(0);
}
