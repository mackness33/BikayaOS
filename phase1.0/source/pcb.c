#include "../header/types_bikaya.h"
#include "../header/listx.h"
#include "../header/pcb.h"
#include "../header/const.h"
#include "../header/p1test_bikaya_v0.h"


/***************************************
* MODULO DI PCB, INCLUDE:              *
* ALLOCAZIONE E DEALLOCAZIONE DEI PCB; *
* GESTIOINE DELLE CODE DEI PCB;        *
* GESTIONE DELL'ALBERO DEI PCB.        *
****************************************
*/



//Lista di pcb che sono liberi o inutilizzati
HIDDEN LIST_HEAD(pcbFree);
//arrary di PCB con dimensione massima di MAXPROC
HIDDEN struct pcb_t pcbFree_table[MAXPROC];
//elemento sentinella della lista pcbFree
HIDDEN LIST_HEAD(pcbFree_h);



/***********************************
ALLOCAZIONE E DEALLOCAZIONE DEI PCB
************************************/


/*Funzione 1:
* Inizializza la pcbFree in modo da contenere tutti gli elementi della pcbFree_table.
* Questo metodo deve essere chiamato una volta sola in fase di inizializzazione della struttura dati. */
void initPcbs(void){
  addokbuf("First of All pt.2   \n");
    HIDDEN struct pcb_t pcb_array[MAXPROC];
    int i;
    for(i=0; i<20; i++){
        list_add_tail(&pcb_array[i].p_next, &pcbFree);
    }
}


/*Funzione 2:
* Inserisce il pcb puntato da p nella lista dei PCB liberi */
void freePcb(pcb_t *p){
	    list_add_tail(&p->p_next, &pcbFree);
}

/*Funzione 3:
* Restituisce NULL se la pcbFree e` vuota.
* Altrimenti rimuove un elemento dalla pcbFree,
* inizializza tutti i campi (NULL\0) e restituisce l'elemento rimosso */
struct pcb_t *allocPcb(void){
	if (list_empty(&(pcbFree))) return NULL;  //se pcbFree e` vuota restituisce NULL
	else{
		struct pcb_t *temp;
			/* Elemento precedente alla sentinella è l'ultimo elemento della lista */
		temp=container_of(pcbFree.next, pcb_t, p_next);
		list_del(pcbFree.next);    //rimuovere un elemento dalla pcbFree
		int b = sizeof(temp);
		while(b){
			char *a = temp;
			*a++ = 0;
			b--;
		}                      //diventa tutti i campi a zero
		return temp;
	}
}



/**********************************************************************************
* GESTIONE DEI CODE DI PCB:
* organizzati i pcb in code(dette code di processi, es.coda dei processi attivi);
* collagare tra elementi singlolo o doppio
* geitire la lista tramite list_head(elemento sintinella
**********************************************************************************/


/*Funzione 4: Inizializza la lista dei pcb, inizializzando l'elemmento sentinella */
void mkEmptyProcQ(struct list_head *head){
	INIT_LIST_HEAD(head);
}

/*Funzione 5: Restituisce TRUE se la lista puntata da head e` vuota, FALSE altrimenti */
int emptyProcQ(struct list_head *head) {
    return list_empty(head);
}

/*Funzione 6: inserisce l’elemento puntato da p nella coda dei processi puntata da head.
* L’inserimento deve avvenire tenendo conto della priorita’ di ciascun pcb (campo p->priority).
* La coda dei processi deve essere ordinata in base alla priorita’ dei PCB,
* in ordine decrescente (i.e. l’elemento di testa è l’elemento con la priorita’ più alta).
*/
void insertProcQ(struct list_head *q, struct pcb_t *p){  //6 Inserisce p nella lista la cui list_head è q
    if ( q == NULL || p == NULL)
        return;
    struct list_head *aux, *old;
    struct pcb_t *tmp;
    old = q;
    aux = q->next;
    while (aux != q && aux != NULL) {
        tmp = container_of(aux, struct pcb_t, p_next);
        if (tmp->priority < p->priority)
            break;

        old = aux;
        aux = aux->next;
    }
    if (aux == q)
        list_add_tail( &(p->p_next) , q );
    else
        list_add( &(p->p_next) , old );
}


/*Funzione 7:
* Restituisce l'elemento di testa della coda dei processi da head, senza rimuoverlo.
* Ritorna NULL se la coda non ha elementi  */
pcb_t *headProcQ(struct list_head *head){
	if (list_empty(head)) return NULL;    //se head vuoto torna NULL
	return (container_of(head->next, pcb_t, p_next));  //se non vuota, torna elmento di testa
}

/*Funzione 8:
* Rimuove il primo elemento dalla coda dei processi puntata da head.
* Ritorna NULL se la coda è vuota.
* Altrimenti ritorna il puntatore all'elemento rimosso dalla lista   */
pcb_t *removeProcQ(struct list_head *head){
	if (list_empty(head)) return NULL;     //se lista vuoto ritorna NULL
	/* C'è almeno un elemento nella lista */
	/* Rimuovo il primo e lo restituisco  */
	struct pcb_t *temp;     //puntatore del primo elemento della lista
	temp = container_of(head->next,pcb_t,p_next) ;
        list_del(head->next);
        return temp ;
}

/*Funzione 9:
* Rimuove il PCB puntato da p dalla coda dei processi puntata da head
* Se p non è presente nella coda, restituisce NULL	  */
struct pcb_t *outProcQ(struct list_head *head, pcb_t *p){
	pcb_t* temp;     // puntatore da utilizzare per iterare sul contenuto della lista
	list_for_each_entry(temp, head, p_next){
		if (p == temp) {
			list_del(&(temp->p_next));
			return p;
		}
	}
	return NULL;    //se p non e` presente nella coda ritorna NULL
}



/******************************************************************************
* GESTIONE DELL'ALBERO DEI PCB:
* organizzare i PCB in alberi di processi .
* Ogni genitore contiene un list_head (p_child) che punta alla lista dei figli.
* Ogni figlio ha un puntatore al padre (p_parent)
* ed un list_head che punta alla lista dei fratelli.
********************************************************************************/


/*Funzione 10: restituisce TRUE se il PCB puntato da p non ha figli, restituisce FALSE altrimenti. */
int emptyChild(struct pcb_t *p) {
    if( p->p_child.next == NULL ) return 1; // un list_head i cui campi puntano a NULL si intende come una lista vuota
    else return list_empty( &(p->p_child) );
}


/*Funzione 11: Inserisce il PCB puntato da p come figlio del PCB puntato da prnt */
void insertChild(struct pcb_t *parent, struct pcb_t *p) {
    if (parent == NULL || p == NULL) return;
    if( parent->p_child.next == NULL ) {	//Se i puntatori sono ancora a NULL li inizializziamo correttamente
        INIT_LIST_HEAD( &parent->p_child );
    }
    list_add_tail( &(p->p_sib), &(parent->p_child) );
    p->p_parent = parent;
}


/*Funzione 12: Rimuove il primo figlio del PCB puntato da p. Se non ha figli ritorna NULL */
struct pcb_t *removeChild(struct pcb_t *p) {
    if ( p == NULL || emptyChild(p) ) return NULL;
    struct pcb_t *removedChild;
    struct list_head *q = &p->p_child;
    if (q == NULL || list_empty(q)) return NULL; //return NULL se q è NULL o la lista dei figli di p è vuota
    removedChild = container_of( q->next, typeof(*removedChild), p_sib );
    list_del(q->next);
    removedChild->p_parent = NULL;
    return removedChild;
}

/*Funzione 13: Rimuove il PCB puntato da p dalla lista dei figli del padre.
* Se il PCB puntato da p non ha un padre, restituisce NULL.
* Altrimenti restituisce l’elemento rimosso (cioè p).
* A differenza della removeChild, p può trovarsi in una posizione arbitraria
* (ossia non è necessariamente il primo figlio del padre). */
struct pcb_t *outChild(struct pcb_t *p) {
    if ( p == NULL || p->p_parent == NULL ) return NULL;
    list_del( &(p->p_sib) );
    p->p_parent = NULL;
    return p;
}
