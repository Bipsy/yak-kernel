#include "../include/yakk.h"
#include "../include/PriorityQueue.h"
#include "../include/clib.h"

void initializePriorityQueue(PriorityQueue* queue) {
	queue->size = 0;
	queue->head = null;
	queue->tail = null;
}

void insertPriorityQueue(PriorityQueue* queue, TCB* tcb) {

	TCB* temp;

	if(tcb == null) return;

	//list is empty
	if (queue->size == 0) {
		queue->head = tcb;
		queue->tail = tcb;
		tcb->next = null;
		tcb->prev = null;
		queue->size = 1;
		return;
	}

	//size of the list = 1
	if (queue->size == 1) {		
		if (queue->head->priority < tcb->priority) {
			queue->head->next = tcb;
			tcb->prev = queue->head;
			tcb->next = null;
			queue->tail = tcb;
			queue->size++;
			return;
		} else {
			tcb->next = queue->head;
			tcb->prev = null;
			queue->tail->prev = tcb;
			queue->head = tcb;
			queue->size++;
			return;
		}
	}

	//size of the priorityQueue > 1
	 
	temp = queue->head;
	while (temp != null) {
		if (temp->priority > tcb->priority) {
			tcb->next = temp;
			tcb->prev = temp->prev;
			if (temp == queue->head) {
				queue->head = tcb;
			} else {
				temp->prev->next = tcb;
			}
			temp->prev = tcb;
			queue->size++;
			return;
		}
		temp = temp->next;
	}	

	//tcb has lowest priority
	queue->tail->next = tcb;
	tcb->prev = queue->tail;
	queue->tail = tcb;
	tcb->next = null;
	queue->size++;
	return;

}

TCB* peekPriorityQueue(PriorityQueue* queue) {	

	return queue->head;
		
}

TCB* removePriorityQueue(PriorityQueue* queue) {

	TCB* retValue;

	//Size of the list = 0
	if (queue->size == 0) {
		return null;
	}
        
	//size of the list = 1
	if (queue->size == 1) {
		//printInt(0);
		//printNewLine();
		retValue = queue->head;
		retValue->next = null;
		retValue->prev = null;
		queue->size--;
		queue->head = null;
		queue->tail = null;
		return retValue;
	}

	//size of the list > 1
	//printString("Removing from priorityQueue\n");
	//printNewLine();
	retValue = queue->head;
	queue->head = queue->head->next;
	queue->size--;
	retValue->next = null;
	retValue->prev = null;
	return retValue;	

}

void printPriorityQueue(PriorityQueue* queue) {
	TCB* current;

	//current = queue->head;
	//printString("Printing Ready Queue with size ");
	//printInt(queue->size);
	//printNewLine();
	
	while (current != null) {
		printInt(current->priority);
		printNewLine();
		current = current->next;
	}
}
