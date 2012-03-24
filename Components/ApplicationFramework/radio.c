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

char* getErrorText(smplStatus_t rc)
{
	switch(rc)
	{
	case SMPL_SUCCESS:
		return "SMPL_SUCCESS";
	case SMPL_TIMEOUT:
		return "SMPL_TIMEOUT";
	case SMPL_BAD_PARAM:
		return "SMPL_BAD_PARAM";
	case SMPL_NOMEM:
		return "SMPL_NOMEM";
	case SMPL_NO_FRAME:
		return "SMPL_NO_FRAME";
	case SMPL_NO_LINK:
		return "SMPL_NO_LINK";
	case SMPL_NO_JOIN:
		return "SMPL_NO_JOIN";
	case SMPL_NO_CHANNEL:
		return "SMPL_NO_CHANNEL";
	case SMPL_NO_PEER_UNLINK:
		return "SMPL_NO_PEER_UNLINK";
	case SMPL_TX_CCA_FAIL:
		return "SMPL_TX_CCA_FAIL";
	case SMPL_NO_PAYLOAD:
		return "SMPL_NO_PAYLOAD";
	case SMPL_NO_AP_ADDRESS:
		return "SMPL_NO_AP_ADDRESS";
	case SMPL_NO_ACK:
		return "SMPL_NO_ACK";
	default:
		return "Unknown rc";
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
    if(rc == SMPL_SUCCESS)
		{
			BSP_TOGGLE_LED1();	
			sprintf((char*)logTemp, "Send to %d success\n", data->dstAddr);			
			log(ERROR, logTemp);
		}
		else
		{
			sprintf((char*)logTemp, "Send to %d failed:%s\n", data->dstAddr, getErrorText(rc));			
			log(ERROR, logTemp);			
		}
		return rc;
	}
	return 	SMPL_NO_LINK;
}