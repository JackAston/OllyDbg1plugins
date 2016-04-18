
GetAddressOfVAForCurrentApiInIAT  PROTO :DWORD, :DWORD, :DWORD

SEH struct
   PrevLink       dd  ?   ; the address of the previous seh structure
   CurrentHandler dd  ?   ; the address of the new exception handler
   SafeOffset     dd  ?   ; the offset where it's safe to continue execution
   PrevEsp        dd  ?   ; the old value in esp
   PrevEbp        dd  ?   ; the old value in ebp
SEH ends

.CODE

ALIGN DWORD
GetAddressOfVAForCurrentApiInIAT proc uses edi BaseOfImage:DWORD, lpszNameOfDll:DWORD, VAOfApi:DWORD
LOCAL ValidPE:BOOL
LOCAL seh:SEH
   and ValidPE, FALSE   ; обнуляем локальную переменную ValidPE
   assume fs:nothing
   push fs:[0]
   pop seh.PrevLink
   mov seh.CurrentHandler, OFFSET SEHHandler
   mov seh.SafeOffset, OFFSET @FinalExit
   lea eax, seh
   mov fs:[0], eax
   assume fs:error
   mov seh.PrevEsp, esp
   mov seh.PrevEbp, ebp
   mov edi, BaseOfImage
   assume edi:ptr IMAGE_DOS_HEADER
   .IF [edi].e_magic == IMAGE_DOS_SIGNATURE
       add edi, [edi].e_lfanew
       assume edi:ptr IMAGE_NT_HEADERS
       .IF [edi].Signature == IMAGE_NT_SIGNATURE
           inc ValidPE            ; mov ValidPE, TRUE
       .ENDIF
   .ENDIF
@FinalExit:
   push seh.PrevLink
   assume fs:nothing
   pop fs:[0]
   assume fs:error
   cmp ValidPE, FALSE
   je @not_found
   assume edi:ptr IMAGE_NT_HEADERS
   mov edi, [edi].OptionalHeader.DataDirectory[sizeof IMAGE_DATA_DIRECTORY].VirtualAddress
   add edi, BaseOfImage
   assume edi:ptr IMAGE_IMPORT_DESCRIPTOR
ALIGN DWORD
@loop1:
   mov eax, [edi].Name1
   test eax, eax
   jz @not_found
   add eax, BaseOfImage
   invoke lstrcmpi, lpszNameOfDll, eax
   test eax, eax
   jz @F
   add edi, sizeof IMAGE_IMPORT_DESCRIPTOR
   jmp @loop1
@@:
   mov edi, [edi].FirstThunk
   assume edi:nothing
   add edi, BaseOfImage
ALIGN DWORD
@loop2:
   mov eax, DWORD PTR [edi]
   test eax, eax
   jz @not_found
   cmp VAOfApi, eax
   je @found
   add edi, 004h
   jmp @loop2
@not_found:
   xor edi, edi
@found:
   mov eax, edi
   ret
GetAddressOfVAForCurrentApiInIAT endp

ALIGN DWORD
SEHHandler proc C uses edx pExcept:DWORD, pFrame:DWORD, pContext:DWORD, pDispatch:DWORD
   mov edx, pFrame
   assume edx:ptr SEH
   mov eax, pContext
   assume eax:ptr CONTEXT
   push [edx].SafeOffset
   pop [eax].regEip
   push [edx].PrevEsp
   pop [eax].regEsp
   push [edx].PrevEbp
   pop [eax].regEbp
   mov eax, ExceptionContinueExecution
   ret
SEHHandler endp
