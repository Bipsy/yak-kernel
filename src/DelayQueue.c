#include "../include/DelayQueue.h"
#include "../include/ReadyQueue.h"
#include "../include/yakk.h"
#include "../include/clib.h"

extern DelayQueue delayQueue;

void initializeDelayQueue() {
	delayQueue.size = 0;
	delayQueue.head = null;
}

void tickClock() {

	TCB* current;
    	TCB* temp;
	
	//Size == 0
	if (delayQueue.size == 0) return;
	//printDelayQueue();
	
	//Potential bug...if two tasks have same delay count	
	current = delayQueue.head;
	if (delayQueue.size == 1) {
		if (current->delayCount == 0) {
			delayQueue.head = null;
			delayQueue.size = 0;
			current->next = null;
			current->prev = null;
			current->state = T_READY;
			insertReadyQueue(current);
			return;
		} else {
			current->delayCount--;
			return;
		}
	}

	//Size > 1
	if (current->delayCount == 0) {
		delayQueue.head = current->next;
		delayQueue.size--;
		current->next = null;
		current->prev = null;
		current->state = T_READY;
		insertReadyQueue(current);
		return;
	} else {
		current->delayCount--;
	}

}

void insertDelayQueue(TCB* tcb) {
    
	TCB* current;
	unsigned int sumCount;
      
	if (tcb == null) return;
    
	//Size = 0
     if (delayQueue.size == 0) {
         delayQueue.head = tcb;
         tcb->next = null;
         tcb->prev = null;
         delayQueue.size++;
         return;
     }
    
     //Size > 0
     current = delayQueue.head;
     sumCount = 0;
     while (current != null) {
         sumCount += current->delayCount;
		if (tcb->delayCount < sumCount) {
			tcb->next = current;
			tcb->prev = current->prev;
     		if (current == delayQueue.head) {
                    delayQueue.head = tcb;
            	} else {
				current->prev->next = tcb;
			}
            	current->prev = tcb;
            	delayQueue.size++;
            	tcb->delayCount = tcb->delayCount - sumCount;
            	return;
        	}
        	if (current->next == null) {
        	    current->next = tcb;
        	    tcb->prev = current;
        	    tcb->next = null;
        	    delayQueue.size++;
        	    tcb->delayCount = tcb->delayCount - sumCount;
        	    return;
        	}
        	current = current->next;
    	}
 
}

void printDelayQueue(void) {

	TCB* current;
	int i;

	current = delayQueue.head;
	printString("Printing Delay Queue with size ");
	printInt(delayQueue.size);
	printNewLine();
	
	while (current != null && i < 10) {
		printInt(current->priority);
		printNewLine();
		current = current->next;
		i++;
	}

}

