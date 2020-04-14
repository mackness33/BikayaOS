#include "../header/interrupts.h"

void interruptHandler(){

	int cause = getCAUSE();

	if(CAUSE_IP_GET(cause, INT_TIMER)) {
	// if(CAUSE_IP_GET(cause, INT_LOCAL_TIMER)) {
		/*Salvo lo stato del processore*/
		saveArea((state_t*)INT_OLDAREA,&(ptr->p_s));

		insertProcQ(&ready_queue, ptr);
	}

	// termprint("\n\r in Interrupt");
	scheduler();
}
