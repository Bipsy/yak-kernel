#include "../include/clib.h"
#include "../include/yakk.h"
#include "../include/yaku.h"

extern unsigned int YKTickCounter;
extern unsigned int NewPieceType;
extern unsigned int NewPieceOrientation;
extern unsigned int NewPieceID;
extern unsigned int NewPieceColumn;
extern YKQ* PiecesQ;
extern Piece* PiecesArray;


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

void newPieceHandler(void) {
	static int nextPiece;

	if (nextPiece >= MAX_PIECES) {
		nextPiece = 0;
	}

	PiecesArray[nextPiece].type = NewPieceType;
	PiecesArray[nextPiece].orientation = NewPieceOrientation;
	PiecesArray[nextPiece].id = NewPieceID;
	PiecesArray[nextPiece].column = NewPieceColumn;

	YKQPost(PiecesQ, (void*) &PiecesArray[nextPiece]);
	nextPiece++;
	
}
