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
extern unsigned int	ScreenBitMap0;
extern unsigned int ScreenBitMap1;
extern unsigned int ScreenBitMap2;
extern unsigned int ScreenBitMap3;
extern unsigned int ScreenBitMap4;
extern unsigned int ScreenBitMap5;

void* PiecesQ[MSGQSIZE];           /* space for message queue */
YKQ* PiecesQPtr;                   /* actual name of queue */
void* CommQ[MSGQSIZE];
YKQ* CommQPtr;
YKSEM* CommSem;

int handleMove(int id, int direction, int function, int times, int nextMove) {
	MovesArray[nextMove].id = id;
	MovesArray[nextMove].direction = direction;
	MovesArray[nextMove].function = function;
	MovesArray[nextMove].times = times;
	YKQPost(CommQPtr, &MovesArray[nextMove]);
	if (nextMove+1 < MSGQSIZE) {
		return nextMove + 1;
	} else {
		return 0;
	}
}

/*	This task should pull pieces off the pieces queue, make the decisions about 
	what moves need to be performed, and place moves onto the move queue for the
	comm task to send to the simptris player.
*/
void MovesTask(void) {
	
	Piece* newPiece;
	static unsigned int nextMove;
	static unsigned int curved;
	static unsigned int straightCount;
	unsigned int row;
	int value, direction, times;

	while(1) {

		newPiece = (Piece*) YKQPend(PiecesQPtr);

		if (newPiece->type == STRAIGHT) {

			if (newPiece->orientation == HORIZONTAL) {
				nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
			}

			if (ScreenBitMap1 < ScreenBitMap0) {
				direction = newPiece->column < 1 ? RIGHT : LEFT;
				times = newPiece->column < 1 ? 1 : newPiece->column-1;
				nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
			} else {
				nextMove = handleMove(newPiece->id, LEFT, SLIDE, newPiece->column, nextMove);
			}

		} else {
			row = 0x8000;
			while (row) {
				if ((ScreenBitMap5 & row) == 0) {
					if (1-((int)newPiece->orientation) > 0) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 1, nextMove);
					} else if (1-((int)newPiece->orientation) < 0) {
						nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 0-(1-newPiece->orientation), nextMove);
					}
					nextMove = handleMove(newPiece->id, RIGHT, SLIDE, 5-newPiece->column, nextMove);
					break;
				}

				if ((ScreenBitMap4 & row) == 0) {
					if (newPiece->orientation != 3) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 3-newPiece->orientation, nextMove);
					}
					direction = (newPiece->column < 4) ? RIGHT : LEFT;
					times = (newPiece->column < 4) ? 4-newPiece->column : newPiece->column-4;
					nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
					break;
				}
				
				if ((ScreenBitMap3 & row) == 0) {
					if (newPiece->orientation == 0) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 1, nextMove);
					} else if (newPiece->orientation != 1) {
						nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 0-(1-newPiece->orientation), nextMove);
					}
					direction = (newPiece->column < 3) ? RIGHT : LEFT;
					times = (newPiece->column < 3) ? 3-newPiece->column : newPiece->column-3;
					nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
					break;
				}

				if ((ScreenBitMap2 & row) == 0) {
					if (newPiece->orientation != 3) {
						if (newPiece->column != 5) {
							nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 3-newPiece->orientation, nextMove);
						} else {
							nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 3-newPiece->orientation, nextMove);
						}
					}
					direction = (newPiece->column < 2) ? RIGHT : LEFT;
					times = (newPiece->column < 2) ? 2-newPiece->column : newPiece->column-2;
					nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
					break;
				}

				row = row >> 1;

			}	
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
    
	SeedSimptris(2);

    YKRun();
} 
