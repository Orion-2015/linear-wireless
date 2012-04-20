#ifndef __RF_H__
#define __RF_H__

void disableOuterRF(void);
void enableOuterRF(void);
void enableOuterRFData(void);
void enableOuterRFConfig(void);
void sendRf(uint8* data, uint8 len);
uint8 recvRf(uint8** data);

#endif