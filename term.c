#include "term.h"
#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#include "umps/types.h"
#endif

#ifdef TARGET_UARM
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "/usr/include/uarm/uARMtypes.h"
#endif
/*
#ifdef TARGET_UARM
#include "uarm/libuarm.h"
#include "uarm/arch.h"
#include "uarm/uARMtypes.h"
#endif
*/

#define ST_READY           1
#define ST_BUSY            3
#define ST_TRANSMITTED     5
#define ST_RECEIVED        5

#define CMD_ACK            1
#define CMD_TRANSMIT       2
#define CMD_RECV           2

#define CHAR_OFFSET        8
#define TERM_STATUS_MASK   0xFF

typedef unsigned int u32;

static u32 tx_status(termreg_t *tp);
static u32 rx_status(termreg_t *tp);

static termreg_t *term0_reg = (termreg_t *) DEV_REG_ADDR(IL_TERMINAL, 0);

/*
void halt(void)
{
    WAIT();
    *((u32 *) MCTL_POWER) = 0x0FF;
    while (1) ;
}
*/

void term_puts(const char *str)
{
    while (*str)
        if (term_putchar(*str++))
            return;
}

void term_gets(char *buf, unsigned int i)
{
    int a;
    while (--i && (a = term_getchar()) != '\n')
        *buf++ = a;
    *buf = '\0';
}

int term_putchar(char c)
{
    u32 stat;

    stat = tx_status(term0_reg);
    if (stat != ST_READY && stat != ST_TRANSMITTED)
        return -1;

    term0_reg->transm_command = ((c << CHAR_OFFSET) | CMD_TRANSMIT);

    while ((stat = tx_status(term0_reg)) == ST_BUSY)
        ;

    term0_reg->transm_command = CMD_ACK;

    if (stat != ST_TRANSMITTED)
        return -1;
    else
        return 0;
}

int term_getchar(void)
{
    u32 stat;

    stat = rx_status(term0_reg);
    if (stat != ST_READY && stat != ST_RECEIVED)
        return -1;

    term0_reg->recv_command = CMD_RECV;

    while ((stat = rx_status(term0_reg)) == ST_BUSY)
        ;

    if (stat != ST_RECEIVED)
        return -1;

    stat = term0_reg->recv_status;

    term0_reg->recv_command = CMD_ACK;

    return stat >> CHAR_OFFSET;
}

static u32 tx_status(termreg_t *tp)
{
    return ((tp->transm_status) & TERM_STATUS_MASK);
}

static u32 rx_status(termreg_t *tp){
    return ((tp->recv_status) & TERM_STATUS_MASK);
}
