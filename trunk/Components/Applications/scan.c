#include "framework.h"

/* scan all nodes
 */
//void scan(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
void scan(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	if(appOutFrame != NULL)
	//if(appInFrame->len == 0)
	{
		appOutFrame->len = MSGMAXLEN;
		appOutFrame->msg[0] = 1;
		appOutFrame->msg[1] = getMyAddress();
	}
	else
	{
		if(appInFrame->msg[0] + 1 > MSGMAXLEN - 1)
		{
#ifndef LOGINFO
			log(ERROR, "scan size is out\n");
#else
			log(ERROR, "SSIO\n");
#endif
			
			return;
		}
		appInFrame->msg[0] = appInFrame->msg[0] + 1;
		appInFrame->msg[appInFrame->msg[0]] = getMyAddress();
	}
	//memcpy(appOutFrame, appInFrame, sizeof((struct AppFrame*)appInFrame));
	return;
}

