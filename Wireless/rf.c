#include "uart.h"
#include "nwk.h"
#include "string.h"

/* let ourterRF to sleep */
void disableOuterRF(void)
{
	P0_6 = 0;
}
void enableOuterRF(void)
{
	P0_6 = 1;
}

void enableOuterRFData(void)
{
	P0_5 =1;
}

void enableOuterRFConfig(void)
{
	P0_5 = 0;
}

void sendRf(uint8* data, uint8 len)
{
	putstr((uint8*)data, len);
}


/* if no data is received, then return zero
 */
uint8 recvRf(uint8** data)
{
	return 	rx_receive_line(data);
}