#ifndef _IRQ_H_
#define _IRQ_H_

#include <Types.h>
#include <DescriptorTables.h>
#include <Hal.h>

#define IRQ_STOREREGISTERS() \
    __asm__ (\
        "push rbp;"\
        "mov rbp , rsp;"\
        "push rax;"\
        "push rbx;"\
        "push rcx;"\
        "push rdx;"\
        "push rdi;"\
        "push rsi;"\
        "push r8;"\
        "push r9;"\
        "push r10;"\
        "push r11;"\
        "push r12;"\
        "push r13;"\
        "push r14;"\
        "push r15;"\
        "mov ax , ds;"\
        "push rax;"\
        "mov ax , es;"\
        "push rax;"\
        "push fs;"\
        "push gs;"\
        "mov ax , 0x10;"\
        "mov ds , ax;"\
        "mov es , ax;"\
        "mov gs , ax;"\
        "mov fs , ax;"\
    )

#define IRQ_RESTOREREGISTERS() \
    __asm__ (\
        "pop gs;"\
        "pop fs;"\
        "pop rax;"\
        "mov es , ax;"\
        "pop rax;"\
        "mov ds , ax;"\
        "pop r15;"\
        "pop r13;"\
        "pop r14;"\
        "pop r12;"\
        "pop r11;"\
        "pop r10;"\
        "pop r9;"\
        "pop r8;"\
        "pop rsi;"\
        "pop rdi;"\
        "pop rdx;"\
        "pop rcx;"\
        "pop rbx;"\
        "pop rax;"\
        "pop rbp;"\
        "leave;"\
        "nop;"\
        "iretq;"\
    )

extern "C" void IRQ0_ASM_Timer(void);
extern "C" void IRQ0_Timer(void);
extern "C" void IRQ1_ASM_Keyboard(void);
extern "C" void IRQ1_Keyboard(void);
extern "C" void IRQ12_ASM_Mouse(void);
extern "C" void IRQ12_Mouse(void);

#endif 