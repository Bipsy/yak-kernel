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
unsigned int futureBitMap0;
unsigned int futureBitMap1;
unsigned int futureBitMap2;
unsigned int futureBitMap3;
unsigned int futureBitMap4;
unsigned int futureBitMap5;

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
	unsigned int column;
	int value, direction, times;

	while(1) {

		newPiece = (Piece*) YKQPend(PiecesQPtr);

		column = newPiece->column;

		if (column == 5) {
			nextMove = handleMove(newPiece->id, LEFT, SLIDE, 1, nextMove);
			column--;
		}

		if (column == 0) {
			nextMove = handleMove(newPiece->id, RIGHT, SLIDE, 1, nextMove);
			column++;
		}

		row = 0x8000;

		if (newPiece->type == STRAIGHT) {
			/*
			if ( ((ScreenBitMap3 & row) == 0) &&
			     ((ScreenBitMap2 & row) == 0) &&
				 ((ScreenBitMap1 & row) == 0)) {
				if (newPiece->orientation == VERTICAL) {
					nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
				}
				direction = column < 2 ? RIGHT : LEFT;
				times = column < 2 ? 2-column : column-2;
				nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
			} else {
				if (newPiece->orientation == HORIZONTAL) {
					nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
				}

				if (ScreenBitMap1 < ScreenBitMap0) {
					direction = column < 1 ? RIGHT : LEFT;
					times = column < 1 ? 1 : column-1;
					nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
				} else {
					nextMove = handleMove(newPiece->id, LEFT, SLIDE, column, nextMove);
				}
			}*/

			while (row) {
				if ((futureBitMap0 & row) == 0) {
					if (((futureBitMap1 & row) == 0) && ((futureBitMap2 & row) == 0)) {
						if (newPiece->orientation == VERTICAL) {
							nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
						}
						nextMove = handleMove(newPiece->id, LEFT, SLIDE, column-1, nextMove);
						futureBitMap0 |= row;
						futureBitMap1 |= row;
						futureBitMap2 |= row;
					} else {
						if (newPiece->orientation == HORIZONTAL) {
							nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
						}
						nextMove = handleMove(newPiece->id, LEFT, SLIDE, column, nextMove);
						futureBitMap0 |= row | (row >> 1) | (row >> 2);
					}
					break;
				} 
				if ((futureBitMap3 & row) == 0) {
					if ( ((futureBitMap4 & row) == 0) && ((futureBitMap5 & row) == 0)) {
						if (newPiece->orientation == VERTICAL) {
							nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
						}
						nextMove = handleMove(newPiece->id, RIGHT, SLIDE, 4-column, nextMove);
						futureBitMap3 |= row;
						futureBitMap4 |= row;
						futureBitMap5 |= row;
					} else {
						if (newPiece->orientation == HORIZONTAL) {
							nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 1, nextMove);
						}
						direction = column < 3 ? RIGHT : LEFT;
						times = column < 3 ? 3-column : column-3;
						nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
						futureBitMap1 |= row | (row >> 1) | (row >> 2);
					}
					break;
				}
				if ((futureBitMap1 & row) == 0) {
					nextMove = handleMove(newPiece->id, LEFT, SLIDE, column-1, nextMove);
					futureBitMap1 |= row | (row >> 1) | (row >> 2);
					break;
				}

				row = row >> 1;
			}

		} else {

			while (row) {
				if ((futureBitMap5 & row) == 0) {
					if (1-((int)newPiece->orientation) > 0) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 1, nextMove);
					} else if (1-((int)newPiece->orientation) < 0) {
						nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 0-(1-newPiece->orientation), nextMove);
					}
					nextMove = handleMove(newPiece->id, RIGHT, SLIDE, 5-column, nextMove);
					break;
				}

				if ((futureBitMap4 & row) == 0) {
					if (newPiece->orientation != 3) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 3-newPiece->orientation, nextMove);
					}
					direction = (column < 4) ? RIGHT : LEFT;
					times = (column < 4) ? 4-column : column-4;
					nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
					break;
				}
				
				if (((futureBitMap3 & row) == 0) && ((futureBitMap2 & row) == 0)) {
					if (newPiece->orientation == 0) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 1, nextMove);
					} else if (newPiece->orientation != 1) {
						nextMove = handleMove(newPiece->id, CLOCKWISE, ROTATE, 0-(1-newPiece->orientation), nextMove);
					}
					direction = (column < 3) ? RIGHT : LEFT;
					times = (column < 3) ? 3-column : column-3;
					nextMove = handleMove(newPiece->id, direction, SLIDE, times, nextMove);
					break;
				}

				if ((futureBitMap2 & row) == 0) {
					if (newPiece->orientation != 3) {
						nextMove = handleMove(newPiece->id, COUNTERCLOCKWISE, ROTATE, 3-newPiece->orientation, nextMove);
					}
					direction = (column < 2) ? RIGHT : LEFT;
					times = (column < 2) ? 2-column : column-2;
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
    
	SeedSimptris(1);

    YKRun();
} 
