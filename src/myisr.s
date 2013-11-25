RESET:

		call 	resetHandler	;calling C interrupt handler


TICK:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp
		call		YKGetISRCallDepth
		test		ax, ax
		jnz		tick_1
		mov		si, [currentTask]
		add		si, word 0x4
		mov 		[si], sp

tick_1:
		call	YKEnterISR

		sti					;enabling interrupts
		call	tickHandler		;calling C interrupt handler
		cli					;disabling interrupts

		mov 		al, 0x20			;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		call	YKExitISR

		pop		bp
		pop		es
		pop		ds
		pop		si
		pop		di
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR

KEYBOARD:

		push 	ax
		mov     al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)
		pop		ax
		iret					;returning from ISR

GAMEOVER:
	
		mov		ax, word 0x0
		push	ax
		call	exit

NEWPIECE:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp
		call	YKGetISRCallDepth
		test	ax, ax
		jnz		newpiece_1
		mov		si, [currentTask]
		add		si, word 0x4
		mov 		[si], sp	

newpiece_1:		
		call	YKEnterISR
		
		sti						;enabling interrupts
		call	newPieceHandler ;calling C interrupt handler
		cli						;disabling interrupts

		mov     al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		call	YKExitISR

		pop		bp
		pop		es
		pop		ds
		pop		si
		pop		di
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR

RECIEVED:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp
		call	YKGetISRCallDepth
		test	ax, ax
		jnz		recieved_1
		mov		si, [currentTask]
		add		si, word 0x4
		mov 		[si], sp	

recieved_1:		
		call	YKEnterISR
		
		sti						;enabling interrupts
		call	receivedHandler ;calling C interrupt handler
		cli						;disabling interrupts

		mov     al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		call	YKExitISR

		pop		bp
		pop		es
		pop		ds
		pop		si
		pop		di
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR

TOUCHDOWN:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp
		call	YKGetISRCallDepth
		test	ax, ax
		jnz		touchdown_1
		mov		si, [currentTask]
		add		si, word 0x4
		mov 		[si], sp	

touchdown_1:		
		call	YKEnterISR
		
		sti						;enabling interrupts
		call	touchdownHandler ;calling C interrupt handler
		cli						;disabling interrupts

		mov     al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		call	YKExitISR

		pop		bp
		pop		es
		pop		ds
		pop		si
		pop		di
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR

CLEAR:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp
		call	YKGetISRCallDepth
		test	ax, ax
		jnz		clear_1
		mov		si, [currentTask]
		add		si, word 0x4
		mov 		[si], sp	

clear_1:		
		call	YKEnterISR
		
		sti						;enabling interrupts
		call	clearHandler	;calling C interrupt handler
		cli						;disabling interrupts

		mov     al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		call	YKExitISR

		pop		bp
		pop		es
		pop		ds
		pop		si
		pop		di
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR 

TRAP:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp
		mov		si, [currentTask]
		add		si, word 0x4
		mov 		[si], sp

		call 	YKScheduler

		pop		bp
		pop		es
		pop		ds
		pop		si
		pop		di
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret

