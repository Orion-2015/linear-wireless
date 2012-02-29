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
		memcpy(appOutFrame->msg, logBuffer, logIndex);
		appOutFrame->len = logIndex;
		log(INFO, "Send log");
	}
	else
	{
		memcpy(appOutFrame->msg, "No log", 6);
		appOutFrame->len = strlen((char*)appOutFrame->msg);
		log(INFO, "No log");
	}
	return;	
}

