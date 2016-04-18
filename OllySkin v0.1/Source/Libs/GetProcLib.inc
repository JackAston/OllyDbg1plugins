	;------------------------------------------------------------------------------
	; Name: GetProcArray
	; Desc: Retrieves multiple addresses from a module.
	; Parameters:
	;  1 - Handle to the DLL module that contains the functions. Cannot be NULL.
	;  2 - Pointer the string table, that will receive the addresses.
	;       Cannot be NULL.
	;------------------------------------------------------------------------------
	GetProcArray proto stdcall :DWORD, :DWORD

	;------------------------------------------------------------------------------
	; Name: GetProcOffset
	; Desc: Retrieves procedure address from an module.
	; Parameters:
	;  1 - Handle to the DLL module that contains the function. Cannot be NULL.
	;  2 - Pointer to a null-terminated string containing the function name.
	;       Cannot be NULL.
	;------------------------------------------------------------------------------
	GetProcOffset proto stdcall :DWORD, :DWORD

	;------------------------------------------------------------------------------
	; Name: GetProcOffsetEx
	; Desc: Retrieves procedure address from an module. Accepts hint.
	; Parameters:
	;  1 - Handle to the DLL module that contains the function. Cannot be NULL.
	;  2 - Pointer to a null-terminated string containing the function name.
	;       Cannot be NULL.
	;  3 - Hint value of the specific procedure.
	;------------------------------------------------------------------------------
	GetProcOffsetEx proto stdcall :DWORD, :DWORD, :DWORD