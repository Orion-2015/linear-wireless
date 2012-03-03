#ifndef _UART_H_
#define _UART_H_

#include "Hal.h"

#define UART_PORT		0	

#define FRAMELENTH  100

struct u
{
	uint8 Buf[FRAMELENTH];
	uint8 count;
	uint8 Flag;
};	

typedef struct u Uart_Rx;		
extern Uart_Rx Uart_R;			
enum n{
	EMPTY,
	FULL,
	RXING,
	CLEARING,
	TIMEROVER,	
	RXOVER,
	};
typedef enum n Uart_State;



/******************************************************************************
 * @fn      initUART
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  none
 *****************************************************************************/

extern void initUART(void );

/******************************************************************************
 * @fn      getUART
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  get char
 *****************************************************************************/

extern char getUART (void );

/******************************************************************************
 * @fn      getchar
 *
 * @brief   Initialize the UART
 *
 * @param   none
 *
 * @return  none
 *****************************************************************************/

extern int  putchar (int c);

void putstr(uint8* msg, uint8 len);
void puthex(uint8* data, uint8 len);
uint8 rx_receive_line(uint8** data, uint8 maxLen);

extern uint8 hasUart(void);

#endif 

