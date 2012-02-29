#include "framework.h"

/* the transmit and receive buffers */
uint8_t rx[MAX_APP_PAYLOAD], tx[MAX_APP_PAYLOAD];
linkID_t	receiveLinkID = 0;
linkID_t	sendLinkID = 0;

struct AppFrame* receive()
{ 
	uint16 		i = 0;
	uint8 		len;
	uint16 		maxTimes = 1000;
	if (SMPL_SUCCESS != SMPL_LinkListen(&receiveLinkID))
	{
		/* blink LED until we link successfully */
		BSP_TOGGLE_LED1( );
		return NULL;
	}
	/* sure we have received data */
	//MRFI_DelayMs( 100 );
	while( SMPL_Receive( receiveLinkID, rx, &len ) != SMPL_SUCCESS)
	{
		if(i >= maxTimes)
		{
			//return NULL;
		}
		//MRFI_DelayMs(10);
		//BSP_TOGGLE_LED2( );
		i++;
	}
	
	return (struct AppFrame*)rx;
}

smplStatus_t send(void *data, uint8 len)
{
	/*
	smplStatus_t rc;
	struct AppFrame *frame = (struct AppFrame*)data;
	uint8 nextAddr;
	nextAddr = getNextAddress(frame->originalAddr, frame->finnalDstAddr, 1);
	if(nextAddr)
	{
		frame->dstAddr = nextAddr;
		while(SMPL_SUCCESS != SMPL_Link(&sendLinkID, nextAddr))
		{
			sprintf(logTemp, "link to %d failed\n", frame->dstAddr);
			log(ERROR, logTemp);
			BSP_TOGGLE_LED2( );
			//return SMPL_NO_LINK;
		}
		log(ERROR, "Link Success\n");
		rc = SMPL_Send(sendLinkID, data, len);
		MRFI_DelayMs( 5 );
		SMPL_Unlink(sendLinkID);
		return rc;
	}
	*/
	return 	SMPL_NO_LINK;
}