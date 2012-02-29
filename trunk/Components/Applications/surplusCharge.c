/*************************************
 * get surplusCharge information
 *
 * Create date: 2011/11/21 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

/* just a test implement
 */
void getSurplusChargeHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	sprintf(logTemp, "%d", getMyAddress()); 
	memcpy(appOutFrame->msg, logTemp, strlen(logTemp));
	appOutFrame->len = strlen(logTemp);
	return;
}

