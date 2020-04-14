#include "../header/header.h"
#include "../header/p15test_bikaya_v0.h"
#ifdef TARGET_UMPS
#include "../umps/libumps.h"
#include "../umps/arch.h"
#include "../umps/types.h"
#include "../header/const_umps.h"

HIDDEN void initArea(memaddr area, memaddr handler){
  //Creo la nuova area che punta alla vecchia

  state_t* newarea = (state_t *)area;

  //Salvo lo stato attuale del processore
  STST(newarea);

  /*Settato SP a RAMTOP*/
  newarea->reg_sp = RAMTOP;

  /*Settato PC all'handler passato*/
  newarea->pc_epc = handler;

  /*Settata ker mode on, interrupt mascherati & disabilitata virtual memory & attivato timer cpu*/
  newarea->status = (((0 & ~STATUS_INT_UNMASKED) & VM_OFF) & KM_ON) | STATUS_TE;
}

#endif

#ifdef TARGET_UARM
#include "/usr/include/uarm/libuarm.h"
#include "/usr/include/uarm/arch.h"
#include "../header/uarmconst.h"

HIDDEN void initArea(memaddr area, memaddr handler){
  //Creo la nuova area che punta alla vecchia

  state_t* newarea = (state_t *)area;

  //Salvo lo stato attuale del processore
  STST(newarea);

  /*Settato SP a RAMTOP*/
  newarea->sp = RAMTOP;

  /*Settato PC all'handler passato*/
  newarea->pc = handler;

  /*Settata ker mode on, interrupt mascherati & disabilitata virtual memory & attivato timer cpu*/
  // newarea->cpsr = (((0 & ~STATUS_ALL_INT_ENABLE) & CP15_DISABLE_VM) & STATUS_SYS_MODE) | STATUS_ENABLE_TIMER;

  //still to finish
  STATUS_ALL_INT_ENABLE(newarea->cpsr);
  CP15_DISABLE_VM(newarea->CP15_Control);
}

#endif


int n;
pcb_t *tmp;

struct list_head* returnhead(){
  return ready_queue;
}

// void setPC(){
//   tmp->p_s.pc_epc = (memaddr)test3;
// }

int main(){
  termprint("Hola");

  /* Inizializzazione delle new Area*/
  initArea(INT_NEWAREA, (memaddr)interruptHandler);
  initArea(SYSBK_NEWAREA, (memaddr)sysHandler);
  initArea(TLB_NEWAREA, 0);
  initArea(PGMTRAP_NEWAREA, 0);

  /*Inizializzazione Pcb*/
  initPcbs();

  /*Inizializzazione ready_queue*/
  mkEmptyProcQ(&(ready_queue));

  // termprint("estas");
  /*Creazione dei 3 processi per i 3 test*/
  for(n = 3; n > 0; n = n-1){
    tmp = allocPcb();
    // termprint("mui");

    if(tmp != NULL){
      // termprint("tenebre");
      setProc(&(tmp->p_s),n);
      // termprint("bien");

      /*Assegniamo priorità n*/
      tmp->priority = n;

      /*Assegniamo anche la priorità originiale"*/
      tmp->original_priority = n;
      /*switch per assegnare l'indirizzo di memoria del test da eseguire*/

      // termprint("luce");
      switch (n) {
        case 3:
        termprint("\n\r test3: \n\r");
        tmp->p_s.pc = (memaddr)test3;
        // FOR UMPS
        // tmp->p_s.pc_epc = (memaddr)test3;
        break;

        case 2:
        termprint("\n\r test2: \n\r");
        tmp->p_s.pc = (memaddr)test2;
        // FOR UMPS
        // tmp->p_s.pc_epc = (memaddr)test2;
        break;

        case 1:
        termprint("\n\r test1: \n\r");
        tmp->p_s.pc = (memaddr)test1;
        // FOR UMPS
        // tmp->p_s.pc_epc = (memaddr)test1;
        break;

        default: termprint("\n\r default: \n\r");
      }
      // termprint("text");
      /*inserimento del processo nella coda*/
      insertProcQ(&(ready_queue),tmp);

      // if(!emptyProcQ(&ready_queue)){
      //   termprint("\n\r there's something in here");
      // }
      // else
      //   termprint("\n\r fuckin empty");
    }
    else{
      termprint("error");
      PANIC();
    }
  }


  // if(!emptyProcQ(&ready_queue)){
  //   termprint("\n\r there's something in here");
  // }
  // else
  //   termprint("\n\r fuckin empty");
  scheduler();

}
