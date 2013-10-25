#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include "yakk.h"

typedef struct PriorityQueue {
	TCB* head;
	TCB* tail;
	unsigned int size;
} PriorityQueue;

void initializePriorityQueue(PriorityQueue* queue);

void insertPriorityQueue(PriorityQueue* queue, TCB* tcb);

TCB* peekPriorityQueue(PriorityQueue* queue);

TCB* removePriorityQueue(PriorityQueue* queue);

void printPriorityQueue(PriorityQueue* queue);

#endif
