/*************************************
 * get log data
 *
 * Create date: 2011/11/22 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

extern uint8 logBuffer[LOGBUFFERLEN];
extern volatile uint16 logIndex;

/* just a test implement 
 */
void getLogHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	if(logIndex != 0)
	{	
		if(logIndex < LOGBUFFERLEN)
			memset(&(appOutFrame->msg[logIndex]),0x00,(LOGBUFFERLEN-logIndex));
		memcpy(appOutFrame->msg, logBuffer, logIndex);
		appOutFrame->len = logIndex;
		logTemp[0] = 2;/* the length of logTemp*/
		logTemp[1] = SEND_LOG;
		log(INFO_OTHER, logTemp);
	}
	else
	{		
		logTemp[0] = 2;/* the length of logTemp*/
		logTemp[1] = NO_LOG;
		log(INFO_OTHER, logTemp);
		memcpy(appOutFrame->msg, logBuffer, logIndex);
		appOutFrame->len = logIndex;
		//appOutFrame->len = strlen((char*)appOutFrame->msg);
	}
	return;	
}

