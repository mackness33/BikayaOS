#include "pcb.h"

/* PCB handling functions */
// need to alloc pcbFree_h then set next to NULL
// then let pcbFree point to pcbFree_h
void init(pcb_t *p){
  pcbFree = NULL;
  pcbFree_h->next = pcbFree;
  pcbFree->next = pcbFree_h;
}

// can become a bool if needed it
// TODO: either add to front and back
void addBack(pcb_t *p){
  if(!p)
    return;
}
void addFront(pcb_t *p);
bool isEmpty();
pcb_t *getHead(void);
pcb_t *getRear(void);

/* PCB free list handling functions */
// TODO: Create a method for the inizialization of the array
// TODO: Create a list class with all the we need for this.
void initPcbs(void){
  // FIRST: I need to inizialize all the pcb and put them in the array.
  // NEXT:  Point all the list's pcb to the array.
}

void freePcb(pcb_t *p);
pcb_t *allocPcb(void);

/* PCB queue handling functions */
void mkEmptyProcQ(struct list_head *head);
int emptyProcQ(struct list_head *head);
void insertProcQ(struct list_head *head, pcb_t *p);
pcb_t *headProcQ(struct list_head *head);

pcb_t *removeProcQ(struct list_head *head);
pcb_t *outProcQ(struct list_head *head, pcb_t *p);


/* Tree view functions */
int emptyChild(pcb_t *this);
void insertChild(pcb_t *prnt, pcb_t *p);
pcb_t *removeChild(pcb_t *p);
pcb_t *outChild(pcb_t *p);
