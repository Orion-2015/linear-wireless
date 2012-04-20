#ifndef __NWK_H__
#define __NWK_H__

#include "bsp.h"
#define MSGMAXLEN 50

extern uint8 myAddress;

struct AppFrame
{
	uint8 port;
	/* address */
	uint8 finnalDstAddr;
	uint8 originalAddr;
	uint8 srcAddr;
	uint8 dstAddr;
	/* data divition */
	uint8 numUnits;
	uint8 currentUnit;
	/* data */
	uint8 len;
	uint8 msg[MSGMAXLEN];
};

BOOL isMyAddress(struct AppFrame *appFrame);
BOOL isValidData(struct AppFrame *appFrame);
struct AppFrame* getGlobalAppFrame(void);
void initAppFrame(struct AppFrame* appFrame, struct AppFrame* pInFrame);
void sendData(struct AppFrame* pOutFrame);
struct AppFrame* getGlobalAppFrame(void);

#endif