#include "../header/header.h"
#include "../header/p15test_bikaya_v0.h"
#ifdef TARGET_UMPS
#include "../umps/libumps.h"
#include "../umps/arch.h"
#include "../umps/types.h"
#include "../header/const_umps.h"

pcb_t *current_p;
struct list_head* ready_queue;

HIDDEN void initArea(memaddr area, memaddr handler);
struct list_head* returnhead();

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

//Set del processo al test
void setPC(memaddr tester){
  current_p->p_s.pc_epc = tester;
}

#endif

int n;
pcb_t *current_p;

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
  newarea->cpsr = STATUS_SYS_MODE;
  STATUS_ENABLE_TIMER(newarea->cpsr);
  STATUS_ALL_INT_ENABLE(newarea->cpsr);
  CP15_DISABLE_VM(newarea->CP15_Control);
}

//Set del processo al test
void setPC(memaddr tester){
  current_p->p_s.pc = tester;
}

#endif



struct list_head* returnhead(){
  return ready_queue;
}


int main(){
  /* Inizializzazione delle new Area*/
  initArea(INT_NEWAREA, (memaddr)interruptHandler);
  initArea(SYSBK_NEWAREA, (memaddr)sysHandler);
  initArea(TLB_NEWAREA, 0);
  initArea(PGMTRAP_NEWAREA, 0);

  initPcbs();

  /*Inizializzazione ready_queue*/
  mkEmptyProcQ(&(ready_queue));

  /*Creazione dei 3 processi per i 3 test*/
  for(n = 3; n > 0; n = n-1){
    current_p = allocPcb();

    if(current_p != NULL){
      setProcess(&(current_p->p_s),n);

      /*Assegniamo priorità*/
      current_p->priority = n;
      current_p->original_priority = n;

      /*switch per assegnare l'indirizzo di memoria del test da eseguire*/
      switch (n) {
        case 3:
        setPC((memaddr)test3);
        break;

        case 2:
        setPC((memaddr)test2);
        break;

        case 1:
        setPC((memaddr)test1);
        break;
      }

      /*inserimento del processo nella coda*/
      insertProcQ(ready_queue,current_p);
    }
    else{
      PANIC();
    }
  }


  scheduler();
}
