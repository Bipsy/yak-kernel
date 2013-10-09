typedef struct DelayQueue DelayQueue;

struct DelayQueue {
	TCB* head;
	unsigned int size;
};

void tickClock(DelayQueue* delayQueue);

void insertClock(DelayQueue* delayQueue, TCB* tcb);

void removeClock(DelayQueue* delayQueue);

