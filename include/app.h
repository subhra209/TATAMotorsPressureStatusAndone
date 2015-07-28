#include "board.h"

typedef enum 
{
	NORMAL,
	ABNORMAL
}STATUS;
void APP_init(void);
void APP_task(void);

enum
{
	BAUDRATEADD      = 1,
	SLAVEADD       	 = 2,
	LIMITSTART       = 3,
	LIMITEND    	 = 12,
	CURRENTVALUESTART  =13,
	CURRENTVALUEEND  = 17,
	LINESTART		 =18,
	LINEEND			=22	

};

