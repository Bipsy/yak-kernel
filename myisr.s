RESET:
		push 	ax			;saving context of interrupted task
		push		bx
		push 	cx
		push 	dx
		push 	si
		push		di
		push 	bp
		push		es
		push		ds
	
		sti					;enabling interrupts
		call 	resetHandler	;calling C interrupt handler
		cli					;disabling interrupts
		
		mov		al, 0x20		;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		pop		ds			;restoring context of interrupted task
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
		push 	ax			;saving context of interrupted task
		push		bx
		push 	cx
		push 	dx
		push 	si
		push		di
		push 	bp
		push		es
		push		ds

		sti					;enabling interrupts
		call		tickHandler	;calling C interrupt handler
		cli					;disabling interrupts

		mov al, 0x20			;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		pop		ds			;restoring context of interrupted task
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
		push 	ax			;saving context of interrupted task
		push		bx
		push 	cx
		push 	dx
		push 	si
		push		di
		push 	bp
		push		es
		push		ds

		sti					;enabling interrupts
		call		keyboardHandler ;calling C interrupt handler
		cli					;disabling interrupts

		mov al, 0x20			;Load nonspecific EOI value (0x20) into register al
		out		0x20, al		;Write EOI to PIC (port 0x20)

		pop		ds			;restoring context of interrupted task
		pop		es
		pop		bp
		pop		di
		pop		si
		pop		dx
		pop		cx
		pop		bx
		pop		ax

		iret					;returning from ISR
		
		
		
