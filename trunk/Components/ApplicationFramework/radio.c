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
		//BSP_TOGGLE_LED2();
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
	for(uint8 i = 1;i<=MAX_HOPS;i++)
	{
		nextAddr = getNextAddress(data->originalAddr, data->finnalDstAddr, i);
		if(nextAddr)
		{
			data->dstAddr = nextAddr;
			rc = SMPL_Send_Linear(data->dstAddr, (uint8_t*)data, len);
    	if(rc == SMPL_SUCCESS)
			{
				//BSP_TOGGLE_LED1();	
#ifndef LOGINFO
				sprintf((char*)logTemp, "Send to %d success\n", data->dstAddr);	
#else
				sprintf((char*)logTemp, "ST:%dS\n", data->dstAddr);	
#endif
				log(ERROR, logTemp);
				break;
			}
			else
			{
#ifndef LOGINFO				
				sprintf((char*)logTemp, "Send to %d failed for %d times:%s\n", data->dstAddr,i, getErrorText(rc));			
#else
				sprintf((char*)logTemp, "ST:%dF for %ds:%s\n", data->dstAddr,i, rc);				
#endif	
				log(ERROR, logTemp);
			}
		}
		else
		{
			rc = SMPL_NO_LINK;
			break;
		}
	}
	return rc;
}

void InitRFIO(void)
{
#ifdef FWVERSION
	if(FWVERSION == 2)
	{
		P1SEL &= ~0x0C;
		P1DIR |= 0x0C;
	//	P0DIR |= 0x04;
	}
#endif
}
void EnableSend(void)
{
#ifdef FWVERSION
	uint8 myaddress = getMyAddress();
	if(FWVERSION == 2)
	{
		P1_3 = 1;
		P1_2 = 0;	
		MRFI_DelayMs(50);
	}
#endif
}

void EnableRecv(void)
{
#ifdef FWVERSION
	if(FWVERSION == 2)
	{
		P1_3 = 0;
		P1_2 = 1;
	}
#endif
}
