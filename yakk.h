#ifndef YAKK.H
#define YAKK.H
#include "yaku.h"


#define null 0;

//Kernel API
void YKInitialize(void);

void YKEnterMutex(void);

void YKExitMutex(void);

void YKIdleTask(void);

void YKNewTask(void (* task) void), void *taskStack, unsigned char priority);

void YKDelayTask(unsigned count);

void YKEnterISR(void);

void YKExitISR(void);

void YKScheduler();

void YKDispatcher(TCB* currentTask, TCB* readyTask, contextBeenSaved);

void YKTickHandler(void);

YKSEM* YKSemCreate(int initialValue);

void YKSemPend(YKSEM* semaphore);

void YKSemPost(YKSEM* semaphore);

YKQ* YKQCreate(void** start, unsigned size);

void YKQPend(YKQ* queue);

int YKQPost(YKQ* queue, void* msg);

YKEVENT* YKEventCreate(unsigned initialValue);

void YKEventSet(YKEVENT* event, unsigned eventMask);

void YKEVentReset(YKEVENT* event, unsigned eventMask);

//Kernel Data Structures

typedef struct {
	unsigned int tid;
	unsigned int priority;
	void* stackPointer;
	void* stackSegment;	
	unsigned int state;
	unsigned int delayCount;
	TCB* next;
	TCB* prev;
}TCB;

typedef struct {
	TCB TCBPool[MAX_TASKS+1];
	unsigned int nextFreeTCB;	
}TaskBlock;

#endif
