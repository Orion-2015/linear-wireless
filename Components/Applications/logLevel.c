/*************************************
 * get or set log level
 *
 * Create date: 2011/12/2 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

extern uint8 logLevel;

/* just a test implement
 */
void logLevelHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{

	if(appInFrame->len == 0)
	{
		logTemp[0] = 2;/* the length of logTemp*/
		logTemp[1] = LOG_LEVEL_ERROR_INFRAME;
		log(ERROR_OTHER, logTemp);
		//log(ERROR, "log level error inframe");
	}
	else
	{
		if(appInFrame->msg[0] == 0x00)
		{
			appOutFrame->msg[0] = logLevel;
			appOutFrame->len = 1;
		}
		else
		{
			if(appInFrame->len >= 2)
			{
				logLevel = appInFrame->msg[1];				
				memcpy(appOutFrame->msg, "Success", 7);
				appOutFrame->len = strlen("Success");
			}
			else
			{
				logTemp[0] = 2;/* the length of logTemp*/
				logTemp[1] = SET_LOGLEVEL_SIZE_ERROR;
				log(ERROR_OTHER, logTemp);
				//log(ERROR, "Set LogLevel size error");
			}
		}
	}
	return;
}

