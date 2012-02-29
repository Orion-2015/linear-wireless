#include "framework.h"

/* index is 1,2 or 3
 */
uint8 getNextAddress(uint8 originalAddr, uint8 finnalDstAddr, uint8 index)
{
	uint8 myAddr;
	myAddr = getMyAddress();
	/* if originalAddr < myAddr < finnalDstAddr */
	if(myAddr >= originalAddr && myAddr + index <= finnalDstAddr)
	{
		return myAddr + index;
	}
	
	/* if finnalDstAddr < myAddr < originalAddr */
	if(myAddr <= originalAddr && myAddr - index >= finnalDstAddr)
	{
		return myAddr - index;
	}
	
	/* other wise */
	return 0;
}