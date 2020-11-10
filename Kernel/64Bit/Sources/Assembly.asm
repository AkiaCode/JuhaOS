[BITS 64]

SECTION .text

global ReadPort
global WritePort

global IRQ0_ASM_Timer
extern IRQ0_Timer
global IRQ1_ASM_Keyboard
extern IRQ1_Keyboard
global IRQ12_ASM_Mouse
extern IRQ12_Mouse

%macro LOADREGISTERS 0
    pop gs
    pop fs
    pop rax
    mov es , ax
    pop rax
    mov ds , ax

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rdx
    pop rcx
    pop rbx
    pop rax
    pop rbp
%endmacro

%macro SAVEREGISTERS 0
    push rbp
    mov rbp , rsp

    push rax
    push rbx
    push rcx
    push rdx
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
    
    mov ax , ds
    push rax
    mov ax , es
    push rax

    push fs
    push gs

    mov ax , 0x10
    mov ds , ax
    mov es , ax
    mov fs , ax
    mov gs , ax
%endmacro

IRQ0_ASM_Timer:
    SAVEREGISTERS
    call IRQ0_Timer
    LOADREGISTERS
    iretq

IRQ1_ASM_Keyboard:
    SAVEREGISTERS
    call IRQ1_Keyboard
    LOADREGISTERS
    iretq

IRQ12_ASM_Mouse:
    SAVEREGISTERS
    call IRQ12_Mouse
    LOADREGISTERS
    iretq

ReadPort:
    push rdx
    mov rdx , rdi
    mov rax , 0x00
    in al , dx
    pop rdx
    ret

WritePort:
    push rax
    push rdx
    mov rdx , rdi
    mov rax , rsi
    out dx , al
    pop rdx
    pop rax
    ret