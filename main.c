#include "term.h"
#include "printprint.h"

#ifdef TARGET_UMPS
#include "umps/libumps.h"
#include "umps/arch.h"
#include "umps/types.h"
#endif

#ifdef TARGET_UARM
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#endif
/*
#ifdef TARGET_UARM
#include "libuarm.h"
#include "arch.h"
#endif
*/

#define TOD 0x1000001c			        /*  Numero di istruzioni svolte dalla CPU   */
#define TIME_SCALE 0x10000024

int main(int argc, char *argv[])
{
    term_puts("Inserire stringa: ");
    print_puts("Stringa letta: ");
    char str[64];
    term_gets(str, 64);
   
    if(print_puts(str)){
        term_puts("\nSTAMPA Failed\n");
        return -1;
    }else{
        term_puts("\nStampa eseguita con successo\n");
    }

    /*print_puts(str);*/

    /* Go to sleep indefinetely 
    while (1) 
        WAIT();
    return 0;
    */
    return 0;
}

