/*************************************
 * get surplusCharge information
 *
 * Create date: 2011/11/21 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

uint8 buffer[1];

/* just a test implement
 */
void getTestHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	//struct _frameStruct* pInframeStruct;
	//pInframeStruct = getFrameStruct();
	//buffer[0] = pInframeStruct->rxMetrics[0];
	//appOutFrame->msg = buffer;
	appOutFrame->msg[0] = 0xd5;
	appOutFrame->len = 1;
	return;
}

