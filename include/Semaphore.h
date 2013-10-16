#ifndef SEMAPHORE_H
#define SEMAPHORE_H

typedef struct Semaphore {
	int value;
	PendQueue queue;
} YKSEM;

YKSEM* YKSemCreate(int initialValue);

YKSemPend(YKSEM* semaphore);

YKSemPost(YKSEM* semaphore);

#endif
