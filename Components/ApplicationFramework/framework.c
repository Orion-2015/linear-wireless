/*************************************
* distribute network frame to application
*
* zyfa2008@163.com
* bupt fop laboratory
*/

#include "framework.h"

static struct _task taskList[COMMAND_SIZE];
struct AppFrame* pInframe; /* used for application */




BOOL registerTask(uint8 port, void (*taskFunction)(struct AppFrame *pAppInFrame, struct AppFrame* pAppOutFrame), uint8 options)
{
	if(port >= COMMAND_SIZE)
	{
		return FALSE;
	}
	taskList[port].taskFunction = taskFunction;
	taskList[port].options = options;
	return TRUE;	 
}

/* forware frame to next device */
smplStatus_t fowardFrame(struct AppFrame* pAppFrame)
{	
	smplStatus_t result = send(pAppFrame, sizeof(struct AppFrame));
	return result;
}

/* init os 
* register os task and other work
*/
void initOS(void)
{
	uint8 i;
	for(i=0; i<COMMAND_SIZE; i++)
	{
		taskList[i].taskFunction = NULL;
	}
	
	registerTask(GETSURPLUSCHARGE, getSurplusChargeHandle, 0);
	registerTask(GETSTATIONDATA, getStationDataHandle, 0);
	registerTask(GETLOG, getLogHandle, 0);
	registerTask(GETTEST, getTestHandle, 0);
	registerTask(CHANNELHANDLE, channelHandle, 0);
	registerTask(LOGLEVELHANDLE, logLevelHandle, 0);
	registerTask(SCAN, scan, 0x01);
	registerTask(DETECT, detect, 0);
	
}

void runOS(void)
{
	while(1)
	{
		checkRF();
		if(getMyAddress() == APADDRESS)
		{
			checkUART();
			checkButton();
		}
	}
}

void checkButton(void)
{
	if(BSP_BUTTON1())
	{
		BSP_TOGGLE_LED1();
		sendTest2AP();
	}
	if(BSP_BUTTON2())
	{
		BSP_TOGGLE_LED2();
		sendTest2AP();
	}
}


/* if dstAddr is APADDRESS, then send data to computer software by uart
 */
void send2Computer(struct AppFrame* pFrame)
{
#ifdef COMDEBUG
	printf("Receive new message\n");
	printf("port is %d\ncontent is ", pFrame->port);
	if(pFrame->numUnits != 1)
	{
		putchar(pFrame->numUnits);
		putchar(pFrame->currentUnit);
	}
#endif	
	printf("startdata");
	putstr((uint8*)pFrame->msg, pFrame->len);	
	printf("bupt");
	return;
}

void sendTest2AP(void)
{
	uint8 myAddr;
	smplStatus_t rc;
	myAddr = getMyAddress();
	struct AppFrame* appFrame = (struct AppFrame*)tx;
	appFrame->srcAddr = myAddr;
	appFrame->originalAddr = myAddr;
	appFrame->finnalDstAddr = 2;
	appFrame->port = 0x00;
	appFrame->msg[0] = 0x00;
	appFrame->len = 1;
	
	/* send frame */
	rc = send(appFrame, sizeof(struct AppFrame));
	if(rc != SMPL_SUCCESS)
	{
		log(ERROR, "Send Test2AP failed");
	}
}

void checkRF(void)
{
	struct AppFrame *appFrame;
	appFrame = receive();
	if(appFrame == NULL) return;
	handleRF(appFrame, FALSE);
	return;
}

void handleRF(struct AppFrame* pInframe, BOOL fromUART)
{
	uint8_t myAddr;
	smplStatus_t rc;
	
	struct AppFrame* pOutFrame;
	
	myAddr = getMyAddress();
	
	pOutFrame = (struct AppFrame*)tx;
	
	if(pInframe->finnalDstAddr != myAddr)
	{
		/* append information to frame if append flag is true */
		if(pInframe->finnalDstAddr < myAddr && (taskList[pInframe->port].options & 0x01))
		{
			if(taskList[pInframe->port].taskFunction != NULL)
			{
				taskList[pInframe->port].taskFunction(pInframe, NULL);
			}
		}
		
		rc = fowardFrame(pInframe);
		if(rc == SMPL_SUCCESS)
		{
			sprintf((char*)logTemp, "Forward to %d success\n", pInframe->dstAddr);			
			log(INFO, logTemp);
		}
		else
		{
			/* if forward failed and is scan application, then go back route */
			if(SCAN == pInframe->port)
			{
				callApplication(pInframe, pOutFrame);
				log(INFO, "Scan back\n");
			}
			else
			{
				sprintf((char*)logTemp, "Forward to %d failed\n", pInframe->dstAddr);			
				log(INFO, logTemp);
			}
		}		
	}
	else
	{
		if(pInframe->port < COMMAND_SIZE)
		{
			/* if dstAddress is ap address, then send data to computer by uart */
			if(myAddr == APADDRESS)
			{
				if(fromUART)
				{
					if(taskList[pInframe->port].taskFunction != NULL)
					{
						taskList[pInframe->port].taskFunction(pInframe, pOutFrame);
						if(pOutFrame->len != 0)
						{
							pOutFrame->port = pInframe->port;
							pOutFrame->numUnits = 1;
							pOutFrame->currentUnit = 1;
							send2Computer(pOutFrame);
						}
					}					
				}
				else
				{
					send2Computer(pInframe);
				}
			}
			/* else call user callback function to handle request */
			else
			{
				callApplication(pInframe, pOutFrame);
			}
		}
		else
		{
			log(ERROR, "Error port");
		}
	}
}

void callApplication(struct AppFrame* pInframe, struct AppFrame* pOutFrame)
{
	smplStatus_t rc;
	if(taskList[pInframe->port].taskFunction != NULL)
	{
		pOutFrame->len = 0; /* to avoid application does not set msg point */
		taskList[pInframe->port].taskFunction(pInframe, pOutFrame);
		
		pOutFrame->port = pInframe->port;
		pOutFrame->finnalDstAddr = pInframe->originalAddr;
		pOutFrame->originalAddr = getMyAddress();
		pOutFrame->srcAddr = getMyAddress();
		if(pOutFrame->len != 0)
		{
			rc = send(pOutFrame, sizeof(struct AppFrame));
			sprintf(logTemp, "Send result: %d\n", rc);
			consoleAP(logTemp);
		}
	}
}
void consoleAP(char* data)
{
	if(getMyAddress() != APADDRESS) return;
	putstr((uint8*)data, strlen((char*)data));
	return;
}

void checkUART(void)
{
	struct AppFrame *pInFrame = (struct AppFrame *)rx;
	struct UartCommand *cmd;
	
	/* holds length of current message */
	int8 len; 
	
	len = rx_receive_line( (uint8**)&cmd, MAX_APP_PAYLOAD );
	len -= 2; /* remove 0x0d0a end with symbol */
	if( len >= 3 )
	{
#ifdef	COMDEBUG
		printf("Get command: ");
		puthex((uint8*)cmd, len);
		printf(",waiting for result\n");
#endif
				
		/* simulate rf frame */
		pInFrame->finnalDstAddr = cmd->finnalDstAddr;
		pInFrame->srcAddr = APADDRESS; /* from to myself */
		pInFrame->originalAddr = APADDRESS;
		pInFrame->dstAddr = getMyAddress();
		pInFrame->port = cmd->port;
		pInFrame->len = len - UARTCOMMANDHEADSIZE;
		
		memcpy(pInFrame->msg, cmd->msg, cmd->len); 
		
		if(pInFrame->dstAddr == APADDRESS)
		{
			handleRF(pInFrame, TRUE);
		}	
	}	
}


/* framwork entry */
void framework_entry (void)
{
	
	initOS();
	runOS();
	
	/* but in case we do... */
	while (1) ;
}

