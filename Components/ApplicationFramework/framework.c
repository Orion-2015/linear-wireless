/*************************************
* distribute network frame to application
*
* zyfa2008@163.com
* bupt fop laboratory
*/

#include "framework.h"

static struct _task taskList[COMMAND_SIZE];
struct AppFrame* pInframe; /* used for application */

extern char roomFlag;
extern char getAckFlag;
static uint32 sendTimes = 0;
static uint32 successTimes = 0;
static uint32 failTimes = 0;

void checkFlag()
{
		if(roomFlag == 0)
		{
			printf("no room in last receive------------------\n");
			roomFlag = 1;
		}
		if(getAckFlag == 1)
		{
			printf("i get ack-------------------------\n");
			getAckFlag = 0;
		}
}

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
	//uint16 count = 0x7ff;
	while(1)
	{
		checkRF();
		if(getMyAddress() == APADDRESS)
		{
			checkUART();
			//checkButton();
		}
		else

		{
			checkUART();
			checkButton();
		}
		/*
		count--;
		if(count == 0)
		{
			BSP_TOGGLE_LED2();
			count = 0x7ff;
		}
		*/
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
	printf("port is %d\ncontent is\n", pFrame->port);
	if(pFrame->numUnits != 1)
	{
		putchar(pFrame->numUnits);
		putchar(pFrame->currentUnit);
	}
	
	//printf("startdata");
	if(pFrame->port == SCAN)
	{
		printf("The number of nodes is %d\nThey are ",pFrame->msg[0]);
		for(uint8 j=pFrame->msg[0];j>=1;j--)
		{
			printf("node %d  ",pFrame->msg[j]);	
		}
		uint8 flag_fail = 0;
		for(uint8 k=pFrame->msg[0],i=1;k>=1,i<=pFrame->msg[0];k--,i++)
		{
			if(pFrame->msg[k] != i)	
			{
				failTimes++;
				flag_fail = 1;
				break;				
			}			
		}
		if(flag_fail == 0)
			successTimes++;
		printf("\nsendTimes is %d\n", sendTimes);
		printf("failTimes is %d\n", failTimes);
		printf("successTimes is %d\n", successTimes);
	}	
	else if(pFrame->port == GETLOG)
	{
		uint8 msgIndex = 0;
		while(msgIndex<=pFrame->len)
		{		
			getLogInfor(&(pFrame->msg[msgIndex]));
			msgIndex += (pFrame->msg[0]+1);		
		}	
	}
	else
	{
		putstr((uint8*)pFrame->msg, pFrame->len);
	}
#endif	
	//putstr((uint8*)pFrame->msg, pFrame->len);
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
	appFrame->finnalDstAddr = 1;
	appFrame->port = 0x00;
	sprintf(logTemp, "%d", getMyAddress()); 
	memcpy(appFrame->msg, logTemp, strlen(logTemp));
	appFrame->len = strlen(logTemp);
	/* send frame */
	rc = send(appFrame, sizeof(struct AppFrame));
	if(rc == SMPL_SUCCESS)
	{
		logTemp[0] = 2;/* the length of logTemp*/
		//logTemp[1] = INFO_SEND;
		logTemp[1] = SEND_TEST_2AP_SUCCESSED;
		log(INFO_SEND, logTemp);
	}
	else
	{
		logTemp[0] = 3;/* the length of logTemp*/
		//logTemp[1] = ERROR_SEND;
		logTemp[1] = SEND_TEST_2AP_FAILED;
		logTemp[2] = rc;
		log(ERROR_SEND, logTemp);
	}
/*	{
#ifdef LOGINFO
		sprintf((char*)logTemp, "ST2ApF:%d\n", rc);			
		log(ERROR, logTemp);
#else
		log(ERROR, "Send Test2AP failed");
#endif
	}*/
}

void checkRF(void)
{
	struct AppFrame *appFrame;
	appFrame = receive();
	if(appFrame == NULL) return;
#ifdef COMDEBUG
	checkFlag();
#endif
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
		/* append information to frame if append flag is true ,options~~scan,control sequence*/
		if(pInframe->finnalDstAddr < myAddr && (taskList[pInframe->port].options & 0x01))
		{
			if(taskList[pInframe->port].taskFunction != NULL)
			{
				taskList[pInframe->port].taskFunction(pInframe, NULL);  /*scan,pOutFrame=NULL*/
			}
		}
		
		rc = fowardFrame(pInframe);
		if(rc == SMPL_SUCCESS)
		{
			logTemp[0] = 3;/* the length of logTemp*/
			//logTemp[1] = INFO_SEND;
			logTemp[1] = FORWARD_SUCCESSED;
			logTemp[2] = pInframe->dstAddr;
			log(INFO_SEND, logTemp);

/*#ifndef LOGINFO
			sprintf((char*)logTemp, "Forward to %d success\n", pInframe->dstAddr);
#else
			sprintf((char*)logTemp, "FT %dS\n", pInframe->dstAddr);
			log(INFO, logTemp);
#endif
		*/
			
		}
		else
		{
			/* if forward failed and is scan application, then go back route */
			if(SCAN == pInframe->port && pInframe->finnalDstAddr > myAddr)
			{
				callApplication(pInframe, pOutFrame);
				logTemp[0] = 2;/* the length of logTemp*/
				//logTemp[1] = INFO_OTHER;
				logTemp[1] = SCAN_BACK;
				log(INFO_OTHER, logTemp);
/*
#ifndef LOGINFO
				log(INFO, "Scan back\n");
#else				
				log(INFO, "SB\n");
#endif	
*/
			}
			else
			{
				logTemp[0] = 4;/* the length of logTemp*/
				//logTemp[1] = ERROR_SEND;
				logTemp[1] = FORWARD_FAILED;
				logTemp[2] = pInframe->dstAddr;
				logTemp[3] = rc;
				log(ERROR_SEND, logTemp);
			
			/*
#ifndef LOGINFO				
				sprintf((char*)logTemp, "Forward to %d failed\n", pInframe->dstAddr);	
#else
				sprintf((char*)logTemp, "FT %d F\n", pInframe->dstAddr);	
#endif		
				log(INFO, logTemp);
*/
			}
		}		
	}
	else
	{
		if(pInframe->port < COMMAND_SIZE)
		{
			BSP_TOGGLE_LED1();
			BSP_TOGGLE_LED2();
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
					if(pInframe->port == SCAN)
					{
						taskList[pInframe->port].taskFunction(pInframe, NULL);
						send2Computer(pInframe);
					}
					else
					{
					send2Computer(pInframe);
					}
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
			  logTemp[0] = 2;/* the length of logTemp*/
				//logTemp[1] = ERROR_OTHER;
				logTemp[1] = ERROR_PORT;
				log(ERROR_OTHER, logTemp);
			//log(ERROR, "Error port");
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
	//	if(pInframe->port == GETLOG)
		pOutFrame->port = pInframe->port;
		pOutFrame->finnalDstAddr = pInframe->originalAddr;
		pOutFrame->originalAddr = getMyAddress();
		pOutFrame->srcAddr = getMyAddress();
		if(pOutFrame->len != 0)
		{
			if(pOutFrame->srcAddr == pOutFrame->finnalDstAddr)
			{
				send2Computer(pOutFrame);
			}
			else
			{
				//MRFI_DelayMs(50);
				rc = send(pOutFrame, sizeof(struct AppFrame));
				//if(rc == SMPL_SUCCESS)
				{
					sprintf((char*)logTemp, "Send result: %s\n", getRcText(rc));
					consoleAP(logTemp);
				}
			}
			
		}
	}
}

void debug(char* data)
{
	/* only ap node have uart */
	if(getMyAddress() != APADDRESS) return;		
	putstr((uint8*)data, strlen((char*)data));
	return;
}

void consoleAP(char* data)
{
	if(getMyAddress() != APADDRESS) return;
	debug(data);
}

void checkUART(void)
{
	struct AppFrame *pInFrame = (struct AppFrame *)rx;
	struct UartCommand *cmd;
	
	/* holds length of current message */
	int8 len; 
	
	len = rx_receive_line( (uint8**)&cmd, MAX_APP_PAYLOAD );
	len -= 2; /* remove 0x0d0a end with symbol */
	if( len >= UARTCOMMANDMINSIZE )
	{
		
#ifdef	COMDEBUG
		if(cmd->port == SCAN)
			sendTimes++;
		checkFlag();
		printf("\nGet command: ");
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

void getLogInfor(char* log_Infor)
{
	switch(log_Infor[1])
	{
	case SEND_SUCCESSED:
		printf("INFO_SEND:SEND_SUCCESSED, dstAddr: d%;\n",log_Infor[2]);
		return;
	case SEND_FAILED:
		printf("ERROR_SEND:SEND_FAILED, dstAddr: d%, rc: d%, failed times:d%;\n", log_Infor[2],log_Infor[3], log_Infor[4]);
		return;
	case SEND_TEST_2AP_SUCCESSED:
		printf("INFO_SEND:SEND_TEST_2AP_SUCCESSED;\n");
		return;
	case SEND_TEST_2AP_FAILED:
		printf("ERROR_SEND:SEND_TEST_2AP_FAILED, rc: d%;\n",log_Infor[2]);
		return;
	case FORWARD_SUCCESSED:
		printf("INFO_SEND:FORWARD_SUCCESSED, dstAddr: d%;\n",log_Infor[2]);
		return;
	case FORWARD_FAILED:
		printf("ERROR_SEND:FORWARD_FAILED, dstAddr: d%, rc: d%;\n",log_Infor[2], log_Infor[3]);
		return;
	case SEND_STATION_DATE:
		return;
	case ERROR_PORT:
		printf("ERROR_OTHER:ERROR_PORT;\n");
		return;
	case SEND_CHANNEL:
		printf("INFO_OTHER:SEND_CHANNEL;\n");
		return;
	case SET_CHANNEL:
		printf("INFO_OTHER:SET_CHANNEL;\n");
		return;
	case SEND_LOG:
		printf("INFO_OTHER:SEND_LOG;\n");
		return;
	case NO_LOG:
		printf("INFO_OTHER:NO_LOG;\n");
		return;
	case LOG_LEVEL_ERROR_INFRAME:
		printf("ERROR_OTHER:LOG_LEVEL_ERROR_INFRAME;\n");
		return;
	case SET_LOGLEVEL_SIZE_ERROR:
		printf("ERROR_OTHER:SET_LOGLEVEL_SIZE_ERROR;\n");
		return;
	case SCAN_BACK:
		printf("INFO_OTHER:SCAN_BACK;\n");
		return;
	case SCAN_SIZEOUT:
		printf("ERROR_OTHER:SCAN_SIZEOUT;\n");
		return;
	default:
		return;
	}
}
