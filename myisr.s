RESET:

		push 	ax
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp

		call	YKEnterISR
		mov		ax, sp
		mov     bx, ss
		call	saveStackPointer
		
		sti						;enabling interrupts
		call 	resetHandler	;calling C interrupt handler
		cli						;disabling interrupts
		
		mov		al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

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


		call	YKEnterISR

		sti						;enabling interrupts
		call	tickHandler		;calling C interrupt handler
		cli						;disabling interrupts

		mov al, 0x20			;Load nonspecific EOI value (0x20) into register al
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
		push 	bx
		push 	cx
		push 	dx
		push 	di
		push 	si
		push 	ds
		push 	es
		push 	bp	
		
		call	YKEnterISR
		
		sti						;enabling interrupts
		call	keyboardHandler ;calling C interrupt handler
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

		call 	YKEnterISR

		mov 	al, 0x20
		out		0x20, al

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

		iret
