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

char* getRcText(smplStatus_t rc)
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
	/*if(data->port == GETLOG)
	{
		rc = SMPL_Send_Linear(data->finnalDstAddr, (uint8_t*)data, len);
		NWK_DELAY(50);
		if(rc == SMPL_SUCCESS)
			{
				logTemp[0] = 2;
				logTemp[1] = SEND_LOG_SUCCESSED;
				log(INFO_SEND, logTemp);
			}
			else
			{
				logTemp[0] = 3;
				logTemp[1] = SEND_LOG_FAILED;
				logTemp[2] = rc;
				log(ERROR_SEND, logTemp);
			}
	}
	else   */

			for(uint8 i = 1;i<=MAX_HOPS;i++)
			{
				nextAddr = getNextAddress(data->originalAddr, data->finnalDstAddr, i);
				if(nextAddr)
				{
					data->dstAddr = nextAddr;
					rc = SMPL_Send_Linear(data->dstAddr, (uint8_t*)data, len);
					if(rc == SMPL_SUCCESS)
					{
						memset(logTemp, 0x00, 4);
						logTemp[0] = 3;/* the length of logTemp*/
						logTemp[1] = SEND_SUCCESSED;
						logTemp[2] = nextAddr;
						log(INFO_SEND, logTemp);
						break;
					}
					else
					{
						memset(logTemp, 0x00, 5);
						logTemp[0] = 4;/* the length of logTemp*/
						logTemp[1] = SEND_FAILED;
						logTemp[2] = nextAddr;
						logTemp[3] = rc;
						//logTemp[4] = i;   /*failed times*/
						log(ERROR_SEND, logTemp);
						NWK_DELAY(20);
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
