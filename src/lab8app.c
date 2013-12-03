#include "../include/clib.h"
#include "../include/yakk.h"                   /* contains kernel definitions */
#include "../include/yaku.h"

#define TASK_STACK_SIZE   512       /* stack size in words */
#define MSGQSIZE          20
#define SLIDE				0
#define ROTATE				1
#define CORNER				0
#define COUNTER-CLOCKWISE	0
#define CLOCKWISE			1
#define RIGHT				1
#define LEFT				0

Piece PiecesArray[MAX_PIECES];  /* buffers for message content */
Move MovesArray[MAX_MOVES];


int MovesTaskStk[TASK_STACK_SIZE];      /* a stack for each task */
int CommTaskStk[TASK_STACK_SIZE];
int STaskStk[TASK_STACK_SIZE];

extern unsigned int YKCtxSwCount;
extern unsigned int YKIdleCount;COUNTER-CLOCKWISE
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
		newPiece = (Piece*) YKQPend(PiecesQ);
		//Corner Piece		
		if (newPiece->type == CORNER) {
			//Move piece to column 4

			//Move left			
			if (newPiece->column > 4) {
				movesArray[nextMove].direction = LEFT;
				movesArray[nextMove].times = 1;
			//Move right
			} else {
				movesArray[nextMove].direction = RIGHT;
				movesArray[nextMove].times = 4 - column;
			}
			
			movesArray[nextMove].id = newPiece->id;
			movesArray[nextMove].function = SLIDE;
			YKQPost(CommPtr, &movesArray[nextMove];
			if (nextMove+1 < MSGQSIZE) {
				nextMove++;
			} else {
				nextMove = 0;
			}

			//Bottom is curved
			if (curved) {
				switch (newPiece->orientation) {
					// *
					// * *
					//Rotate clockwise one turn
					case 0: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = CLOCKWISE;
							movesArray[nextMove].function = ROTATE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							else {
								nextMove = 0;				
							}
					//   *
					// * *
					//Rotate counter-clockwise two turns
					case 1: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = COUNTER-CLOCKWISE;
							movesArray[nextMove].function = ROTATE;
							movesArray[nextMove].times = 2;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;
							}
							break;
					// * *
					//   *
					//Rotate counter-clockwise one turn
					case 2: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = COUNTER-CLOCKWISE;
							movesArray[nextMove].function = ROTATE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							else {
								nextMove = 0;				
							}
							break;
					// * *
					// *
					//Rotate counter-clockwise two turns
					//Move right one space
					case 4:	 break;
					default: break;
				}
			//Bottom is flat
			} else {
				switch (newPiece->orientation) {
					// *
					// * *
					//Rotate counter-clockwise one turn
					//Move right one space
					case 0: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = COUNTER-CLOCKWISE;
							movesArray[nextMove].function = ROTATE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							else {
								nextMove = 0;				
							}
							movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = RIGHT;
							movesArray[nextMove].function = SLIDE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;
							}
							break;
					//   *
					// * *
					//Move right one space					
					case 1: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = RIGHT;
							movesArray[nextMove].function = SLIDE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
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
					case 2: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = CLOCKWISE;
							movesArray[nextMove].function = ROTATE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							else {
								nextMove = 0;				
							}
							movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = RIGHT;
							movesArray[nextMove].function = SLIDE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
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
					case 4: movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = COUNTER-CLOCKWISE;
							movesArray[nextMove].function = ROTATE;
							movesArray[nextMove].times = 2;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							else {
								nextMove = 0;				
							}
							movesArray[nextMove].id = newPiece->id;
							movesArray[nextMove].direction = RIGHT;
							movesArray[nextMove].function = SLIDE;
							movesArray[nextMove].times = 1;
							YKQPost(CommPtr, &movesArray[nextMove];
							if (nextMove+1 < MSGQSIZE) {
								nextMove++;
							} else {
								nextMove = 0;
							}
							break;
					default: break;
				}
			}

		//Straight Piece
		} else {
			column = newPiece->column;
			//Move Left			
			if (column > 0) {
				movesArray[nextMove].direction = LEFT;
				movesArray[nextMove].times = (column - 1);
				if (nextMove+1 < MSGQSIZE) {
					nextMove++;
				else {
					nextMove = 0;				
				}
			//Move Right
			} else {
				movesArray[nextMove].direction = RIGHT;
				movesArray[nextMove].times = 1;
			}
			movesArray[nextMove].id = newPiece->id;  
			movesArray[nextMove].function = SLIDE;
			YKQPost(CommQPtr, &movesArray[nextMove];
			if (nextMove+1 < MSGQSIZE) {
				nextMove++;
			else {
				nextMove = 0;				
			}
			
			//Lay the piece flat
			if (newPiece->orientation == 1) {
				movesArray[nextMove].id = newPiece->id;
				movesArray[nextMove].direction = 1;
				movesArray[nextMove].function = ROTATE;
				movesArray[nextMove].times = 1;
				YKQPost(CommQPtr, &movesArray[nextMove];

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

    YKNewTask(MovesTask, (void *) &MovesTaskStk[TASK_STACK_SIZE], 3);
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
    YKNewTask(STask, (void *) &STaskStk[TASK_STACK_SIZE], 1);
    
	SeedSimptris(1247);

    YKRun();
} 
