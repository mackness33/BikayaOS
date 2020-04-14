#include "../header/utils.h"

#ifdef TARGET_UMPS

void setProc(state_t* temp, int n){
	/*settaggio registri*/
	temp->status = (((0 | INT_MASK_TIME_ON) & VM_OFF) & KM_ON) | STATUS_TE;

  /*Assegnamento StackPointer*/
  temp->reg_sp = RAMTOP - FRAME_SIZE * n;
}

void saveArea(state_t* new, state_t* old) {
  int i; for(i=0;i<29;i++){                  //Copia lo stato della OldArea nello stato del processo passato
	   (*old).gpr[i]=(*new).gpr[i];
  }
	(*old).entry_hi=(*new).entry_hi;
	(*old).cause=(*new).cause;
	(*old).status=(*new).status;
	(*old).pc_epc=(*new).pc_epc;
	(*old).hi=(*new).hi;
	(*old).lo=(*new).lo;
}

#endif

#ifdef TARGET_UARM

void setProc(state_t* temp, int n){
	/*settaggio registri*/
	// temp->status = (((0 | INT_MASK_TIME_ON) & VM_OFF) & KM_ON) | STATUS_TE;
  STATUS_ALL_INT_ENABLE(temp->cpsr);
  CP15_DISABLE_VM(temp->CP15_Control);

  /*Assegnamento StackPointer*/

	temp->sp = RAMTOP - FRAME_SIZE * n;
}

void saveArea(state_t* new, state_t* old) {
  (*old).a1 = (*new).a1;
  (*old).a2 = (*new).a2;
  (*old).a3 = (*new).a3;
  (*old).a4 = (*new).a4;
  (*old).v1 = (*new).v1;
  (*old).v2 = (*new).v2;
  (*old).v3 = (*new).v3;
  (*old).v4 = (*new).v4;
  (*old).v5 = (*new).v5;
  (*old).v6 = (*new).v6;
  (*old).sl = (*new).sl;
  (*old).fp = (*new).fp;
  (*old).ip = (*new).ip;
  (*old).sp = (*new).sp;
  (*old).lr = (*new).lr;
  (*old).pc = (*new).pc;
  (*old).cpsr = (*new).cpsr;
  (*old).CP15_Control = (*new).CP15_Control;
  (*old).CP15_EntryHi = (*new).CP15_EntryHi;
  (*old).CP15_Cause = (*new).CP15_Cause;
  (*old).TOD_Hi = (*new).TOD_Hi;
  (*old).TOD_Low = (*new).TOD_Low;
}

#endif

void cpuIdle(){
  /*Funzione idle che viene invocata quando non ci sono processi in esecuzione*/
  // setSTATUS(getSTATUS() | STATUS_IEc | STATUS_INT_UNMASKED);
  setTIMER(3000);

  while(1) WAIT();
}

void KillProc(pcb_t *pcb){

    if(pcb!=NULL){                        //Controlla che il processo passato non sia NULL, rimuove i processi figli
        while(!emptyChild(pcb)){          //dalla coda, mette a NULL il processo padre e richiama lo scheduler
            KillProc(removeChild(pcb));
        }
        outChild(pcb);
        if(pcb == ptr){
            ptr = NULL;
        }

        outProcQ(&ready_queue, pcb);
        freePcb(pcb);
    }

    scheduler();
}
