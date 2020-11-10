#include <MemoryManagement.h>
#include <Hal.h>
#include <DescriptorTables.h>
#include <TextScreen.h>
#include <RAMDisk.h>

static Memory::Management MemoryManager;

BOOL Memory::Initialize(void) {
    unsigned long long int i;
    unsigned long long int *MemoryLocation;
    unsigned long long int TempBuffer;
    MemoryLocation = (unsigned long long int*)MEMORY_STARTADDRESS;
    while(1) {
        TempBuffer = (*MemoryLocation);
        (*MemoryLocation) = 0x12345678AABBCCDD;
        if(((unsigned long long int)(*MemoryLocation)) != 0x12345678AABBCCDD) {
            break;
        }
        (*MemoryLocation) = TempBuffer;
        MemoryLocation += MEMORY_BLOCKSIZE;
    }
    MemoryManager.Blocks = (Memory::BLOCK*)MEMORY_STARTADDRESS;
    MemoryManager.TotalSize = (unsigned long long int)MemoryLocation-MEMORY_STARTADDRESS;
    MemoryManager.StartAddress = MEMORY_STARTADDRESS+(MemoryManager.TotalSize/MEMORY_BLOCKSIZE);
    MemoryManager.TotalSize -= (MemoryManager.TotalSize/MEMORY_BLOCKSIZE);
    MemoryManager.BlockCount = (unsigned long long int)(MemoryManager.TotalSize/MEMORY_BLOCKSIZE);
    MemoryManager.UsingBlock = 0x00;
    for(i = 0; i < (unsigned long long int)(MemoryManager.TotalSize/MEMORY_BLOCKSIZE); i++) {
        MemoryManager.Blocks[i].Start = 0x00;
        MemoryManager.Blocks[i].Allocated = FALSE;
        MemoryManager.Blocks[i].BlockCount = 0x00;
    }
    return TRUE;
}

unsigned long long int Memory::GetTotalSize(void) {
    return MemoryManager.TotalSize;
}

unsigned long long int Memory::GetUsingBlock(void) {
    return MemoryManager.UsingBlock;
}

unsigned long long int Memory::malloc(unsigned long long int Size) {
    unsigned long long int i;
    int TempColor;
    unsigned long long int UsingBlock;
    unsigned long long int Address = MemoryManager.StartAddress;
    unsigned long long int StartBlock;
    BOOL Founded = FALSE;
    if(Size < MEMORY_BLOCKSIZE) {
        UsingBlock = 1;
    }
    else {
        if(Size%MEMORY_BLOCKSIZE != 0x00) {
            UsingBlock = (Size/MEMORY_BLOCKSIZE)+1;
        }
        else {
            UsingBlock = Size/MEMORY_BLOCKSIZE;
        }
    }
    MemoryManager.UsingBlock += UsingBlock;
    for(i = 0; i < MemoryManager.BlockCount; i++) {
        if(MemoryManager.Blocks[i].Allocated == FALSE) {
            if(MemoryManager.Blocks[i].Start == TRUE) {
                if(MemoryManager.Blocks[i].BlockCount < UsingBlock) {
                    i++;
                    continue;
                }
            }
            MemoryManager.Blocks[i].Allocated = TRUE;
            MemoryManager.Blocks[i].Start = TRUE;
            MemoryManager.Blocks[i].BlockCount = UsingBlock;
            Address = (unsigned long long int)(MemoryManager.StartAddress+(i*MEMORY_BLOCKSIZE));
            Founded = TRUE;
            StartBlock = i;
            break;
        }
    }
    if(Founded == FALSE) {
        TempColor = TextScreen::GetColor();
        TextScreen::SetColor(0x04);
        TextScreen::printf("No more memory for you.");
        TextScreen::SetColor(TempColor);
        return 0x00;
    }
    for(i = StartBlock+1; i < StartBlock+UsingBlock; i++) {
            MemoryManager.Blocks[i].Allocated = TRUE;
            MemoryManager.Blocks[i].Start = FALSE;
            MemoryManager.Blocks[i].BlockCount = 0;
    }
    return Address;
}

void Memory::free(void *Address) {
    unsigned long long int i;
    unsigned long long int BlockAddress;
    unsigned long long int BlockCount;
    BlockAddress = (unsigned long long int)((((unsigned long long int)Address)-MemoryManager.StartAddress)/(MEMORY_BLOCKSIZE));
    if((MemoryManager.Blocks[BlockAddress].Start == FALSE) && (MemoryManager.Blocks[BlockAddress].Allocated == TRUE)) {
        return;
    }
    BlockCount = MemoryManager.Blocks[BlockAddress].BlockCount;
    MemoryManager.UsingBlock -= BlockCount;
    for(i = BlockAddress; i <= BlockAddress+BlockCount; i++) {
        MemoryManager.Blocks[i].Allocated = FALSE;
        MemoryManager.Blocks[i].Start = FALSE; 
        MemoryManager.Blocks[i].BlockCount = 0;
    }
    return;
}