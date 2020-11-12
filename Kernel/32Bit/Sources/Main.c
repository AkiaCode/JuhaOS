#include <Types.h>
#include <Paging.h>
#include <Assembly.h>

#define KERNEL32_SECTORCOUNT 16
#define BYTES_PER_SECTOR 512
#define KERNEL32_STARTADDRESS 0x100000
#define KERNEL64_REALADDRESS 0x100000+(KERNEL32_SECTORCOUNT*BYTES_PER_SECTOR)
#define KERNEL64_STARTADDRESS 0x200000
#define KERNEL64_SECTOR_COUNT 32

#define KERNEL64_STACKSIZE 0x800000
#define VBEMODE 0x117

#pragma pack(push , 1)

typedef struct {
    void *Start;
    void *End;
    char *Name;
    DWORD Reserved;
}MODULE;

typedef struct {
    DWORD Flags;
    DWORD MemoryLow;
    DWORD MemoryHigh;
    DWORD BootDrive;
    char *CommandLine;
    DWORD ModulesCount;
    MODULE *Modules;
}MULTIBOOTINFO;

typedef struct {
    WORD DI;
    WORD SI;
    WORD BP;
    WORD SP;
    WORD BX;
    WORD DX;
    WORD CX;
    WORD AX;

    WORD GS;
    WORD FS;
    WORD ES;
    WORD DS;
    WORD EFlags;
}REGISTERS;

#pragma pack(pop)

void SwitchTo64BitModeAndJump(void);
DWORD CheckMaxMemory(void);
BOOL Check64BitSupported(void);

void PrintString(int X , int Y , const char *Buffer);
int strcmp(const char *Destination , const char *Source);
int strlen(const char *Destination);
void LoadModule(int Number , DWORD Address , DWORD MultibootAddress);

void Main32(unsigned long MultibootMagic , unsigned long MultibootAddress) {
    int i = 80*25;
    BYTE *TextScreenBuffer = (BYTE*)0xB8000;
    DWORD RAMSize;
    REGISTERS Registers;
    char *Buffer = (char*)0xACACAC;
    while(i --> 0) {
        *TextScreenBuffer++ = 0x00;
        *TextScreenBuffer++ = 0x07;
    }
    if((RAMSize = CheckMaxMemory()) < 128) {
        PrintString(0 , 0 , "RAM size is too low, min 128MB required :(");
        while(1) {
            ;
        }
    }
    if(Check64BitSupported() == FALSE) {
        PrintString(0 , 0 , "64bit isn't supporting in this PC :(");
        while(1) {
            ;
        }
    }
    Buffer[0] = 0xFF;
    Registers.AX = 0x4F01;
    Registers.CX = VBEMODE;
    Registers.ES = 0x400;
    Registers.DI = 0x00;
    int32(0x10 , &(Registers));

    Registers.AX = 0x4F02;
    Registers.BX = VBEMODE+0x4000;
    int32(0x10 , &(Registers));


    InitPML4(0x16000);
    LoadModule(0 , 0x7C00 , MultibootAddress);
    LoadModule(1 , 0x200000-0x1000 , MultibootAddress);
    SwitchTo64BitAndJump();
    while(1) { 
        ;
    }
}

void LoadModule(int Number , DWORD Address , DWORD MultibootAddress) {
    int i;
    MULTIBOOTINFO *MultibootInfo = (MULTIBOOTINFO*)MultibootAddress;
    BYTE *MemoryAddress;
    BYTE *Buffer = (BYTE*)Address;
    i = 0;
    Buffer[i] = (BYTE)MultibootInfo->Modules[Number].Start;
    for(MemoryAddress = MultibootInfo->Modules[Number].Start; (MemoryAddress < MultibootInfo->Modules[Number].End); MemoryAddress++) {
        Buffer[i++] = (BYTE)(*MemoryAddress);
    }
}

void PrintString(int X , int Y , const char *Buffer) {
    int i;
    const int Offset = ((Y*80)+X)*2;
    char *ScreenBuffer = (char*)0xB8000+Offset;
    for(i = 0; Buffer[i] != '\0'; i++) {
        *(ScreenBuffer++) = Buffer[i];
        *(ScreenBuffer++) = 0x07;
    }
}

int strlen(const char *Destination) {
    int i;
    for(i = 0; Destination[i] != '\0'; i++) {
        __asm__ ("nop");
    }
    return i;
}

int strcmp(const char *Destination , const char *Source) {
    char Dest;
    char Src;
    while(1) {
        Dest = *(Destination)++;
        Src = *(Source)++;
        if(Dest != Src) {
            return 0x01;
        }
        if((Dest == '\0')||(Src == '\0')) {
            break;
        }
    }
    return 0;
}

void CopyKernelTo64BitArea(void) {
    const DWORD *Kernel64RealAddress = (DWORD*)KERNEL64_REALADDRESS;
    DWORD *Kernel64Address = (DWORD*)KERNEL64_STARTADDRESS;
    while(1) {
        if(((DWORD)Kernel64RealAddress) == (KERNEL64_SECTOR_COUNT*BYTES_PER_SECTOR)+0xA200) {
            break;
        }
        *(Kernel64Address) = *(Kernel64RealAddress);
        Kernel64Address++;
        Kernel64RealAddress++;
    }
}

DWORD CheckMaxMemory(void) {
    DWORD Buffer;
    DWORD *MemoryLocation = (DWORD*)KERNEL32_STARTADDRESS;
    while(1) {
        MemoryLocation += 4*MB;
        Buffer = (DWORD)MemoryLocation;
        *MemoryLocation = 0x31415926;
        if(*MemoryLocation != 0x31415926) {
            break;
        }
        *MemoryLocation = Buffer;
    }
    return ((DWORD)MemoryLocation)/MB;
}