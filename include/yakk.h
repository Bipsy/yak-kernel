#ifndef YAKK_H
#define YAKK_H

#include "yaku.h"

#define null 0
#define IDLETASKSTACKSIZE 512

//Error Codes
#define NEW_TASK_FAILED 1
#define READY_QUEUE_EMPTY 2
#define NEW_SEM_FAILED 3
#define NEW_QUEUE_FAILED 4
#define NEW_EVENT_FAILED 5
#define EVENT_PEND_ERROR 6
#define EVENT_SET_ERROR 7
#define GAME_OVER 8

//Event Codes
#define EVENT_WAIT_ANY 0
#define EVENT_WAIT_ALL 1

//Simptris Goodies
#define TASK_STACK_SIZE   	512       /* stack size in words */
#define MSGQSIZE          	20
#define SLIDE				0
#define ROTATE				1
#define CORNER				0
#define COUNTERCLOCKWISE	0
#define CLOCKWISE			1
#define RIGHT				1
#define LEFT				0
#define HORIZONTAL			0
#define VERTICAL			1

//Kernel Data Structures

enum TaskState {T_BLOCKED, T_READY, T_RUNNING};
enum KernelState {K_BLOCKED, K_RUNNING};

typedef struct TCB {
	unsigned int eventMask;
	unsigned char waitMode;
	unsigned char priority;
	void* stackPointer;	
	enum TaskState state;	
	unsigned int delayCount;
	struct TCB* next;
	struct TCB* prev;
} TCB;

typedef struct TaskBlock {
	TCB TCBPool[MAX_TASKS+1];
	unsigned int nextFreeTCB;	
} TaskBlock;

typedef struct PriorityQueue {
	TCB* head;
	TCB* tail;
	unsigned int size;
} PriorityQueue;

typedef struct DelayQueue {
	TCB* head;
	unsigned int size;
} DelayQueue;

typedef struct Semaphore {
	int value;
	PriorityQueue queue;
} YKSEM;

typedef struct SemBlock {
	YKSEM SemPool[MAX_SEMS];
	unsigned int nextFreeSem;
} SemBlock;

typedef struct MessageQueue {
	void** messages;
	unsigned int currentSize;
	unsigned int maxSize;
	PriorityQueue queue;
} YKQ;

typedef struct MessageQueueBlock {
	YKQ QueuePool[MAX_QUEUES];
	unsigned int nextFreeQueue;
} MsgQueueBlock;

typedef struct Event {
	unsigned int mask;
	PriorityQueue queue;
} YKEVENT;

typedef struct EventBlock {
	YKEVENT EventPool[MAX_EVENTS];
	unsigned int newFreeEvent;
} EventBlock;

typedef struct Piece {
	unsigned int id;
	unsigned int type;
	unsigned int orientation;
	unsigned int column;
} Piece;

typedef struct Move {
	unsigned int id;
	unsigned int direction;
	unsigned int function;
	unsigned int times;
} Move;

//Kernel API
void YKInitialize(void);
void YKEnterMutex(void);
void YKExitMutex(void);
void YKIdleTask(void);
void YKNewTask(void (* task) (void), void *taskStack, unsigned char priority);
void YKDelayTask(unsigned count);
void YKEnterISR(void);
void YKExitISR(void);
unsigned int YKGetISRCallDepth(void);
void YKScheduler(void);
void YKDispatcher(TCB* readyTask);
void YKTickHandler(void);
YKSEM* YKSemCreate(int initialValue);
void YKSemPend(YKSEM* semaphore);
void YKSemPost(YKSEM* semaphore);
YKQ* YKQCreate(void** start, unsigned size);
void* YKQPend(YKQ* queue);
int YKQPost(YKQ* queue, void* msg);
YKEVENT* YKEventCreate(unsigned initialValue);
unsigned YKEventPend(YKEVENT* event, unsigned eventMask, int waitMode);
void YKEventSet(YKEVENT* event, unsigned eventMask);
void YKEventReset(YKEVENT* event, unsigned eventMask);
TCB* getNewTCB(void);
YKSEM* getNewSem(void);
YKQ* getNewQueue(void);
YKEVENT* getNewEvent(void);
void YKRun(void);
void breakpoint();

#endif
