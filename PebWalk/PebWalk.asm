;extern ExitProcess:PROC ;Used for Debugging

.code
main PROC


;======================Start of finding NTDLL======================

FindNTDLL:
push r12
mov  r12, rsp
sub  rsp, 28h
xor rax, rax
mov rax, gs:[60h]    ;Get PEB pointer from TEB (Thread Environment Block)
mov rcx, [rax + 18h] ;Ldr
mov rdx, [rcx + 10h] ;InLoadOrderModuleList first entry

test rcx, rcx ;Test if valid
jz Exit

ModuleLoop:
mov rcx, [rdx + 30h] ;Get base
test rcx, rcx
jz Exit

movsxd rax, dword ptr [rcx + 3Ch] ;Get e_lfanew (PE header offset)
mov r10, rcx                      ;Save module base in r10
xor r9d, r9d
mov r9d, [rax + rcx + 88h]        ;Get Export Directory RVA from Optional Header
test r9d, r9d
jz NextModule

xor r8d, r8d
lea r8, [rcx + r9]        ;r8 = Export Directory address

mov ecx, [rcx + r9 + 0Ch] ;Get Name RVA from Export Directory
mov eax, [rcx + r10]      ;Read first 4 bytes of DLL name. Uses r10
or eax, 20202020h		  ;Make lowercase
cmp eax, 6C64746Eh        ;Compare with 'ntdl' (little-endian) 6C642E6C6C64746E
jnz NextModule            ;If it did not find the module

mov eax, [rcx + r10 + 04h] ;Read next 4 bytes
or eax, 20202020h 
cmp eax, 6C642E6Ch
jmp FoundDLL


NextModule:
mov rdx, [rdx]       ;Get next LIST_ENTRY
jmp ModuleLoop       ;Continue loop

FoundDLL:
test r8, r8          ;Check if Export Directory valid
add rsp, 28h
jz Exit

;======================End of finding NTDLL======================



;======================Start of finding LdrLoadDLL======================

mov r11d, [r8 + 18h] ;Set r11 with NumberOfNames RVA
mov edi, [r8 + 20h]  ;Set edi with AddressOfNames RVA
mov esi, [r8 + 1Ch]  ;Set esi with AddressOfFunctions RVA
mov ebp, [r8 + 24h]  ;Set ebp with AddressOfNameOrdinals RVA

add rdi, r10   ;Get the address of NumberOfNames
add rsi, r10   ;Get the address of Functions
add rbp, r10   ;Get the address of NameOrdinals

xor ecx, ecx   ;Set index counter to 0

FindDLL:
cmp ecx, r11d
jae DLLNotFound

mov edx, [rdi + rcx * 4]
add rdx, r10             ;NameVA = base + NameRVA

mov eax, [rdx]
or eax, 20202020h
cmp eax, 6C72646Ch       ;Compare with 'ldrl' (little-endian)  642E6C6C6464616F6C72646C
jne NextDLL

mov eax, [rdx + 04h]
or eax, 20202020h
cmp eax, 6464616Fh
jne NextDLL

mov eax, [rdx + 08h]
or eax, 20202020h
cmp eax, 6C206C6Ch
jne NextDLL

;If found DLL
movzx eax, word ptr [rbp + rcx * 2] ;ordinal
mov eax, [rsi + rax * 4]            ;function RVA
add r10, rax                        ;DLL VA
jmp LoadUser32

NextDLL:      
inc ecx
jmp FindDLL

DLLNotFound:
xor rax, rax
jmp Exit

;======================End of finding LdrLoadDLL======================


;======================Start of loading USER32.DLL======================

LoadUser32:
sub rsp, 40h                 ; reserve enough space

lea rax, [rsp + 20h]

; UTF‑16 string buffer on stack
mov word ptr [rax + 0], 'U'
mov word ptr [rax + 2], 'S'
mov word ptr [rax + 4], 'E'
mov word ptr [rax + 6], 'R'
mov word ptr [rax + 8], '3'
mov word ptr [rax + 0Ah], '2'
mov word ptr [rax + 0Ch], '.'
mov word ptr [rax + 0Eh], 'D'
mov word ptr [rax + 10h], 'L'
mov word ptr [rax + 12h], 'L'
mov word ptr [rax + 14h], 0     ; null terminator

; UNICODE_STRING at [rsp+20h]
mov word ptr [rsp + 0], 20     ; Length (bytes)
mov word ptr [rsp + 2], 22     ; MaximumLength
mov qword ptr [rsp + 8], rax   ; Buffer


xor rcx, rcx
xor rdx, rdx
mov qword ptr [rsp - 78h], 0

lea r9, [rsp - 78h] 
lea r8, [rsp]               ; UNICODE_STRING*

sub rsp, 20h                  ; shadow space
call r10
add rsp, 20h
mov r8, [rsp - 78h] ;r8 contains base address of user32.dll
mov rcx, r8
mov rbx, rcx
test r8, r8
jz Exit

;======================End of loading USER32.DLL======================


;======================Start of loading MessageBoxA======================

movsxd rax, dword ptr [r8 + 3Ch]  ;Get e_lfanew (PE header offset)
xor r9d, r9d
mov r9d, [rax + r8 + 88h]        ;Get Export Directory RVA from Optional Header
test r9d, r9d

xor r8d, r8d
lea r8, [rcx + r9]        ;r8 = Export Directory address

LoadMessageBox:
mov r11d, [r8 + 18h] ;Set r11 with NumberOfNames RVA
mov edi, [r8 + 20h]  ;Set edi with AddressOfNames RVA
mov esi, [r8 + 1Ch]  ;Set esi with AddressOfFunctions RVA
mov ebp, [r8 + 24h]  ;Set ebp with AddressOfNameOrdinals RVA

add rdi, rcx   ;Get the address of NumberOfNames
add rsi, rcx   ;Get the address of Functions
add rbp, rcx   ;Get the address of NameOrdinals

xor ecx, ecx   ;Set index counter to 0

FindFunction:
cmp ecx, r11d
jae FunctionNotFound

mov edx, [rdi + rcx * 4]
add rdx, rbx             ;NameVA = base + NameRVA

mov eax, [rdx]
or eax, 20202020h
cmp eax, 7373656Dh
jne NextFunction

mov eax, [rdx + 04h]
or eax, 20202020h
cmp eax, 62656761h
jne NextFunction

mov eax, [rdx + 07h]
or eax, 20202020h
cmp eax, 61786F62h
jne NextFunction

;If found function
movzx eax, word ptr [rbp + rcx * 2] ;ordinal
mov eax, [rsi + rax * 4]            ;function RVA
add rax, rbx                        ;function VA
mov rbx, rax
jmp ActivateMessageBox

NextFunction:
inc ecx
jmp FindFunction

FunctionNotFound:
xor rax, rax
jmp Exit

;======================End of loading MessageBoxA======================


;======================Start of activating MessageBoxA======================

ActivateMessageBox:

sub rsp, 40h                 ; reserve enough space

lea rax, [rsp + 20h]
;Load the caption onto the stack
mov word ptr [rax], 'M'
mov word ptr [rax + 1], 'e'
mov word ptr [rax + 2], 's'
mov word ptr [rax + 3], 's'
mov word ptr [rax + 4], 'a'
mov word ptr [rax + 5], 'g'
mov word ptr [rax + 6], 'e'
mov word ptr [rax + 7], '!'
mov word ptr [rax + 8], 0     ; null terminator

;Load the Message onto the stack
xor rcx, rcx
lea rcx, [rsp + 30h]
mov word ptr [rcx], 'I'
mov word ptr [rcx + 1], ' '
mov word ptr [rcx + 2], 'a'
mov word ptr [rcx + 3], 'm'
mov word ptr [rcx + 4], ' '
mov word ptr [rcx + 5], 'a'
mov word ptr [rcx + 6], ' '
mov word ptr [rcx + 7], 'M'
mov word ptr [rcx + 8], 'e'
mov word ptr [rcx + 9], 's'
mov word ptr [rcx + 10], 's'
mov word ptr [rcx + 11], 'a'
mov word ptr [rcx + 12], 'g'
mov word ptr [rcx + 13], 'e'
mov word ptr [rcx + 14], ' '
mov word ptr [rcx + 15], 'B'
mov word ptr [rcx + 16], 'o'
mov word ptr [rcx + 17], 'x'
mov word ptr [rcx + 18], '!'
mov word ptr [rcx + 19], 0     ;null terminator


xor r9d, r9d
xor rcx, rcx
lea r8, [rsp + 20h]
lea rdx, [rsp + 30h]
call rbx               ;Call Message Box!

;======================End of activating MessageBoxA======================


Exit:
mov  rsp, r12
pop  r12
ret


;Success:
;mov rcx, 1h               ; ExitCode = 1
;call ExitProcess

;Fail:
;xor rcx, rcx               ; ExitCode = 0
;add rsp, 20h
;call ExitProcess


main ENDP
END
