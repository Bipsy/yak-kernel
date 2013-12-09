#include "../include/clib.h"
#include "../include/yakk.h"                   /* contains kernel definitions */
#include "../include/yaku.h"
#include "../include/simptris.h"

Piece PiecesArray[MAX_PIECES];  /* buffers for message content */
Move MovesArray[MAX_MOVES];

int MovesTaskStk[TASK_STACK_SIZE];      /* a stack for each task */
int CommTaskStk[TASK_STACK_SIZE];
int STaskStk[TASK_STACK_SIZE];

extern unsigned int YKCtxSwCount;
extern unsigned int YKIdleCount;
extern unsigned int YKTickCounter;
extern unsigned int NewPieceType;
extern unsigned int NewPieceOrientation;
extern unsigned int NewPieceID;
extern unsigned int NewPieceColumn;

void* PiecesQ[MSGQSIZE];           /* space for message queue */
YKQ* PiecesQPtr;                   /* actual name of queue */
void* CommQ[MSGQSIZE];
YKQ* CommQPtr;
YKSEM* CommSem;


/*	This task should pull pieces off the pieces queue, make the decisions about 
	what moves need to be performed, and place moves onto the move queue for the
	comm task to send to the simptris player.
*/
void MovesTask(void) {
	
	Piece* newPiece;
	static unsigned int nextMove;
	static unsigned int curved;
	static unsigned int straightCount;
	unsigned int column;
	unsigned int i;

	while(1) {

		newPiece = (Piece*) YKQPend(PiecesQPtr);

		switch (newPiece->id) {
			case 0: 	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;

			case 1:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 2:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 3:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 4:		break;
			case 5:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 6:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 7:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 8:		MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = COUNTERCLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 9:		break;
			case 10:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 11:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 12:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 13:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 14:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 15:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 16:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 17:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 18:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 19:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 20:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 21:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 22:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 23:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 24:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 25:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 26: 	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 27:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 28:	break;
			case 29:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = COUNTERCLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 30:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = COUNTERCLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 31:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 32:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 33:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = COUNTERCLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 34:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 35:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 36:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 37:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 38:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 3;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 39:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 40:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 41:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 42:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 43:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 44:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 45:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 46:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 47:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 4;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = CLOCKWISE;
						MovesArray[nextMove].function = ROTATE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 48:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = RIGHT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 2;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			case 49:	MovesArray[nextMove].id = newPiece->id;
						MovesArray[nextMove].direction = LEFT;
						MovesArray[nextMove].function = SLIDE;
						MovesArray[nextMove].times = 1;
						YKQPost(CommQPtr, &MovesArray[nextMove]);
						if (nextMove+1 < MSGQSIZE) {
							nextMove++;
						} else {
							nextMove = 0;
						}
						break;
			default: 	break;
		}


	}

}

/*	This task pulls moves off the move queue and send commands to the simptris 
	player.
*/
void CommTask(void) {
	
	Move* newMove;	
	unsigned int i;

	while(1) {

		newMove = (Move*) YKQPend(CommQPtr);
		for (i = 0; i < newMove->times; i++) {
			YKSemPend(CommSem);
			if (newMove->function == SLIDE) {
				SlidePiece(newMove->id, newMove->direction);
			} else {
				RotatePiece(newMove->id, newMove->direction);
			}
		}

	}
}

void STask(void) {
    unsigned max, switchCount, idleCount;
    int tmp;

    YKDelayTask(1);
    printString("Welcome to the YAK kernel\r\n");
    printString("Determining CPU capacity\r\n");
    YKDelayTask(1);
    YKIdleCount = 0;
    YKDelayTask(5);
    max = YKIdleCount / 25;
    YKIdleCount = 0;

    YKNewTask(MovesTask, (void *) &MovesTaskStk[TASK_STACK_SIZE], 2);
    YKNewTask(CommTask, (void *) &CommTaskStk[TASK_STACK_SIZE], 1);
	StartSimptris();
    
    while (1)
    {
        YKDelayTask(20);
        
        YKEnterMutex();
        switchCount = YKCtxSwCount;
        idleCount = YKIdleCount;
        YKExitMutex();
        
        printString("<CS: ");
        printInt((int)switchCount);
        printString(", CPU: ");
        tmp = (int) (idleCount/max);
        printInt(100-tmp);
        printString(">\r\n");
        
        YKEnterMutex();
        YKCtxSwCount = 0;
        YKIdleCount = 0;
        YKExitMutex();
    }
}

void main(void) {
    YKInitialize();
    
    /* create queue, at least one user task, etc. */
    PiecesQPtr = YKQCreate(PiecesQ, MSGQSIZE);
	CommQPtr = YKQCreate(CommQ, MSGQSIZE);
	CommSem = YKSemCreate(1);
    YKNewTask(STask, (void *) &STaskStk[TASK_STACK_SIZE], 3);
    
	SeedSimptris(1);

    YKRun();
} 
