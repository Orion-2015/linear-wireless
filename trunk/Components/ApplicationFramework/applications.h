#ifndef __APPLICATIONS_H__
#define __APPLICATIONS_H__

#define GET	 0
#define SET	 1

#define GETSURPLUSCHARGE	0
#define GETSTATIONDATA		1
#define GETLOG						2
#define GETTEST						3
#define CHANNELHANDLE			15
#define LOGLEVELHANDLE  	5
#define SCAN					  	6
#define DETECT  			  	7

enum logInfor  {
	SEND_SUCCESSED = 1,
	SEND_FAILED,
	SEND_LOG_SUCCESSED,
	SEND_LOG_FAILED,
	SEND_CALLBACK_SUCCESSED,
	SEND_CALLBACK_FAILED,
	SEND_TEST_2AP_SUCCESSED,
	SEND_TEST_2AP_FAILED,
	FORWARD_SUCCESSED,
	FORWARD_FAILED,
	SEND_STATION_DATE,
	ERROR_PORT,
	SEND_CHANNEL,
	SET_CHANNEL,
	SEND_LOG,
	NO_LOG,
	LOG_LEVEL_ERROR_INFRAME,
	SET_LOGLEVEL_SIZE_ERROR,
	SCAN_BACK,
	SCAN_SIZEOUT,

};
typedef enum logInfor    logInfor_t;

void getSurplusChargeHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void getStationDataHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void getLogHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void getTestHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void channelHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void logLevelHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void scan(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void detect(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
char* getRcText(smplStatus_t rc);
void InitRFIO(void);
void EnableRecv(void);
void EnableSend(void);
#endif