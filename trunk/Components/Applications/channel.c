/*************************************
 * change or get channel
 *
 * Create date: 2011/11/22 
 * Author: yongfa.zhou, email: zyfa2008@163.com
 * bupt fop laboratory
 */

#include "framework.h"

freqEntry_t channel;

/* channel handle
 * if appInFrame->msg[0] == 0x00, then get current channel
 * if is 0x01, then set current channel
 */
void channelHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	if(appInFrame->len == 0) return;
	if(appInFrame->msg[0] == GET)
	{
		SMPL_Ioctl(IOCTL_OBJ_FREQ, IOCTL_ACT_GET, &channel);
		appOutFrame->msg[0] = (channel.logicalChan);
		appOutFrame->len = 1;
		logTemp[0] = 2;/* the length of logTemp*/
		logTemp[1] = SEND_CHANNEL;
		log(INFO_OTHER, logTemp);
		//log(INFO, "Send channel");
		
	}
	else
	{
		if(appInFrame->msg[0] == SET && appInFrame->len >= 2)
		{
			channel.logicalChan =  appInFrame->msg[1];
			SMPL_Ioctl( IOCTL_OBJ_FREQ, IOCTL_ACT_SET, &channel);
			appOutFrame->len = 0;
			logTemp[0] = 2;/* the length of logTemp*/
			logTemp[1] = SET_CHANNEL;
			log(INFO_OTHER, logTemp);			
			//log(INFO, "Set channel");
		}
	}	

	return;	
}

