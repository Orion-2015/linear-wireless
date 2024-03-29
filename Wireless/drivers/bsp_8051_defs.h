/**************************************************************************************************
  Revised:        $Date: 2007-07-06 11:19:00 -0700 (Fri, 06 Jul 2007) $
  Revision:       $Revision: 13579 $

  Copyright 2007 Texas Instruments Incorporated.  All rights reserved.

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

  YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE PROVIDED �AS IS?  WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY
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

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 *   BSP (Board Support Package)
 *   MCU : 8051
 *   Microcontroller definition file.
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
 */

#ifndef BSP_8051_DEFS_H
#define BSP_8051_DEFS_H

#include "pp_utils.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Defines
 * ------------------------------------------------------------------------------------------------
 */
#define BSP_MCU_8051


#define SFRUNION( name, addr, ... ) __sfr __no_init volatile \
          DECL_BIT_FIELD_UNION( , , unsigned char, name, , __VA_ARGS__ ) @ addr

SFRUNION( IP0xx   ,  0xA9, DECL_BIT_FIELDS_7( unsigned char, , 2, IP0_IPG5, 1, IP0_IPG4, 1, IP0_IPG3, 1, IP0_IPG2, 1, IP0_IPG1, 1, IP0_IPG0, 1 ) );
SFRUNION( IP1xx   ,  0xB9, DECL_BIT_FIELDS_7( unsigned char, , 2, IP1_IPG5, 1, IP1_IPG4, 1, IP1_IPG3, 1, IP1_IPG2, 1, IP1_IPG1, 1, IP1_IPG0, 1 ) );

/* adjust radio interrupts to be lower than the uart but keep the timer
 * interrupts higher than the uart
 */
#define INIT_INTERRUPT_PRIORITY( )                                                                        \
st( IP0_IPG1 = IP1_IPG1 = 1;                           /* timer interrupts are priority 3 (highest) */    \
    IP0_IPG0 = 1; IP1_IPG0 = 0;                        /* radio interrupts are priority 1 */              \
    IP0_IPG2 = IP0_IPG3 = 0; IP1_IPG2 = IP1_IPG3 = 1;  /* uart interrupts are priority 2 */               \
    IP0_IPG4 = IP1_IPG4 = 0; IP0_IPG5 = IP1_IPG5 = 0; ) /* remainder of interrupts priority 0 (lowest) */


/* ------------------------------------------------------------------------------------------------
 *                                     Compiler Abstraction
 * ------------------------------------------------------------------------------------------------
 */

/* ---------------------- IAR Compiler ---------------------- */
#ifdef __IAR_SYSTEMS_ICC__
#define BSP_COMPILER_IAR
#ifndef MCU_H
#error "ERROR:  The MCU include file must be specified.  Define MCU_H=<mcu_file.h> at the project level."
#endif
#include MCU_H
#define __bsp_LITTLE_ENDIAN__   1
#define __bsp_CODE_MEMSPACE__   __code
#define __bsp_XDATA_MEMSPACE__  __xdata

#define __bsp_QUOTED_PRAGMA__(x)    _Pragma(#x)
#define __bsp_ISR_FUNCTION__(f,v)   __bsp_QUOTED_PRAGMA__(vector=v) __interrupt void f(void); \
                                    __bsp_QUOTED_PRAGMA__(vector=v) __interrupt void f(void)

/* ------------------ Unrecognized Compiler ------------------ */
#else
#error "ERROR: Unknown compiler."
#endif


/* ------------------------------------------------------------------------------------------------
 *                                          Common
 * ------------------------------------------------------------------------------------------------
 */
typedef   signed char     int8_t;
typedef   signed short    int16_t;
typedef   signed long     int32_t;

typedef   unsigned char   uint8_t;
typedef   unsigned short  uint16_t;
typedef   unsigned long   uint32_t;

typedef   signed char     int8;
typedef   signed short    int16;
typedef   signed long     int32;

typedef   unsigned char   uint8;
typedef   unsigned short  uint16;
typedef   unsigned long   uint32;

typedef   signed char     BOOL;
#define	  TRUE            1
#define   FALSE			  0
#define __bsp_ENABLE_INTERRUPTS__()         st( EA = 1; )
#define __bsp_DISABLE_INTERRUPTS__()        st( EA = 0; )
#define __bsp_INTERRUPTS_ARE_ENABLED__()    EA

#define __bsp_ISTATE_T__                    unsigned char
#define __bsp_GET_ISTATE__()                EA
#define __bsp_RESTORE_ISTATE__(x)           st( EA = x; )

/**************************************************************************************************
 */
#endif
