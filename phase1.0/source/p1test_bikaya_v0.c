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

#include "../header/const.h"
#include "../header/listx.h"

#ifdef TARGET_UMPS
#include "../umps/src/support/libumps/libumps.h"
#include "../umps/src/include/umps/arch.h"
// #include "../umps/arch.h"
#endif
#ifdef TARGET_UARM
#include "../uarm/libuarm.h"
#include "../uarm/arch.h"
#endif

#include "../header/p1test_bikaya_v0.h"
#include "../header/asl.h"

#define ST_READY       1
#define ST_BUSY        3
#define ST_TRANSMITTED 5

#define CMD_ACK      1
#define CMD_TRANSMIT 2

#define CHAR_OFFSET      8
#define TERM_STATUS_MASK 0xFF


/******************************************************************************
 * I/O Routines to write on a terminal
 ******************************************************************************/

/* This function returns the terminal transmitter status value given its address */
static unsigned int tx_status(termreg_t *tp) {
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

/* This function prints a string on specified terminal and returns TRUE if
 * print was successful, FALSE if not   */
unsigned int termprint(char *str, unsigned int term) {
    termreg_t *term_reg;

    unsigned int stat;
    unsigned int cmd;

    unsigned int error = FALSE;

    if (term < DEV_PER_INT) {
        term_reg = (termreg_t *)DEV_REG_ADDR(IL_TERMINAL, term);

        /* test device status */
        stat = tx_status(term_reg);
        if ((stat == ST_READY) || (stat == ST_TRANSMITTED)) {
            /* device is available */

            /* print cycle */
            while ((*str != '\0') && (!error)) {
                cmd                      = (*str << CHAR_OFFSET) | CMD_TRANSMIT;
                term_reg->transm_command = cmd;

                /* busy waiting */
                while ((stat = tx_status(term_reg)) == ST_BUSY)
                    ;

                /* end of wait */
                if (stat != ST_TRANSMITTED) {
                    error = TRUE;
                } else {
                    /* move to next char */
                    str++;
                }
            }
        } else {
            /* device is not available */
            error = TRUE;
        }
    } else {
        /* wrong terminal device number */
        error = TRUE;
    }

    return (!error);
}


/* This function places the specified character string in okbuf and
 *	causes the string to be written out to terminal0 */
void addokbuf(char *strp) {

    termprint(strp, 0);
}


/* This function places the specified character string in errbuf and
 *	causes the string to be written out to terminal0.  After this is done
 *	the system shuts down with a panic message */
void adderrbuf(char *strp) {

    termprint(strp, 0);

    PANIC();
}
