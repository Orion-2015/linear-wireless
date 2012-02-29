#include "framework.h"
#include "nwk.h"

/* the transmit and receive buffers */
uint8_t rx[MAX_APP_PAYLOAD], tx[MAX_APP_PAYLOAD];

struct AppFrame* receive(void)
{ 
	uint8 len;
	if( SMPL_Receive( SMPL_LINKID_LINEAR, rx, &len ) != SMPL_SUCCESS)
	{
		return NULL;
	}
	else
	{
		BSP_TOGGLE_LED2();
		return (struct AppFrame*)rx;
	}
}

smplStatus_t send(struct AppFrame* data, uint8 len)
{
	smplStatus_t 	rc;
	uint8 				nextAddr;
	nextAddr = getNextAddress(data->originalAddr, data->finnalDstAddr, 1);
	if(nextAddr)
	{
		data->dstAddr = nextAddr;
		rc = SMPL_Send_Linear(data->dstAddr, (uint8_t*)data, len);
		BSP_TOGGLE_LED1();
		return rc;
	}
	return 	SMPL_NO_LINK;
}