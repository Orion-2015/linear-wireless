#ifndef __APPLICATIONS_H__
#define __APPLICATIONS_H__

#define GET	 0
#define SET	 1

#define GETSURPLUSCHARGE	0
#define GETSTATIONDATA		1
#define GETLOG						2
#define GETTEST						3
#define CHANNELHANDLE			4
#define LOGLEVELHANDLE  	5
#define SCAN					  	6

void getSurplusChargeHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void getStationDataHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void getLogHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void getTestHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void channelHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void logLevelHandle(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);
void scan(struct AppFrame* appInFrame, struct AppFrame* appOutFrame);

#endif