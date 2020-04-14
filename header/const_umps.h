#ifndef _CONST_H
#define _CONST_H

/****************************************************************************
 *
 * This header file contains the global constant & macro definitions.
 *
 ****************************************************************************/

#include "base.h"

/* Maxi number of overall (eg, system, daemons, user) concurrent processes */
#define MAXPROC 20

#define UPROCMAX 3  /* number of usermode processes (not including master proc
											 and system daemons */

#define NULL ((void *) 0)

/* Addresses for new and old areas (where new and	 old processor states are
   stored on exceptions) */
#define INT_NEWAREA 0x2000008c
#define INT_OLDAREA 0x20000000
#define TLB_NEWAREA 0x200001a4
#define TLB_OLDAREA 0x20000118
#define PGMTRAP_NEWAREA 0x200002bc
#define PGMTRAP_OLDAREA 0x20000230
#define SYSBK_NEWAREA 0x200003d4
#define SYSBK_OLDAREA 0x20000348


#define INT_LOCAL_TIMER 1
#define STATUSMASK 0xFF

/* Used for SYS5 processing */
#define SYS5_TYPES 3
#define SYS5_TLB 0
#define SYS5_PGMTRAP 1
#define SYS5_SYSBK 2

#define INT_OLDAREA_INDEX			0
#define INT_NEWAREA_INDEX 		1
#define TLB_OLDAREA_INDEX 		2
#define TLB_NEWAREA_INDEX 		3
#define PGMTRAP_OLDAREA_INDEX	4
#define PGMTRAP_NEWAREA_INDEX	5
#define SYSBK_OLDAREA_INDEX 	6
#define SYSBK_NEWAREA_INDEX 	7


#define SYS3 3


#define RAMBASE *((unsigned int *)0x10000000)
#define RAMSIZE *((unsigned int *)0x10000004)
#define RAMTOP (RAMBASE + RAMSIZE)


/* Physical memory frame size */
#define FRAME_SIZE 4096   /* 4K */
/* Scheduling constants */
#define SCHED_TIME_SLICE 3000     /* in microseconds, aka 3 milliseconds */


/* "current" TOD value (elapsed CPU ticks), converted in microseconds */
#define GET_TODLOW (*((unsigned int *)BUS_TODLOW) / (*(unsigned int *)BUS_TIMESCALE))

/* Set the interval timer with the given value (in microseconds). Convert the
   value in CPU ticks, and load the interval timer register with it */
#define SET_IT(timer_val) ((*((unsigned int *)BUS_INTERVALTIMER)) = (timer_val * (*(unsigned int *)BUS_TIMESCALE)))



/* Utility definitions for the status register: OR the register with these
   to set a specific bit, AND with the opposite to unset a specific bit.
	 For example, to set STATUS_TE, do status = status | STATUS_TE;  to unset
	 it do status = status & ~STATUS_TE */

/* All interrupts unmasked */
#define STATUS_INT_UNMASKED 0x0000FF15

/*IEc, IEp, IEo and IM[1] unmasked*/
#define INT_MASK_TIME_ON	0x00000215

/*VM OFF*/
#define VM_OFF 0xF8FFFFFF

/*Kernel mode ON*/
#define KM_ON  0xFFFFFFD5

#define STATUS_TE  0x08000000

/*Get syscall type*/

#define CAUSE_EXCCODE_GET(cause) (((cause) & 0x0000007c) >> 2)

/*Get interrupt cause*/
#define CAUSE_IP_GET(cause, int_no) ((cause) & (1 << ((int_no) + 8)))


#define	HIDDEN static
#define	TRUE 	1
#define	FALSE	0
#define ON 	1
#define OFF 	0
#define EOS '\0'
#define DEV_PER_INT 8 /* Maximum number of devices per interrupt line */


/*Exceptions types*/
#define EXC_INTERRUPT 0
#define EXC_TLBMOD 1
#define EXC_TLBINVLOAD 2
#define EXC_TLBINVSTORE 3
#define EXC_ADDRINVLOAD 4
#define EXC_ADDRINVSTORE 5
#define EXC_BUSINVFETCH 6
#define EXC_BUSINVLDSTORE 7
#define EXC_SYSCALL 8
#define EXC_BREAKPOINT 9
#define EXC_RESERVEDINSTR 10
#define EXC_COPROCUNUSABLE 11
#define EXC_ARITHOVERFLOW 12
#define EXC_BADPTE 13
#define EXC_PTEMISS 14

#endif
