#include "../header/header.h"


HIDDEN semd_t semd_table[MAXPROC];
HIDDEN struct list_head semdfree_h;
HIDDEN struct list_head semd_h;


semd_t* getSemd(int *key) {
	semd_t *sem;
    list_for_each_entry(sem, &semd_h, s_next) {      //Scorro la lista per trovare il semaforo con quella
                                                    //chiave
		if (key == sem->s_key) {
			return sem;
		}
	}
	return NULL;
}


void initASL() {
    INIT_LIST_HEAD(&semd_h);                        //inizializza semd_h
    INIT_LIST_HEAD(&semdfree_h);                    //inizializza semdfree_h


	int i;

	for (i = 0; i < MAXPROC; i++) {
        semd_t *semd = &semd_table[i];              //Mette tutto gli elementi di semd_table
                                                    //nella semdFreee
		list_add(&semd->s_next, &semdfree_h);
	}
}



int insertBlocked(int *key, pcb_t* p) {
	semd_t *semd = getSemd(key);

    if (list_empty(&semdfree_h)) {                  //Lista vuota ritorno true
		return TRUE;
	}

	if (semd != NULL) {
		insertProcQ(&semd->s_procQ, p);             //semd non c'è nella lista e lo aggiunge
		p->p_semkey = key;
		return FALSE;
	}

	semd = container_of(semdfree_h.next, semd_t, s_next);
	list_del(semdfree_h.next);

	INIT_LIST_HEAD(&semd->s_procQ);
    INIT_LIST_HEAD(&semd->s_next);                     //aggiunta della chiave e inizializzazione
                                                        //struttura dati
	semd->s_key = key;

	insertProcQ(&semd->s_procQ, p);

	semd_t *entry_sem;
	list_for_each_entry(entry_sem, &semd_h, s_next) {
		if (semd->s_key > entry_sem->s_key) {
			list_add(&semd->s_next, &entry_sem->s_next);    //Aggiunge in ordina di priorita della chiave
			p->p_semkey = key;
			return FALSE;
		}
	}

	list_add(&semd->s_next, &semd_h);               //Aggiunge in testa
	p->p_semkey = key;
	return FALSE;
}


pcb_t* removeBlocked(int *key) {
	semd_t *semd = getSemd(key);

  if (semd != NULL){
		pcb_t *ptr = container_of(semd->s_procQ.next, pcb_t, p_next);
		ptr->p_semkey = NULL;

		list_del(semd->s_procQ.next);

      if (emptyProcQ(&semd->s_procQ)) {                     //rimuove il semaforo con quella chiave
                                                            //e  lo aggiunge nella lista dei semafori liberi
			list_del(&semd->s_next);
			list_add(&semd->s_next, &semdfree_h);
		}
		return ptr;
	}
	else {
		return NULL;
	}
}


pcb_t* outBlocked(pcb_t *p) {
	semd_t *semd = getSemd(p->p_semkey);

	if (semd == NULL) {
		return NULL;                            //errore condizione
	}

    pcb_t *tmp = outProcQ(&semd->s_procQ, p);           //altrimenti rimuove il processo dalla coda su cui è
                                                        //bloccato
	tmp->p_semkey = NULL;

    if (emptyProcQ(&semd->s_procQ)) {                   //rimuove il semaforo con quella chiave
                                                        //e  lo aggiunge nella lista dei semafori liberi
		list_del(&semd->s_next);
		list_add(&semd->s_next, &semdfree_h);
	}

	return tmp;
}

pcb_t* headBlocked(int *key) {
	semd_t *semd = getSemd(key);

	if (semd == NULL || emptyProcQ(&semd->s_next)) {
        return NULL;                                    //Non compare nella ASL
	}

    return container_of(semd->s_procQ.next, pcb_t, p_next);         //restituisce il puntatore di testa
                                                                    //senza rimuoverlo
}


void outChildBlocked(pcb_t *p) {
    outBlocked(p);                          //restituisce il puntatore di testa
                                            //senza rimuoverlo

	pcb_t *c;
	list_for_each_entry(c, &p->p_child, p_sib) {
		outChildBlocked(c);
	}
}
