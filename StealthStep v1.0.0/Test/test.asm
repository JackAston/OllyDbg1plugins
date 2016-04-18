include c:\masm32\include\masm32rt.inc
.data? 
align dword

.data 

	Welcome			db			"Step through the code by holding F8",0
	Detected	    db			"You got detected!",0
	NotDetected 	db			"No stepping was detected, congrats!",0

.code

main:
	invoke MessageBox, 0, addr Welcome, 0, MB_OK

@Minefield:
	push ss
	pop ss
	pushfd
	pop eax
	and eax, 0100h
	jnz @Detected
	pushfd
	pop eax
	and eax, 0100h
	jnz @Detected
	jmp @OK

@Detected:
	invoke MessageBox, 0 , addr Detected, 0, MB_OK
	jmp @Exit

@OK:
	invoke MessageBox, 0, addr NotDetected, 0, MB_OK

@Exit:
	invoke ExitProcess,NULL
	
end main