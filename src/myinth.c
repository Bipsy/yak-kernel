#include "../include/clib.h"
#include "../include/yakk.h"
#include "../include/DelayQueue.h"

extern int KeyBuffer;
extern unsigned int YKTickCounter;
extern YKSEM* NSemPtr;

void resetHandler() {
	exit(0);
}

void tickHandler() {

	YKEnterMutex();
	YKTickCounter++;
	YKExitMutex();
	printNewLine();
	printString("Tick ");
	YKEnterMutex();
	printInt(YKTickCounter);
	YKExitMutex();
	printNewLine();
	tickClock();
	return;

}

void keyboardHandler() {

	int i;	

	if (KeyBuffer == 'd') {
		printString("\nDELAY KEY PRESSED\n");
		for (i = 0; i < 5000; i++);
		printString("\nDELAY COMPLETE\n");		
	} else if (KeyBuffer == 'p') {
		YKSemPost(NSemPtr);
	} else {
		printString("\nKEYPRESS (");
		printChar((char) KeyBuffer);
		printString(") IGNORED\n");
	}

	return;
}
