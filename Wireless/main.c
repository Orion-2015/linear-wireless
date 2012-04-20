#include "bsp.h"
#include "bsp_leds.h"
#include "uart.h"
#include "rf.h"
#include "nwk.h"
#include "application.h"

uint8 myAddress = 7;
void running(void);
void doCommand(struct AppFrame* appFrame);

void initOuterRF(void)
{
	P2SEL &= ~0x01;  
	P2DIR &= ~0x01;
	P0SEL &= ~0x30;
	P0DIR |= 0x60;
}

void main()
{
	BSP_Init( );	
	SET_MAIN_CLOCK_SOURCE(CRYSTAL); 	
	initUART();
	
	initOuterRF();	
 	enableOuterRF();
	enableOuterRFData();

	/* turn on LED. */
	BSP_TURN_ON_LED1( );
	BSP_TURN_ON_LED2( );
	
	running();
	return;
}

void running(void)
{
	uint8		len;
	struct AppFrame* 	appFrame;
	uint16 count = 0xffff;
	while(1)
	{
		len = recvRf((uint8**)&appFrame);
		if(len > 0)
		{
			if(!isValidData(appFrame)) continue;
			if(!isMyAddress(appFrame))
			{
				
			}
			else
			{
				BSP_TOGGLE_LED2();
				doCommand(appFrame);
			}
		}
		
		count--;
		if(count == 0)
		{
			BSP_TOGGLE_LED1();
			count = 0xffff;
		}
	}		
}

void doCommand(struct AppFrame* pInFrame)
{
	struct AppFrame* pOutFrame;
	switch(pInFrame->port)
	{
		case TESTPORT:
		{
			pOutFrame = getTestData(pInFrame);
			sendData(pOutFrame);
		}
	}
}