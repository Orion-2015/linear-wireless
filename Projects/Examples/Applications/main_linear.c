/*----------------------------------------------------------------------------
 *  Demo Application for SimpliciTI
 * 
 *  L. Friedman
 *  Texas Instruments, Inc.
 *---------------------------------------------------------------------------- */

/********************************************************************************************
  Copyright 2004-2007 Texas Instruments Incorporated. All rights reserved.

  IMPORTANT: Your use of this Software is limited to those specific rights granted under
  the terms of a software license agreement between the user who downloaded the software,
  his/her employer (which must be your employer) and Texas Instruments Incorporated (the
  "License"). You may not use this Software unless you agree to abide by the terms of the
  License. The License limits your use, and you acknowledge, that the Software may not be
  modified, copied or distributed unless embedded on a Texas Instruments microcontroller
  or used solely and exclusively in conjunction with a Texas Instruments radio frequency
  transceiver, which is integrated into your product. Other than for the foregoing purpose,
  you may not use, reproduce, copy, prepare derivative works of, modify, distribute,
  perform, display or sell this Software and/or its documentation for any purpose.

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED “AS IS?  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
  WARRANTY OF MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
  IN NO EVENT SHALL TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
  NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER LEGAL EQUITABLE
  THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES INCLUDING BUT NOT LIMITED TO ANY
  INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST
  DATA, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY
  THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

  Should you have any questions regarding your right to use this Software,
  contact Texas Instruments Incorporated at www.TI.com.
**************************************************************************************************/


#include "bsp.h"
#include "mrfi.h"
#include "nwk_types.h"
#include "nwk_api.h"
#include "bsp_leds.h"
#include "bsp_buttons.h"
#include "nwk.h"
#include "nwk_pll.h"
#include "uart_intfc.h"
#include "framework.h"

addr_t   sMyROMAddress = { 2 };

static void startSystem(void);

void toggleLED(uint8_t);
static void start2Babble(void);

/* For FHSS systems, calls to NWK_DELAY() will also call nwk_pllBackgrounder()
 * during the delay time so if you use the system delay mechanism in a loop,
 * you don't need to also call the nwk_pllBackgrounder() function.
 */
#define SPIN_ABOUT_A_SECOND           NWK_DELAY(1000)
#define SPIN_ABOUT_A_QUARTER_SECOND   NWK_DELAY(250)

#define BAD_NEWS   (1)
#define CHECK_RATE (5)

void main (void)
{
	
	BSP_Init( );
	
	SMPL_Init( NULL );
	
	uart_intfc_init( );
	
	/* turn on the radio so we are always able to receive data asynchronously */
	SMPL_Ioctl( IOCTL_OBJ_RADIO, IOCTL_ACT_RADIO_RXON, NULL );
	
	/* turn on LED. */
	BSP_TURN_ON_LED1( );
	BSP_TURN_ON_LED2( );
	console( "Start iwsn system...\n" );
	
  /* never coming back... */
  framework_entry();

  /* but in case we do... */
  while (1) ;
}

static void startSystem(void)
{
  uint8_t msg[1], len;

  /* Turn off LEDs. Check for bad news will toggle one LED. 
   * The other LED will toggle when bad news message is sent.
   */



  while (1)
  {
    //toggleLED(1);
    /* check "sensor" to see if we need to send an alert */
    if (BSP_BUTTON1() || BSP_BUTTON2())
    {
      /* sensor activated. start babbling. */
      start2Babble();
    }

    /* got message? */
    if (SMPL_SUCCESS == SMPL_Receive(SMPL_LINKID_LINEAR, msg, &len))
    {
      /* got something. is it bad news? */
      if (len && (msg[0] == BAD_NEWS))
      {
        /* Yep. start babbling. If there is a filtering token make
         * sure the token matches the expected value.
         */
        //start2Babble();
				console("New message\n");
      }
    }
  }
}


void toggleLED(uint8_t which)
{
  if (1 == which)
  {
    BSP_TOGGLE_LED1();
  }
  else if (2 == which)
  {
    BSP_TOGGLE_LED2();
  }
  return;
}


static void start2Babble()
{
  uint8_t msg[1];

  /* Send the bad news message. To prevent confusion with different "networks"
   * such as neighboring smoke alarm arrays send a token controlled by a DIP 
   * switch, for example, and filter in this token. 
   */
  msg[0] = BAD_NEWS;

	/* babble... */
	SMPL_Send_Linear(0x01, msg, sizeof(msg));
	toggleLED(2);
  
}
