[BITS 64]

SECTION .text

global Entry
extern Main

Entry:
    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
    mov ss , ax

    mov rsp , 0x600000
    mov rbp , 0x600000

    call Main

    Halt:
        hlt
        jmp Halt