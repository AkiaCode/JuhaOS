#include <DescriptorTables.h>
#include <Hal.h>
#include <ISR.h>
#include <IRQ.h>
#include <TextScreen.h>
#include <string.h>

using namespace DescriptorTables;

static GDTENTRY GDT[4];
static IDTENTRY IDT[256];

void DescriptorTables::Initialize(void) {
    Hal::WritePort(0x20 , 0x11);
    Hal::WritePort(0x21 , 0x20);
    Hal::WritePort(0x21 , 0x04);
    Hal::WritePort(0x21 , 0x01);

    Hal::WritePort(0xA0 , 0x11);
    Hal::WritePort(0xA1 , 0x28);
    Hal::WritePort(0xA1 , 0x02);
    Hal::WritePort(0xA1 , 0x01);

    Hal::WritePort(0x21 , 0x00);
    Hal::WritePort(0xA1 , 0x00);
    InitIDT();
    InitGDT();
    __asm__ ("sti");
}

void DescriptorTables::InitGDT(void) {
    TSS *Tss = (TSS*)TSS_STARTADDRESS;
    DESCRIPTORPOINTER GDTInfo;
    GDTInfo.Limit = ((sizeof(GDTENTRY)*3)+(sizeof(TSSENTRY)))-1;
    GDTInfo.Base = (QWORD)GDT;
    SetGDTEntry(0 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00);
    SetGDTEntry(1 , 0x00 , 0xFFFFF , 0xA0 , 0x9A , 0x0A);
    SetGDTEntry(2 , 0x00 , 0xFFFFF , 0xA0 , 0x92 , 0x02);
    SetTSSEntry((TSSENTRY*)(&(GDT[3])) , (QWORD)Tss , sizeof(TSS)-1 , 0x80 , 0x80 , 0x09);
    InitTSS(Tss);

    __asm__ ("mov rax , %0"::"r"(&GDTInfo));
    __asm__ ("lgdt [rax]");
    __asm__ ("mov rax , 0x00");

    __asm__ ("mov di , 0x18");
    __asm__ ("ltr di");
}

void DescriptorTables::InitIDT(void) {
    int i;
    DESCRIPTORPOINTER IDTInfo;
    IDTInfo.Limit = (sizeof(IDTENTRY)*256)-1;
    IDTInfo.Base = (QWORD)(IDT);
    for(i = 0; i < 0xFF; i++) {
        SetIDTEntry(i , 0x00 , 0x00 , 0x00);
    }
    SetIDTEntry(0 , (QWORD)ISR0_DividedByZero , 0x08 , 0x8E);
    SetIDTEntry(1 , (QWORD)ISR1_Reserved , 0x08 , 0x8E);
    SetIDTEntry(2 , (QWORD)ISR2_NMI , 0x08 , 0x8E);
    SetIDTEntry(3 , (QWORD)ISR3_BreakPoint , 0x08 , 0x8E);
    SetIDTEntry(4 , (QWORD)ISR4_Overflow , 0x08 , 0x8E);
    SetIDTEntry(5 , (QWORD)ISR5_BoundsRangeExceeded , 0x08 , 0x8E);
    SetIDTEntry(6 , (QWORD)ISR6_InvalidOpcode , 0x08 , 0x8E);
    SetIDTEntry(7 , (QWORD)ISR7_DeviceNotAvailable , 0x08 , 0x8E);
    SetIDTEntry(8 , (QWORD)ISR8_DoubleFault , 0x08 , 0x8E);
    SetIDTEntry(9 , (QWORD)ISR9_CoprocessorSegmentOverrun , 0x08 , 0x8E);
    SetIDTEntry(10 , (QWORD)ISR10_InvalidTSS , 0x08 , 0x8E);
    SetIDTEntry(11 , (QWORD)ISR11_SegmentNotPresent , 0x08 , 0x8E);
    SetIDTEntry(12 , (QWORD)ISR12_StackSegmentFault , 0x08 , 0x8E);
    SetIDTEntry(13 , (QWORD)ISR13_GeneralProtectionFault , 0x08 , 0x8E);
    SetIDTEntry(14 , (QWORD)ISR14_PageFault , 0x08 , 0x8E);
    SetIDTEntry(15 , (QWORD)ISR15_Reserved , 0x08 , 0x8E);
    SetIDTEntry(16 , (QWORD)ISR16_FPUError , 0x08 , 0x8E);
    SetIDTEntry(17 , (QWORD)ISR17_AlignmentCheck , 0x08 , 0x8E);
    SetIDTEntry(18 , (QWORD)ISR18_MachineCheck , 0x08 , 0x8E);
    SetIDTEntry(19 , (QWORD)ISR19_SMIDFloatingPointException , 0x08 , 0x8E);
    SetIDTEntry(20 , (QWORD)ISR20_Reserved , 0x08 , 0x8E);
    SetIDTEntry(21 , (QWORD)ISR21_Reserved , 0x08 , 0x8E);
    SetIDTEntry(22 , (QWORD)ISR22_Reserved , 0x08 , 0x8E);
    SetIDTEntry(23 , (QWORD)ISR23_Reserved , 0x08 , 0x8E);
    SetIDTEntry(24 , (QWORD)ISR24_Reserved , 0x08 , 0x8E);
    SetIDTEntry(25 , (QWORD)ISR25_Reserved , 0x08 , 0x8E);
    SetIDTEntry(26 , (QWORD)ISR26_Reserved , 0x08 , 0x8E);
    SetIDTEntry(27 , (QWORD)ISR27_Reserved , 0x08 , 0x8E);
    SetIDTEntry(28 , (QWORD)ISR28_Reserved , 0x08 , 0x8E);
    SetIDTEntry(29 , (QWORD)ISR29_Reserved , 0x08 , 0x8E);
    SetIDTEntry(30 , (QWORD)ISR30_Reserved , 0x08 , 0x8E);
    SetIDTEntry(31 , (QWORD)ISR31_Reserved , 0x08 , 0x8E);

    SetIDTEntry(32 , (QWORD)IRQ0_ASM_Timer , 0x08 , 0x8E);
    SetIDTEntry(33 , (QWORD)IRQ1_ASM_Keyboard , 0x08 , 0x8E);
    SetIDTEntry(44 , (QWORD)IRQ12_ASM_Mouse , 0x08 , 0x8E);

    __asm__ ("mov rax , %0"::"r"(&IDTInfo));
    __asm__ ("lidt [rax]");
}

void DescriptorTables::InitTSS(TSS *Tss) {
    memset(Tss , 0 , sizeof(TSS));
    Tss->IST[0] = IST_STARTADDRESS+IST_SIZE;
    Tss->IOMap = 0xFFFF;
}

void DescriptorTables::SetGDTEntry(int Index , DWORD BaseAddress , DWORD Limit , BYTE FlagsHigh , BYTE FlagsLow , BYTE Type) {
    GDT[Index].LimitLow = Limit & 0xFFFF;
    GDT[Index].LimitHigh = ((Limit >> 16) & 0xFF)|FlagsHigh;
    GDT[Index].BaseAddressLow = BaseAddress & 0xFFFF;
    GDT[Index].BaseAddressMiddle = (BaseAddress >> 16) & 0xFF; 
    GDT[Index].BaseAddressHigh = (BaseAddress >> 24) & 0xFF;
    GDT[Index].Type = FlagsLow|Type;
}

void DescriptorTables::SetIDTEntry(int Index , QWORD BaseAddress , WORD Selector , BYTE Type) {
    IDT[Index].BaseAddressLow = (QWORD)(BaseAddress & 0xFFFF);
    IDT[Index].BaseAddressMiddle = (QWORD)(BaseAddress >> 16) & 0xFFFF;
    IDT[Index].BaseAddressHigh = (QWORD)(BaseAddress >> 32);
    IDT[Index].Selector = Selector;
    IDT[Index].Type = Type;
    IDT[Index].IST = 0x01 & 0x3;
    IDT[Index].NoUse = NULL;
}

void DescriptorTables::SetTSSEntry(TSSENTRY *Tss , QWORD BaseAddress , DWORD Limit , BYTE FlagsHigh , BYTE FlagsLow , BYTE Type) {
    Tss->LimitLow = Limit & 0xFFFF;
    Tss->LimitHigh = ((Limit >> 16) & 0xFF)|FlagsHigh;
    Tss->BaseAddressLow = BaseAddress & 0xFFFF;
    Tss->BaseAddressMiddle1 = (BaseAddress >> 16) & 0xFF;
    Tss->BaseAddressMiddle2 = (BaseAddress >> 24) & 0xFF;
    Tss->BaseAddressHigh = BaseAddress >> 32;
    Tss->Type = FlagsLow|Type;
    Tss->Reserved = 0;
}

void DescriptorTables::SendToEOI(int InterruptNumber) {
    if(InterruptNumber >= 40) {
        Hal::WritePort(0xA0 , 0x20);
    }
    Hal::WritePort(0x20 , 0x20);
}