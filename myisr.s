RESET:
		push 	ax
		push 	bx
		push 	cx
		push	dx
		push 	si
		push 	di
		push	bp
		push	es
		push	ds

		call	YKEnterISR
		mov		ax, sp
		mov     bx, ss
		call	saveStackPointer
		
		sti						;enabling interrupts
		call 	resetHandler	;calling C interrupt handler
		cli						;disabling interrupts
		
		mov		al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		pop		ds
		pop		es
		pop		bp
		pop		di
		pop		si
		pop		dx
		pop		cx
		pop		bx
		pop		ax		

		iret					;returning from ISR

TICK:
		push 	ax
		push 	bx
		push 	cx
		push	dx
		push 	si
		push 	di
		push	bp
		push	es
		push	ds
		call	YKEnterISR

		sti						;enabling interrupts
		call	tickHandler		;calling C interrupt handler
		cli						;disabling interrupts

		mov al, 0x20			;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		pop		ds
		pop		es
		pop		bp
		pop		di
		pop		si
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR

KEYBOARD:

		push 	ax
		push 	bx
		push 	cx
		push	dx
		push 	si
		push 	di
		push	bp
		push	es
		push	ds
		
		call	YKEnterISR
		
		sti						;enabling interrupts
		call	keyboardHandler ;calling C interrupt handler
		cli						;disabling interrupts

		mov     al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		pop		ds
		pop		es
		pop		bp
		pop		di
		pop		si
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR	

