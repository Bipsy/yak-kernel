#include "yakk.h"

typedef struct ReadyQueue ReadyQueue;

struct ReadyQueue {
	TCB* head;
	TCB* tail;
	unsigned int size;
};

void initializeReadyQueue(ReadyQueue* readyQueue);

void insertSorted(ReadyQueue* readyQueue, TCB* tcb);

TCB* dequeue(ReadyQueue* readyQueue);
