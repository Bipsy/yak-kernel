lab8.bin:	lab8.s
			nasm lab8.s -o lab8.bin -l lab8.lst

lab8.s:		clib.s myinth.s yakc.s PriorityQueue.s DelayQueue.s Semaphore.s MessageQueue.s Event.s lab8app.s
			cat clib.s simptris.s myisr.s myinth.s yaks.s yakc.s PriorityQueue.s DelayQueue.s Semaphore.s MessageQueue.s Event.s lab8app.s > lab8.s

myinth.s:		myinth.c
			cpp myinth.c myinth.i
			c86 -g myinth.i myinth.s

PriorityQueue.s:   PriorityQueue.c
				cpp PriorityQueue.c PriorityQueue.i
				c86 -g PriorityQueue.i PriorityQueue.s

DelayQueue.s:   DelayQueue.c
				cpp DelayQueue.c DelayQueue.i
				c86 -g DelayQueue.i DelayQueue.s

Semaphore.s:	Semaphore.c
			cpp Semaphore.c Semaphore.i
			c86 -g Semaphore.i Semaphore.s

MessageQueue.s:	MessageQueue.c
			cpp MessageQueue.c MessageQueue.i
			c86 -g MessageQueue.i MessageQueue.s

Event.s:   Event.c
				cpp Event.c Event.i
				c86 -g Event.i Event.s

lab8app.s:	lab8app.c
			cpp lab8app.c lab8app.i
			c86 -g lab8app.i lab8app.s

yakc.s:		yakc.c
			cpp yakc.c yakc.i
			c86 -g yakc.i yakc.s

clean:		
			rm lab8.bin lab8.lst lab8.s myinth.s myinth.i DelayQueue.s DelayQueue.i yakc.s yakc.i PriorityQueue.i PriorityQueue.s Semaphore.i Semaphore.s MessageQueue.i Event.i MessageQueue.s Event.s lab8app.i lab8app.s *~
