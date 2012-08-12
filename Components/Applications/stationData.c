/*************************************
 * get stationData
 *
 * Create date: 2011/11/21 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

static uint8 stationDataBuffer[100];
/* just a test implement 
 */
void getStationDataHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	uint8 myAddr;
	myAddr = getMyAddress();
	sprintf((char*)stationDataBuffer, "my address is %d\n", myAddr);
#ifdef COMDEBUG	
	memcpy(appOutFrame->msg, stationDataBuffer, strlen((char*)stationDataBuffer));
	appOutFrame->len = strlen((char*)stationDataBuffer);
#else	
	memcpy(appOutFrame->msg, &myAddr, sizeof(myAddr));
	appOutFrame->len = sizeof(myAddr);
#endif
//	log(INFO, "send station data\n");
	return;	
}

