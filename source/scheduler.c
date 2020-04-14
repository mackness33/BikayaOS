#include "../header/scheduler.h"

void add_priority(){
  pcb_t* proc;
  list_for_each_entry(proc,ready_queue,p_next){
      proc->priority=proc->priority + 3;
  }
}


void scheduler(){
  if(!emptyProcQ(ready_queue)){

    /*Settaggio timer (SCHED_TIME_SLICE = 3ms)*/
    setTIMER(3000);

    /*rimuovo il processo in testa ovvero quello con priorità maggiore*/
    ptr = removeProcQ(&ready_queue);

    /*aumento della priorità dei processi per evitare starving*/
    add_priority();

    ptr->priority = ptr->original_priority;

    /*Carico lo stato del processo da eseguire*/
    LDST(&(ptr->p_s));
  }
  else{
    /*Sono finiti i processi da eseguire e la macchina va in WAIT*/
    cpuIdle();
  }

}
