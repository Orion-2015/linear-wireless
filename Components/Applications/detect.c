#include "framework.h"

/* detect uart port
 */
void detect(struct AppFrame* appInFrame, struct AppFrame* appOutFrame)
{
	memcpy(appOutFrame->msg, "test", 4);
	appOutFrame->len = 4;;
	return;
}

