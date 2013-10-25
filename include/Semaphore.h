#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct Semaphore {
	int value;
	PriorityQueue queue;
} YKSEM;

YKSEM* YKSemCreate(int initialValue);

void YKSemPend(YKSEM* semaphore);

void YKSemPost(YKSEM* semaphore);

#endif
