#ifndef PENDQUEUE_H
#define PENDQUEUE_H

typedef struct PendQueue {
	unsigned int size;
	PendQueue* head;
	PendQueue* tail;
} PendQueue;

#endif
