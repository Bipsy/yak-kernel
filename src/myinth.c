#include "../include/clib.h"
#include "../include/yakk.h"
#include "../include/DelayQueue.h"

extern unsigned int YKTickCounter;
extern char KeyBuffer;
extern unsigned NewPieceID;
extern unsigned NewPieceType;
extern unsigned NewPieceOrientation;
extern unsigned NewPieceColumn;
extern YKQ* PiecesQPtr;
extern YKSEM* CommSem;


void resetHandler(void) {
	exit(0);
}

void tickHandler(void) {

	unsigned int localCounter;

	tickClock();
	YKEnterMutex();
	localCounter = ++YKTickCounter;
	YKExitMutex();

	printString("\nTick: ");
	printInt(localCounter);
	printNewLine();

}

void keyboardHandler(void) {

	return;

}

void gameOverhandler(void) {

	exit(GAME_OVER);

}

void newPieceHandler(void) {
	/* 	This function needs to get the details of the new simptris piece and 
		place a new piece on the piece queue. It obtains the next piece from 
		the piece array using a static counter.
	*/

	static unsigned int nextPiece;

	//Build new piece
	pieceArray[nextPiece].id = NewPieceID;
	pieceArray[nextPiece].type = NewPieceType;
	pieceArray[nextPiece].orientation = NewPieceOrientation;
	pieceArray[nextPiece].column = column;
	if (nextPiece+1 < MSGQSIZE) {	
		nextPiece++;
	} else {
		nextPiece = 0;
	}

	//Place it on piece queue
	YKPost(PiecesQPtr, &pieceArray[nextPiece-1];
	return;	

}

void receivedHandler(void) {
	/*	This function needs to signal to the communication task that the last
		command was received and completed. We will do this using a semaphore. 
	*/

	YKSemPost(CommSem);
	
}

/*	We don't have a use for the handler just yet.
	
*/
void touchdownHandler(void) {
	return;
}

/* We don't have a use for this handler just yet.
*/
void clearHandler(void) {
	return;
}
