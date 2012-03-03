#include "framework.h"

/* scan all nodes
 */
void scan(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	if(appOutFrame != NULL)
	{
		appOutFrame->len = MSGMAXLEN;
		appOutFrame->msg[0] = 1;
		appOutFrame->msg[1] = getMyAddress();
	}
	else
	{
		if(appInFrame->msg[0] + 1 > MSGMAXLEN - 1)
		{
			log(ERROR, "scan size is out\n");
			return;
		}
		appInFrame->msg[0] = appInFrame->msg[0] + 1;
		appInFrame->msg[appInFrame->msg[0]] = getMyAddress();
	}
	return;
}

