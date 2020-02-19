#ifndef PCB_H
#define PCB_H
#include <types_bikaya.h>
#include "const.h"

#define memaddr MAX_PROC = 20;

pcb_t *pcbFree;                 //: lista dei PCB che sono liberi o inutilizzati.
pcb_t *pcbfree_h;               //: elemento sentinella della lista pcbFree.
pcb_t pcbFree_table[MAX_PROC];  //: array di PCB con dimensione massima di MAX_PROC.

/* PCB handling functions */
HIDDEN void initBack(pcb_t *p);
void addBack(pcb_t *p);
void addFront(pcb_t *p);
bool isEmpty();
pcb_t *getHead(void);
pcb_t *getRear(void);

/* PCB free list handling functions */
void initPcbs(void);
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

#endif