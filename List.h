#include "yakk.h"

typedef struct List List;

struct List {
	TCB* head;
	TCB* tail;
	unsigned int size;
};

void initializeList(List* list) {
	list->size = 0;
}

void insertSorted(List* list, TCB* tcb) {

	if(tcb == null || list == null) return;

	//list is empty
	if (list->size == 0) {
		list->head = tcb;
		list->tail = tcb;
		tcb->next = null;
		tcb->prev = null;
		list->size = 1;
		return;
	}

	//size of the list = 1
	if (list->head == list->tail) {
		if (tcb == list->head) return;		
		if (head->priority > tcb->priority) {
			list->head->next = tcb;
			tcb->prev = head;
			tcb->next = null;
			list->tail = tcb;
			list->size++;
			return;
		} else {
			tbc->next = list->head;
			tbc->prev = null;
			list->tail->prev = tcb;
		}
	}

	//size of the list > 1
	TCB* temp = list->head;
	while (temp != null) {
		if (temp->priority < tcb->priority) {
			tcb->next = temp;
			tcb->prev = temp->prev;
			temp->prev = tcb;
			if (temp == list->head) list->head = tcb;
			list->size++;
			return;
		}
		temp = temp->next;
	}	

	//tcb has lowest priority
	list->tail->next = tcb;
	tcb->prev = list->tail;
	list->tail = tcb;
	tcb->next = null;
	list->size++;
	return;

}

void enqueue(List* list, TCB* tcb) {

	//list hasn't been initialized
	if (tcb == null || list == null) return;

	//list is empty
	if (list->size == 0) {
		list->head = tcb;
		list->tail = tcb;
		tcb->next = null;
		tcb->prev = null;
		list->size = 1;
		return;
	}

	//size of the list = 1
	if (list->head == list->tail) {
		if (tcb == list->head) return;
		list->head->next = tcb;
		tcb->prev = head;
		tcb->next = null;
		list->tail = tcb;
		list->size++;
		return;
	}

	//list > 1
	list->tail->next = tcb;
	tcb->prev = list->tail;
	list->tail = tcb;
	tcb->next = null;
	list->size++;
	return;

}

TCB* dequeue(List* list) {

	if (list == null) return null;

	//size of the list = 1
	if (list->head == list->tail) {
		TCB* retValue = list->head;
		
	}	

}
