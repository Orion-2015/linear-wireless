#ifndef __FRAMEWORK_H__
#define __FRAMEWORK_H__

#include <stdio.h>
#include <string.h>
#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "nwk_pll.h"
#include "nwk_globals.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"

#ifdef MRFI_CC430
#include "uart_intfc_cc430.h"
#else
#include "uart_intfc.h"
#endif


enum LOGTYPE { ERROR, WARN, INFO, TRACE};
#define LOGBUFFERLEN		500
#define UARTCOMMANDHEADSIZE	3
#define APADDRESS	1
#define SEND_UART_ECHO

struct AppFrame
{
	uint8 port;
	uint8 finnalDstAddr;
	uint8 originalAddr;
	uint8 srcAddr;
	uint8 dstAddr;
	uint8 numUnits;
	uint8 currentUnit;
	uint8 len;
	uint8 msg[6];
};

struct UartCommand
{
	uint8 port;
	uint8 finnalDstAddr;
	uint8 len;
	uint8 msg[MAX_APP_PAYLOAD - 2];
};


struct _task
{
	uint8 port;
	void (*taskFunction)(struct AppFrame *appInFrame, struct AppFrame* appOutFrame);
};

extern char logTemp[LOGTEMP_SIZE];
extern uint8_t rx[MAX_APP_PAYLOAD], tx[MAX_APP_PAYLOAD];

void log(uint8 logType, char* logContent);
void removeOldLog(uint16 newLogLength);
BOOL registerTask(uint8 port, void (*taskFunction)(struct AppFrame *appInFrame, struct AppFrame* appOutFrame));
smplStatus_t fowardFrame(struct AppFrame* _frameStruct);
void initOS(void);
void runOS(void);
void handleUART(void);
void checkRF(void);
void handleRF(struct AppFrame* pInframeStruct, BOOL fromUART);
void checkUART(void);
void checkButton(void);
struct AppFrame* receive(void);
void framework_entry (void);
smplStatus_t send(struct AppFrame* data, uint8 len);
uint8 getNextAddress(uint8 originalAddr, uint8 finnalDstAddr, uint8 index);
void sendTest2AP(void);
bool consoleAP(const void* data);

#include "applications.h"

#endif