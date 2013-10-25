#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "PriorityQueue.h"

typedef struct Semaphore {
	int value;
	PriorityQueue queue;
} YKSEM;

YKSEM* YKSemCreate(int initialValue);

void YKSemPend(YKSEM* semaphore);

void YKSemPost(YKSEM* semaphore);

#endif
