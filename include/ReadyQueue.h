#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "yakk.h"

typedef struct ReadyQueue {
	TCB* head;
	TCB* tail;
	unsigned int size;
} ReadyQueue;

void initializeReadyQueue(void);

void insertReadyQueue(TCB* tcb);

TCB* peekReadyQueue(void);

TCB* removeReadyQueue(void);

void printReadyQueue(void);

#endif
