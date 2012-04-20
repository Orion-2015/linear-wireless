#include "nwk.h"
#include "rf.h"

struct AppFrame globalAppFrame;

BOOL isMyAddress(struct AppFrame *appFrame)
{
	return myAddress == appFrame->finnalDstAddr ? 1 : 0;
}

BOOL isValidData(struct AppFrame *appFrame)
{
	return myAddress == appFrame->dstAddr ? 1 : 0;
}

uint8 getMyAddress(void)
{
	return myAddress;
}

/* index is 1,2 or 3
 * if has nextAddress, then return address, else return 0
 */
uint8 getNextAddress(uint8 originalAddr, uint8 finnalDstAddr, uint8 index)
{
	uint8 myAddr;
	myAddr = getMyAddress();
	/* if originalAddr < myAddr < finnalDstAddr */
	if(myAddr >= originalAddr && myAddr + index <= finnalDstAddr)
	{
		return myAddr + index;
	}
	
	/* if finnalDstAddr < myAddr < originalAddr */
	if(myAddr <= originalAddr && myAddr - index >= finnalDstAddr)
	{
		return myAddr - index;
	}
	
	/* other wise */
	return 0;
}

struct AppFrame* getGlobalAppFrame(void)
{
	return &globalAppFrame;
}

void initAppFrame(struct AppFrame* appFrame, struct AppFrame* pInFrame)
{
	appFrame->port = pInFrame->port;
	appFrame->finnalDstAddr = pInFrame->originalAddr;
	appFrame->originalAddr = getMyAddress();
	appFrame->srcAddr = getMyAddress();
}

void sendData(struct AppFrame* pOutFrame)
{
	pOutFrame->dstAddr = getNextAddress(pOutFrame->originalAddr, pOutFrame->finnalDstAddr, 1);
	sendRf((uint8*)pOutFrame, sizeof(struct AppFrame));
}