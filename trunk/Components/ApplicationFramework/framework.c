/*************************************
* distribute network frame to application
*
* zyfa2008@163.com
* bupt fop laboratory
*/

#include "framework.h"

static struct _task taskList[COMMAND_SIZE];
struct AppFrame* pInframe; /* used for application */




BOOL registerTask(uint8 port, void (*taskFunction)(struct AppFrame *pAppInFrame, struct AppFrame* pAppOutFrame))
{
	if(port >= COMMAND_SIZE)
	{
		return FALSE;
	}
	taskList[port].taskFunction = taskFunction;
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
	
	registerTask(GETSURPLUSCHARGE, getSurplusChargeHandle);
	registerTask(GETSTATIONDATA, getStationDataHandle);
	registerTask(GETLOG, getLogHandle);
	registerTask(GETTEST, getTestHandle);
	registerTask(CHANNELHANDLE, channelHandle);
	registerTask(LOGLEVELHANDLE, logLevelHandle);
	
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
	console("Receive new message\n");
	sprintf(logTemp, "port is %d\ncontent is ", pFrame->port);
	console(logTemp);
	if(pFrame->numUnits != 1)
	{
		putchar(pFrame->numUnits);
		putchar(pFrame->currentUnit);
	}
	tx_send_wait((char*)pFrame->msg, pFrame->len);
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
		rc = fowardFrame(pInframe);
		if(rc == SMPL_SUCCESS)
		{
			sprintf(logTemp, "Forward to %d success\n", pInframe->dstAddr);			
			log(INFO, logTemp);
		}
		else
		{
			sprintf(logTemp, "Forward to %d failed\n", pInframe->dstAddr);			
			log(INFO, logTemp);
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
		}
		else
		{
			log(ERROR, "Error port");
		}
	}
}

bool consoleAP(const void* data)
{
	if(getMyAddress() != APADDRESS) return false;
	return tx_send_wait(data, strlen(data));
}

void checkUART(void)
{
	struct AppFrame *pInFrame = (struct AppFrame *)rx;
	struct UartCommand cmd;
	
	/* holds length of current message */
	uint8_t len; 
	MRFI_DelayMs(5); /* for sure we have receive all the message from uart */
	
	len = rx_receive( &cmd, MAX_APP_PAYLOAD );
	if( len != 0 )
	{
#ifdef	SEND_UART_ECHO
		console("Get command: ");
		tx_send_hex_wait((const uint8*)&cmd, len);
		console(",waiting for result\n");
#endif
				
		/* simulate rf frame */
		pInFrame->finnalDstAddr = cmd.finnalDstAddr;
		pInFrame->srcAddr = APADDRESS; /* from to myself */
		pInFrame->originalAddr = APADDRESS;
		pInFrame->dstAddr = getMyAddress();
		pInFrame->port = cmd.port;
		pInFrame->len = len - UARTCOMMANDHEADSIZE;
		
		memcpy(pInFrame->msg, cmd.msg, cmd.len); 
		
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

