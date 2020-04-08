// #include "../header/asl.h"
// #include "../header/pcb.h"
// #include "../header/const.h"

#include "../header/listx.h"
#include "../header/const.h"
#include "../header/pcb.h"
#include "../header/asl.h"
#include "../header/types_bikaya.h"
#include "../header/p1test_bikaya_v0.h"


/**************************************************************
* Modulo di ASL, gestione di una active semaphore list(ASL),
* che gestituire la coda del process bloccati su un semaforo.
* Ad ogni semaforo e` associato un descrittore(SEMD).
**************************************************************/





//lista di semafori liberi
HIDDEN LIST_HEAD(semdFree);
//lista di semafori non liberi
HIDDEN LIST_HEAD(semdFree_h);
//descrittori di semafori disponibili
HIDDEN struct semd_t semdTable[MAXPROC];



void initASL(){                                       //inizializzare la lista dei semafori
    int i;
    for( i = 0; i < MAXPROC; i++ ){
    INIT_LIST_HEAD( &semdTable[i].s_procQ ) ;
        list_add( &(semdTable[i].s_next) , &(semdFree) );
    }
}






/*Funzione 14: restituisce il puntatore al SEMD nella ASL
* la cui chiave è pari a key. Se non esiste un elemento
* nella ASL con chiave eguale a key, viene restituito NULL. */
struct semd_t* getSemd(int* key){
	struct list_head *tmp ;
	tmp = semdFree_h.next ;
	while( tmp != &semdFree_h  && (((container_of(tmp, struct semd_t, s_next) )->s_key) < key) ) tmp = tmp->next;
	if( (container_of(tmp, struct semd_t, s_next) )->s_key == key ) return container_of( tmp , struct semd_t, s_next ) ;
	else return NULL ;
}


/*Funzione 15: Viene inserito il PCB puntato da p nella coda
* dei processi bloccati associata al SEMD con chiave key.
* Se il semaforo corrispondente non è presente nella ASL,
* alloca un nuovo SEMD dalla lista di quelli liberi (semdFree)
* e lo inserisce nella ASL, settando I campi in maniera opportuna (i.e. key e s_procQ).
* Se non è possibile allocare un nuovo SEMD
* perché la lista di quelli liberi è vuota, restituisce TRUE.
* In tutti gli altri casi, restituisce FALSE.                                     */
int insertBlocked(int *key, struct pcb_t *p){
    if( p == NULL ) return NULL;
    struct list_head *temp , *tmp;
    struct semd_t* semd;
    semd = getSemd( key ) ;
    temp = semdFree_h.next;
    if( semd == NULL ){
	if( list_empty( &(semdFree) )) return TRUE;                //ritorna true se quelli liberi sono vuota
	while( temp != &semdFree_h  && (((container_of(temp, struct semd_t, s_next) )->s_key) < key) ) temp = temp->next;  //inserire il nuovo semaforo nel ASL
        tmp = semdFree.next;
        list_del(tmp);
        list_add(tmp , temp->prev );                           //settare i campi in maniera opportuna
        semd = container_of( tmp , struct semd_t , s_next );
        semd->s_key = key;
    }
    p->p_semkey = semd->s_key;
    list_add_tail(&(p->p_next) , &(semd->s_procQ));
    return 0;
}



/*Funzione 16: Ritorna il primo PCB dalla coda dei processi
* bloccati (s_ProcQ) associata al SEMD della ASL con chiave key.
* Se tale descrittore non esiste nella ASL, restituisce NULL.
* Altrimenti, restituisce l’elemento rimosso.
* Se la coda dei processi bloccati per il semaforo diventa vuota,
* rimuove il descrittore corrispondente dalla ASL
* e lo inserisce nella coda dei descrittori liberi (semdFree).   */
pcb_t* removeBlocked(int *key) {
	semd_t *semd;
	semd = getSemd(key);
	pcb_t * tmp;
  if (semd != NULL){
		tmp = removeProcQ( &(semd->s_procQ) );
      if (list_empty(&(semd->s_procQ))) {                     //rimuove il semaforo con quella chiave e  lo aggiunge nella lista dei semafori liberi
			list_del(&(semd->s_next));
			list_add(&(semd->s_next), &(semdFree));
		}
		return tmp;
	}
	else return NULL;
}



/*Funzione 17: Rimuove il PCB puntato da p dalla coda del semaforo
* su cui è bloccato (indicato da p->p_semKey).
* Se il PCB non compare in tale coda, allora restituisce NULL (condizione di errore).
* Altrimenti, restituisce p.
* Se la coda dei processi bloccati per il semaforo diventa vuota,
* rimuove il descrittore corrispondente dalla ASL e lo
* inserisce nella coda dei descrittori liberi (semdFree).    */
pcb_t* outBlocked(pcb_t *p) {
	semd_t *semd = getSemd(p->p_semkey);
	if (semd == NULL) {
		return NULL;                                    //errore condizione
	}
    pcb_t *tmp = outProcQ(&semd->s_procQ, p);           //altrimenti rimuove il processo dalla coda su cui è bloccato
	tmp->p_semkey = NULL;
    if (emptyProcQ(&semd->s_procQ)) {                   //rimuove il semaforo con quella chiave e  lo aggiunge nella lista dei semafori liberi
		list_del(&semd->s_next);
		list_add(&semd->s_next, &semdFree);
	}
	return tmp;
}


/*Funzione 18: Restituisce (senza rimuovere) il puntatore al PCB
* che si trova in testa alla coda dei processi associata al SEMD con chiave key.
* Ritorna NULL se il SEMD non compare nella ASL
* oppure se compare ma la sua coda dei processi è vuota.                     */
pcb_t* headBlocked(int *key) {
	semd_t *semd = getSemd(key);
	if (semd == NULL || emptyProcQ(&semd->s_next)) {
        return NULL;                                                //Non compare nella ASL
	}
    return container_of(semd->s_procQ.next, pcb_t, p_next);         //restituisce il puntatore di testa senza rimuoverlo
}


/* Funzione 19: Rimuove il PCB puntato da p
* dalla coda del semaforo su cui è bloccato (indicato da p->p_semKey).
* Inoltre, elimina tutti i processi dell’albero radicato in p
* (ossia tutti i processi che hanno come avo p)
* dalle eventuali code dei semafori su cui sono bloccati.           */
void outChildBlocked(struct pcb_t *p) {
    if (p == NULL)
        return NULL;
    struct list_head *tmp = p->p_child.next;
    while (tmp != &(p->p_child) && tmp != NULL) {                //restituisce il puntatore
        outChildBlocked(container_of(tmp, struct pcb_t, p_sib));
        outBlocked(container_of(tmp, struct pcb_t, p_sib));
        tmp = tmp->next;
    }
    outBlocked(p);
}
