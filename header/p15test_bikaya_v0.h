/*****************************************************************************
 * Copyright 2004, 2005 Michael Goldweber, Davide Brini.                     *
 * Modified 2012 by Marco Di Felice                                          *
 * Modified 2018 by Renzo Davoli
 *                                                                           *
 * This file is part of BIKAYA.                                              *
 *                                                                           *
 * bikaya is free software;you can redistribute it and/or modify it under the*
 * terms of the GNU General Public License as published by the Free Software *
 * Foundation; either version 2 of the License, or (at your option) any      *
 * later version.                                                            *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
 * Public License for more details.                                          *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.                  *
 *****************************************************************************/

/*********************************P1TEST.C*******************************
 *
 *	Test program for the modules ASL and PCB (phase 1).
 *
 *	Produces progress messages on terminal 0 in addition
 *		to the array ``okbuf[]''
 *		Error messages will also appear on terminal 0 in
 *		addition to the array ``errbuf[]''.
 *
 *		Aborts as soon as an error is detected.
 *
 *
 */

// #ifndef P1TEST_BIKAYA_V0_H
// #define P1TEST_BIKAYA_V0_H
//
// #include "const.h"
// #include "listx.h"
//
// #ifdef TARGET_UMPS
// #include "../umps/src/support/libumps/libumps.h"
// #include "../umps/src/include/umps/arch.h"
// #endif
// #ifdef TARGET_UARM
// #include "../uarm/libuarm.h"
// #include "../uarm/arch.h"
// #endif
//
// #include "pcb.h"
// #include "asl.h"
//
// #define MAXSEM MAXPROC
//
// #define MAX_PCB_PRIORITY     10
// #define MIN_PCB_PRIORITY     0
// #define DEFAULT_PCB_PRIORITY 5
//
// char okbuf[2048]; /* sequence of progress messages */
// char errbuf[128]; /* contains reason for failing */
// char msgbuf[128]; /* nonrecoverable error message before shut down */
//
//
//
// int     onesem;
// pcb_t * procp[MAXPROC], *p, *q, *maxproc, *minproc, *proc;
// semd_t *semd[MAXSEM];
// int     sem[MAXSEM + 1];
//
// struct list_head qa;
//
// /******************************************************************************
//  * I/O Routines to write on a terminal
//  ******************************************************************************/
//
// /* This function returns the terminal transmitter status value given its address */
// static unsigned int tx_status(termreg_t *tp);
//
// /* This function prints a string on specified terminal and returns TRUE if
//  * print was successful, FALSE if not   */
// unsigned int termprint(char *str, unsigned int term);
//
// /* This function places the specified character string in okbuf and
//  *	causes the string to be written out to terminal0 */
// void addokbuf(char *strp);
//
// /* This function places the specified character string in errbuf and
//  *	causes the string to be written out to terminal0.  After this is done
//  *	the system shuts down with a panic message */
// void adderrbuf(char *strp);
//
// #endif


#ifndef P15TEST_BIKAYA_V0_H
#define P15TEST_BIKAYA_V0_H


  #include "header.h"

  #ifdef TARGET_UMPS
    #include "../umps/libumps.h"
    #include "../umps/arch.h"
  #endif
  #ifdef TARGET_UARM
    #include "../uarm/libuarm.h"
    #include "../uarm/arch.h"
  #endif

  // #include "const.h"
  // #include "listx.h"
  // #ifdef TARGET_UMPS
  // #include "umps/libumps.h"
  // #include "umps/arch.h"
  // #endif
  //
  // #ifdef TARGET_UARM
  // #include "uarm/libuarm.h"
  // #include "uarm/arch.h"
  // #endif
  // #include "pcb.h"


  #define TOD_LO     *((unsigned int *)BUS_REG_TOD_LO)
  #define TIME_SCALE *((unsigned int *)BUS_REG_TIME_SCALE)
  #define RAMBASE    *((unsigned int *)BUS_REG_RAM_BASE)
  #define RAMSIZE    *((unsigned int *)BUS_REG_RAM_SIZE)
  #define RAMTOP     (RAMBASE + RAMSIZE)

  #define ST_READY         1
  #define ST_BUSY          3
  #define ST_TRANSMITTED   5
  #define CMD_ACK          1
  #define CMD_TRANSMIT     2
  #define CHAR_OFFSET      8
  #define TERM_STATUS_MASK 0xFF

  #define SYS3       3
  #define STEPS      6

  typedef unsigned int devreg;

  static unsigned int get_microseconds();
  static void delay_ms(unsigned int ms);

  /******************************************************************************
   * I/O Routines to write on a terminal
   ******************************************************************************/

  #ifdef TARGET_UMPS
    static termreg_t *term0_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, 0);

    static unsigned int tx_status(termreg_t *tp);

    void termprint(char *str);
  #endif
  #ifdef TARGET_UARM
    #define termprint(str) tprint(str);
  #endif

  void test1();

  void test2();

  void test3();

#endif
