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
	
	sprintf((char*)stationDataBuffer, "This is station test data, my address is %d\n", myAddr);
	
	memcpy(appOutFrame->msg, stationDataBuffer, strlen((char*)stationDataBuffer));
	appOutFrame->len = strlen((char*)stationDataBuffer);
	
	log(INFO, "send station data\n");
	return;	
}

