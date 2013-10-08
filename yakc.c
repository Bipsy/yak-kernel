#include "clib.h"

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

	//

	

	YKExitMutex();

}


