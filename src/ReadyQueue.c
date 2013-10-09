#include "yakk.h"
#include "ReadyQueue.h"

void initializeReadyQueue() {
	readyQueue.size = 0;
}

void insertReadyQueue(TCB* tcb) {

	if(tcb == null) return;

	//list is empty
	if (readyQueue.size == 0) {
		readyQueue.head = tcb;
		readyQueue.tail = tcb;
		tcb->next = null;
		tcb->prev = null;
		readyQueue.size = 1;
		return;
	}

	//size of the list = 1
	if (readyQueue.head == readyQueue.tail) {		
		if (readyQueue.head->priority < tcb->priority) {
			readyQueue.head->next = tcb;
			tcb->prev = readyQueue.head;
			tcb->next = null;
			readyQueue.tail = tcb;
			readyQueue.size++;
			return;
		} else {
			tcb->next = readyQueue.head;
			tcb->prev = null;
			readyQueue.tail->prev = tcb;
		}
	}

	//size of the readyQueue > 1
	TCB* temp = readyQueue.head;
	while (temp != null) {
		if (temp->priority > tcb->priority) {
			tcb->next = temp;
			tcb->prev = temp->prev;
			temp->prev = tcb;
			if (temp == readyQueue.head) readyQueue.head = tcb;
			readyQueue.size++;
			return;
		}
		temp = temp->next;
	}	

	//tcb has lowest priority
	readyQueue.tail->next = tcb;
	tcb->prev = readyQueue.tail;
	readyQueue.tail = tcb;
	tcb->next = null;
	readyQueue.size++;
	return;

}

TCB* removeReadyQueue() {

	TCB* retValue;
        
	//size of the list = 1
	if (readyQueue.head == readyQueue.tail) {
		retValue = readyQueue.head;
		readyQueue.size--;
		readyQueue.head = null;
		readyQueue.tail = null;
		return retValue;
	}

	//size of the list > 1
	retValue = readyQueue.head;
	readyQueue.head = readyQueue.head->next;
	readyQueue.size--;
	return retValue;	

}
