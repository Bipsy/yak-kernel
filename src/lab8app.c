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
	unsigned int column;
	unsigned int i;

	while(1) {
		newPiece = (Piece*) YKQPend(PiecesQPtr);
		printString("Got Piece\n");
		column = newPiece->column;
		//Corner Piece		
		if (newPiece->type == CORNER) {
			//Move piece to column 4

			printString("I am a corner\n");
			//Move left			
			if (column > 4) {
				MovesArray[nextMove].direction = LEFT;
				MovesArray[nextMove].times = 1;
			//Move right
			} else {
				MovesArray[nextMove].direction = RIGHT;
				MovesArray[nextMove].times = 4 - column;
			}
			
			MovesArray[nextMove].id = newPiece->id;
			MovesArray[nextMove].function = SLIDE;
			YKQPost(CommQPtr, &MovesArray[nextMove]);
			if (nextMove+1 < MSGQSIZE) {
				nextMove++;
			} else {
				nextMove = 0;
			}

			//Bottom is curved
			if (curved) {
				printString("Bottom is curved\n");
				switch (newPiece->orientation) {
					// *
					// * *
					//Rotate clockwise one turn
					case 0: MovesArray[nextMove].id = newPiece->id;
							MovesArray[nextMove].direction = CLOCKWISE;
							MovesArray[nextMove].function = ROTATE;
							MovesArray[nextMove].times = 1;
							YKQPost(CommQPtr, &MovesArray[nextMove]);
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;				
							}
					//   *
					// * *
					//Rotate counter-clockwise two turns
					case 1: MovesArray[nextMove].id = newPiece->id;
							MovesArray[nextMove].direction = COUNTERCLOCKWISE;
							MovesArray[nextMove].function = ROTATE;
							MovesArray[nextMove].times = 2;
							YKQPost(CommQPtr, &MovesArray[nextMove]);
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;
							}
							break;
					// * *
					//   *
					//Rotate counter-clockwise one turn
					case 2: MovesArray[nextMove].id = newPiece->id;
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
					// * *
					// *
					//No moves
					case 3:	 break;
					default: break;
				}
			curved = 0;
			//Bottom is flat
			} else {
				printString("Bottom is flat\n");
				switch (newPiece->orientation) {
					// *
					// * *
					//Rotate counter-clockwise one turn
					//Move right one space
					case 0: MovesArray[nextMove].id = newPiece->id;
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
					//   *
					// * *
					//Move right one space					
					case 1: MovesArray[nextMove].id = newPiece->id;
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
					// * *
					//   *
					//Rotate clockwise one turn
					//Move right one space
					case 2: MovesArray[nextMove].id = newPiece->id;
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
							MovesArray[nextMove].times = 1;
							YKQPost(CommQPtr, &MovesArray[nextMove]);
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;
							}
							break;
					// * *
					// *
					//Rotate counter-clockwise two turns
					//Move right one space
					case 3: breakpoint();
							MovesArray[nextMove].id = newPiece->id;
							MovesArray[nextMove].direction = COUNTERCLOCKWISE;
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
							MovesArray[nextMove].times = 1;
							YKQPost(CommQPtr, &MovesArray[nextMove]);
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;
							}
							break;
					default: break;
				}
			curved = 1;
			}

		//Straight Piece
		} else {
			column = newPiece->column;
			printString("I am a line\n");
			printInt(column);
			printNewLine();
			//Move Left			
			if (column > 0) {
				MovesArray[nextMove].direction = LEFT;
				MovesArray[nextMove].times = (column - 1);
			//Move Right
			} else {
				MovesArray[nextMove].direction = RIGHT;
				MovesArray[nextMove].times = 1;
			}
			MovesArray[nextMove].id = newPiece->id;  
			MovesArray[nextMove].function = SLIDE;
			YKQPost(CommQPtr, &MovesArray[nextMove]);
			if (nextMove+1 < MSGQSIZE) {
				nextMove++;
			} else {
				nextMove = 0;				
			}
			
			//Lay the piece flat
			if (newPiece->orientation == 1) {
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
		printString("Got Move\n");
		breakpoint();
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

    YKNewTask(MovesTask, (void *) &MovesTaskStk[TASK_STACK_SIZE], 1);
    YKNewTask(CommTask, (void *) &CommTaskStk[TASK_STACK_SIZE], 2);
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
    
	SeedSimptris(1247);

    YKRun();
} 
