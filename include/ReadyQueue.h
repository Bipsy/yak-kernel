#ifndef READY_QUEUE_H
#define READY_QUEUE_H

#include "yakk.h"

typedef struct ReadyQueue {
	TCB* head;
	TCB* tail;
	unsigned int size;
} ReadyQueue;

void initializeReadyQueue(ReadyQueue* readyQueue);

void insertSorted(ReadyQueue* readyQueue, TCB* tcb);

TCB* dequeue(ReadyQueue* readyQueue);

#endif
