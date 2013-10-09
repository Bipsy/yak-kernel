#include "../include/clib.h"
#include "../include/yakk.h"
#include "../include/yaku.h"

//User Accessible Variables
unsigned int YKCtxSwCount = 0;
unsigned int YKIdleCount = 0;
unsigned int YKTickNum = 0;

//Kernel Accessible Variables
static unsigned int ISRCallDepth = 0;
static TCB* currentTask = null;
static ReadyQueue readyQueue;
static DelayQueue delayQueue;
static TaskBlock taskBlock;
static int idleTaskStack[IDLETASKSTACKSIZE];
static int kernelState = K_BLOCKED;

//Error Codes
#define NEW_TASK_FAILED 1
#define READY_QUEUE_EMPTY 2

char* TCBFullError = "Task Block Full\n";

void YKEnterMutex(void) {

	//disable interrupts
	asm("cli");

}

void YKExitMutex(void) {

	//Conditionally enable interrupts
	asm("sti");

}

void YKEnterISR(void) {

	ISRCallDepth++;

}

void YKExitISR(void) {

	ISRCallDepth--;
	if (ISRCallDepth == 0) YKScheduler();

}

void YKInitialize(void) {

	YKEnterMutex();

	//Set up Task Block
	taskBlock.nextFreeTCB == 0;
	//
		
	//Set up Idle Task
	YKNewTask(YKIdleTask, &idleTaskStack[IDLETASKSTACKSIZE], 100);
	//

	YKExitMutex();
	return;

}

void YKIdleTask(void) {

	while (1) {
		int i;
		i = 1;
		YKIdleCount++;
	}

}

void YKNewTask(void (*task)(void), void* taskStack, unsigned char priority) {

	TCB* newTask;	
	
	//Obtain a TCB
	newTask = getNewTCB();
	if (newTask == null) exit(NEW_TASK_FAILED);

	//Fill TCB
	newTask->tid = 0;
	newTask->priority = priority;
	newTask->stackPointer = taskStack;
	newTask->state = READY;
	newTask->delayCount = 0;
	newTask->next = null;
	newTask->prev = null;

	//Set up Stack
	asm("mov bx, [bp+6]"); //Get address of stack
	asm("mov cx, [bp+4]"); //Get address of function pointer
	asm("mov [bx-20], 0x0200"); //Move flag register onto the stack
	asm("mov [bx-22], 0x0");	
	asm("mov [bx-24], cx");

	//Insert into ready queue
	insertSorted(&readyQueue, newTask);
	scheduler();
	return; 

}

TCB* getNewTCB(void) {
	
	TCB* task;
	if (taskBlock.nextFreeTCB < MAX_TASKS + 1) {
                task = &taskBlock.tasks[taskBlock.nextFreeTCB];
		taskBlock.nextFreeTCB++;
		return task;
	} else {
		printf("TCBFullError\n");		
		return null;
	}

}

void scheduler(void) {

	YKEnterMutex();
	TCB* readyTask = dequeue(readyQueue);
	if (kernelState == K_BLOCKED) return;
	if (readyTask == null) exit (READY_QUEUE_EMPTY);
	if (readyTask != currentTask) {
		YKCtxSwCount++;
		currentTask->state = READY;
		readyTask->state = RUNNING;
		dispatcher(readyTask);
		YKExitMutex();
		return;
	}
	YKExitMutex();
	return;
}

void YKRun(void) {

	YKEnterMutex();
	kernelState = K_RUNNING;
	scheduler();
	YKExitMutex();
	return;

}

void YKDelayTask(unsigned int count) {

	if (count == 0) return;

	currentTask->state = BLOCKED;
	currentTask->delayCount = count;
	insert(delayQueue, currentTask);
	asm("int 0x20");
	return;

}

void YKTickHandler(void) {

	tickClock();

}


