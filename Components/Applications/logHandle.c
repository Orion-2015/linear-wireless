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
	if(logIndex != 0 && logIndex<=40)
	{
		
		memcpy(appOutFrame->msg, logBuffer, logIndex);
		appOutFrame->len = logIndex;
		logTemp[0] = 2;/* the length of logTemp*/
		logTemp[1] = SEND_LOG;
		log(INFO_OTHER, logTemp);
/*
#ifndef LOGINFO
		log(INFO, "Send log\n");
#else
		log(INFO, "SL\n");
#endif   */
	}
	else if(logIndex == 0)
	{
		
		memcpy(appOutFrame->msg, logBuffer, logIndex);
		appOutFrame->len = strlen((char*)appOutFrame->msg);
		logTemp[0] = 2;/* the length of logTemp*/
		logTemp[1] = NO_LOG;
		log(INFO_OTHER, logTemp);
		
/*		
#ifndef LOGINFO
		log(INFO, "No log\n");
#else
		log(INFO, "NL\n");
#endif		*/
		
	}
	return;	
}
