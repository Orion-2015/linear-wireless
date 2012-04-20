#include "application.h"

struct AppFrame* getTestData(struct AppFrame* pInFrame)
{
	struct AppFrame* appFrame;
	appFrame = getGlobalAppFrame();
	initAppFrame(appFrame, pInFrame);
	appFrame->numUnits = 1;
	appFrame->currentUnit = 1;
	appFrame->len = 1;
	appFrame->msg[0] = 0xcc;
	return appFrame;
}