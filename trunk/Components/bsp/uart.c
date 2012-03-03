
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include "bsp.h"
#include "includes.h"

__interrupt void URX0_ISR(void);

Uart_Rx Uart_R; //串口接收数据 单元



/******************************************************************************
 * @fn      initUART
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  none
 *****************************************************************************/
void initUART(void)
{
#if UART_PORT == 0
		IO_PER_LOC_UART0_AT_PORT0_PIN2345();
  	UART_SETUP(0, 19200, HIGH_STOP);
    INT_SETFLAG(INUM_UTX0, INT_SET);
		IEN0 |= 0x84;				//开总中断，接收中断
		Uart_R.count=0;
		Uart_R.Flag=false;
#else
		IO_PER_LOC_UART1_AT_PORT1_PIN4567();
  	UART_SETUP(1, 19200, HIGH_STOP);
   	INT_SETFLAG(INUM_UTX1, INT_SET);
#endif
}
/******************************************************************************
 * @fn      getUART
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  get char
 *****************************************************************************/
uint8_t hasUart()
{
   	U0CSR |= UART_ENABLE_RECEIVE;
		if(INT_GETFLAG(INUM_URX0))
			return 1;
		else
			 return 0;
}

char getUART (void)
{
   	char c;
   	byte status;

#if UART_PORT == 0
   	// Turning on reception
   	status = U0CSR;
   	U0CSR |= UART_ENABLE_RECEIVE;

   	while (!INT_GETFLAG(INUM_URX0));
   	c = U0DBUF;
	INT_SETFLAG(INUM_URX0, INT_CLR);

   	// Restoring old state
   	U0CSR = status;
#else
   	// Turning on reception
   	status = U1CSR;
   	U1CSR |= 0x40;

   	while (!INT_GETFLAG(INUM_URX1));
   	c = U1DBUF;
 	INT_SETFLAG(INUM_URX1, INT_CLR);

   	// Restoring old state
   	U1CSR = status;
#endif
   	return c;
}
/******************************************************************************
 * @fn      getchar
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  none
 *****************************************************************************/
int putchar (int c)
{		
  	if (c == '\n')
#if UART_PORT == 0
    {
		while (!INT_GETFLAG(INUM_UTX0));
    	INT_SETFLAG(INUM_UTX0, INT_CLR);
		U0DBUF = 0x0d;       /* output CR  */
   	}

   	while (!INT_GETFLAG(INUM_UTX0));
    INT_SETFLAG(INUM_UTX0, INT_CLR);
   	return (U0DBUF = c);
#else
    {
		while (!INT_GETFLAG(INUM_UTX1));
		INT_SETFLAG(INUM_UTX1, INT_CLR);
		U1DBUF = 0x0d;       /* output CR  */
   	}

   	while (!INT_GETFLAG(INUM_UTX1));
   	INT_SETFLAG(UTX1IF, INT_CLR);
   	return (U1DBUF = c);
#endif
}

void putstr(uint8* msg, uint8 len)
{
	for(int i=0;i<len;i++)
		putchar(msg[i]);
	return;
}

void puthex(uint8* data, uint8 len)
{
	extern char logTemp[LOGTEMP_SIZE];
	
	uint8 i;
	if(len * 2 > LOGTEMP_SIZE) return;
	for(i=0; i<len; i++)
	{
		sprintf((char*)(logTemp + 2 * i), "%02x", data[i]);
	}
	putstr((uint8*)logTemp, 2 * len);
	return;
}

uint8 rx_receive_line(uint8** data, uint8 maxLen)
{
	uint8 count;
	if(Uart_R.Flag == true)
	{
		*data = Uart_R.Buf;
		count = Uart_R.count;
		Uart_R.count = 0;
		Uart_R.Flag = false;
		return count;
	}
	else
	{
		return 0;
	}
}

//接收中断
#pragma vector = URX0_VECTOR
 __interrupt void URX0_ISR(void)
{ 	
	URX0IF=0;
	if(Uart_R.Flag==false) //只有当用户已经处理后再处理
	{
		Uart_R.Buf[Uart_R.count] = U0DBUF;

		if(Uart_R.Buf[Uart_R.count]==10&&Uart_R.Buf[Uart_R.count-1]==13)  //如果以\r\n结束，则接收完成
		{ 	
			Uart_R.Flag = true;  //收完,由用户来清除Flag
			//putstr(Uart_R.Buf,Uart_R.count+1);
		}		
		Uart_R.count++;		
	}
	else
	{
		putchar(0x01); //ap addr
		putchar(0x3f); //错误端口号
		putchar(0x01); //错误类型,01表示串口数据还没有被处理		
	}
}

/*------------------------------------------------------------------------------
										  0ooo
								ooo0     (   )
								(   )     ) /
								 \ (     (_/
	    				          \_)        By:zyfa
------------------------------------------------------------------------------*/

