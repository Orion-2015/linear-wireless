/*************************************
 * log module
 *
 * Create date: 2011/11/22 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

uint8 logBuffer[LOGBUFFERLEN];
volatile uint16 logIndex = 0;
char logTemp[LOGTEMP_SIZE];

uint8 logLevel = INFO; 

/* not implement, send to uart or lcd
 */
void log(uint8 logType, char* logContent)
{

	if(logType <= logLevel)
	{
		if(logIndex + strlen(logContent) + 1 > LOGBUFFERLEN)
		{
			removeOldLog(strlen(logContent) + 1);
		}
		/* olgIndex has updated by removeOldLog() function if has call it */
		memcpy(logBuffer + logIndex, logContent, strlen(logContent) + 1);
		logIndex += (strlen(logContent) + 1);
		
#ifdef COMDEBUG2		
		uint8 myAddr;
		myAddr = getMyAddress();
		if(myAddr == APADDRESS)
		{
			console(logContent);
		}
#endif	
	}
}

/* remove the oldest log, and append new log to the last
 */
void removeOldLog(uint16 newLogLength)
{
	uint16 i;
	uint16 newIndex;
	
	i = newLogLength - 1;
	/* search to the end log content */
	while(logBuffer[i] != '\0' && i < LOGBUFFERLEN)
	{
		i++;
	}
	/* remove all log data */
	if(i == LOGBUFFERLEN)
	{
		logIndex = 0;
		return;
	}
	i++; /* point to new log content */
	newIndex = i;
	
	for(; i<LOGBUFFERLEN; i++)
	{
		logBuffer[i - newIndex] = logBuffer[i];
	}
	logIndex -= newIndex;
	return;
}

