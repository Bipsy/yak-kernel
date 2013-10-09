#ifndef DELAY_QUEUE_H
#define DELAY_QUEUE_H

typedef struct DelayQueue {
	TCB* head;
	unsigned int size;
} DelayQueue;

void tickClock(DelayQueue* delayQueue);

void insertClock(DelayQueue* delayQueue, TCB* tcb);

void removeClock(DelayQueue* delayQueue);

#endif
