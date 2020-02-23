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

#include "../header/pcb.h"
#include "../header/const.h"
#include "../header/listx.h"

int x = 0;
int j = 0;
pcb_t *temp = NULL;

pcb_t pcbfree_h;
pcb_t pcbFree_table[MAXPROC];

void delete_pcb(pcb_t *x){           //Funzione ausiliaria per inizializzare campi
  INIT_LIST_HEAD(&x->p_next) ;
  INIT_LIST_HEAD(&x->p_child) ;
  INIT_LIST_HEAD(&x->p_sib) ;

  // x->p_s.entry_hi = 0;
  // x->p_s.cause = 0;
  // x->p_s.status = 0;
  // x->p_s.pc_epc = 0;
  // x->p_s.hi = 0;
  // x->p_s.lo = 0;
  // for(int i = 0; i < STATE_GPR_LEN; i++){
  //   x->p_s.gpr[i] = 0;
  // }
  x->priority = 0;
  x->p_semkey = NULL;
  x->p_parent = NULL;
}

void initPcbs(void){
  //mkEmptyProcQ(&(pcbfree_h.p_next));//inizializzo la lista
  INIT_LIST_HEAD(&(pcbfree_h.p_next));
  for (int x = 0; x < MAXPROC; x++) {
    //inserisco tutti gli elementi presenti nella tabella dei processi liberi
    freePcb(&pcbFree_table[x]);
  }
}



void freePcb(pcb_t *p){             //funzione che inserisce un processo nella lista dei processi liberi
  insertProcQ(&(pcbfree_h.p_next), p);
}



pcb_t *allocPcb(){//funzione che alloca processi e li rimuove dalla lista di quelli liberi
  pcb_t *pcb = removeProcQ(&(pcbfree_h.p_next));//ritorna il processo in testa e lo rimuove dalla lista
  if(pcb != NULL){//se il processo non possiede gia' i campi settati a NULL o 0 provvedo a settarli
  delete_pcb(pcb);
}
return (pcb);
}



void mkEmptyProcQ(struct list_head *head)//funzione che inizializza una lista
{
  INIT_LIST_HEAD(head);
}

int emptyProcQ(struct list_head *head)//ritorna TRUE se la lista e' vuota
{
  return list_empty(head);
}



void insertProcQ(struct list_head* head, pcb_t * p){//funzione che inserisce un processo in una lista tenendo        conto della sua priorita'
  pcb_t *ptr;
  if (emptyProcQ(head)){
    //controllo se la lista e vuota e nel caso aggiungo il primo elemento
    list_add(&(p->p_next), head);

//    return head;
  }

	list_for_each_entry(ptr, head, p_next) {
    	if (p->priority > ptr->priority) {
    		list_add(&(p->p_next), &(ptr->p_next));
    //		return head;
    	}
	}
//  addokbuf("ciao");

	list_add_tail(&p->p_next, head);
  //return head;
}


pcb_t *headProcQ(struct list_head *head)//restituisce l'elemento in testa di una lista di processi
{
  pcb_t *p;
  if (emptyProcQ(head)){//Se la coda è vuota restituisce NULL
    return NULL;
  }
  else{         //Altrimenti il pcb in testa
    p=container_of(head->next, pcb_t, p_next);
    return (p);
  }

}


pcb_t* removeProcQ(struct list_head* head){//rimuove il processo in testa e lo ritorna
  pcb_t *pcb;
  pcb = headProcQ(head);                    //Preleva il proc dalla testa lo rimuove e lo restituisce
  list_del(head->next);
  return pcb;

}





pcb_t *outProcQ(struct list_head *head, pcb_t *p)//funzione che rimuove un determinato processo dalla lista e lo ritorna
{
  pcb_t *tmp;
  list_for_each_entry(tmp, head, p_next){
    if(p==tmp) /*Ciclo per trovare il processo che si
    desidera rimuovere*/
    {
      list_del(&p->p_next);
      return(p);
    }
  }

  return NULL;//lista vuota
}


//Funzione che verifica se la lista e vuota
int emptyChild(pcb_t *this){
  return list_empty(&(this->p_child));
}

//Funzione che inserisce un nodo come figlio di un pcb puntato
void insertChild(pcb_t *prnt, pcb_t *p){
  list_add_tail(&p->p_sib, &prnt->p_child);
  p->p_parent=prnt;
}

//funzine che rimuove il primo figlio di un pcb padre
pcb_t *removeChild(pcb_t *p)
{
  pcb_t *c;
  if (list_empty(&(p->p_child)))  //controllo se la lista e vuota e nel caso ritorna NULL
  return NULL;
  else
  {
    //prendo il primo elemento
    c=container_of(p->p_child.next, pcb_t, p_sib);

    //verifico, una volta eliminato il primo figlio, se la lista e vuota
    if((container_of(c->p_sib.next, pcb_t, p_child) == p) && (container_of(c->p_sib.prev, pcb_t, p_child) == p))
    INIT_LIST_HEAD(&p->p_child); //reinizializza la lista
    else
    {
      //altrimenti inizializzo il secondo figlio come primo
      p->p_child.next=c->p_sib.prev;
      p->p_child.prev=c->p_sib.next;
    }
    //elimino il primo figlio del padre
    list_del(&(c->p_sib));
    c->p_parent=NULL;
    return c;
  }
}

//fuznione che elimina il pcb puntato da p dalla lista dei figli padre
pcb_t *outChild(pcb_t *p)
{
  pcb_t *prnt;
  //se non ha un padre ritorna NULL
  if(p->p_parent == NULL)
  return NULL;
  else
  {
    //altrimenti controllo se, una volta eliminato il pcb, la lista resulta vuota
    prnt=container_of(&p->p_parent->p_sib, pcb_t, p_sib);
    if((container_of(p->p_sib.next, pcb_t, p_child)== prnt) && (container_of(p->p_sib.prev, pcb_t, p_child) == prnt))
    INIT_LIST_HEAD(&prnt->p_child); //se lo fosse, reinizializzo la lista
    else
    {// altrimenti collego le due foglie collegate al pcb che si sta rimuovendo
      prnt->p_child.next=p->p_sib.prev;
      prnt->p_child.prev=p->p_sib.next;
    }
    //elimino del figlio
    list_del(&(p->p_sib));
    p->p_parent=NULL;
    return p;
  }
}
