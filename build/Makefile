lab4.bin:	lab4.s
			nasm lab4.s -o lab4.bin -l lab4.lst

lab4.s:		clib.s myisr.s myinth.s yaks.s yakc.s List.s
			cat clib.s myisr.s myinth.s yaks.s yakc.s List.s > lab4.s

myinth.s:	myinth.c
			cpp myinth.c myinth.i
			c86 -g myinth.i myinth.s

List.s:   List.c
			cpp List.c List.i
			c86 -g List.i List.s

yakc.s:		yakc.c
			cpp yakc.c yakc.i
			c86 -g yakc.i yakc.s

clean:		
			rm lab4.bin lab4.lst lab4.s myinth.s myinth.i List.s List.i yakc.s yakc.i

