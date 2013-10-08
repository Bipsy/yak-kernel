#include "yakk.h"
//#include "clib.h"

int main(int argc, char** argv) {

	int i;
	TCB* tcb;
	for (i = 0; i < 10; i++ ) {
		YKIdleTask();
	}
	
	return 0;

}
