[BITS 32]

SECTION .text

    align 4
	dd 0x1BADB002
	dd (1 << 0)|(1 << 1)
	dd -(0x1BADB002+((1 << 0)|(1 << 1)))

global Entry
extern Main32

GDTInfo:
    dw GDT_END-GDT-1
    dd GDT

GDT:
    NullSegment:
        dw 0x00
        dw 0x00
        db 0x00
        db 0x00
        db 0x00
        db 0x00
    CodeSegment:
        dw 0xFFFF
        dw 0x00
        db 0x00
        db 0b10011010
        db 0b11001111
        db 0x00
    DataSegment:
        dw 0xFFFF
        dw 0x00
        db 0x00
        db 0b10010010
        db 0b11001111
        db 0x00
GDT_END:

Entry:
    cli
    mov esp , Stack
    push ebx
    push eax

    lgdt [GDTInfo]

    call Main32
    Halt:
        jmp Halt

SECTION .bss

Stack:
    resb 8192