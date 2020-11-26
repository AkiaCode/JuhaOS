#ifndef _MEMORYMANAGEMENT_H_
#define _MEMORYMANAGEMENT_H_

#include <Types.h>

#define MEMORY_START 0xF0
#define MEMORY_NEXT 0xE0
#define MEMORY_STARTADDRESS 0xC00000
#define MEMORY_BLOCKSIZE 256

#pragma pack(push , 1)

namespace Memory {
    typedef struct {
        BYTE Start;
        BYTE Allocated;
        unsigned long long int BlockCount;
    }BLOCK;
    class Management {
        public:
            unsigned long long int StartAddress;
            unsigned long long int EndAddress;
            unsigned long long int TotalSize;
            unsigned long long int BlockCount;
            unsigned long long int UsingBlock;
            BLOCK *Blocks;
    };
    
    BOOL Initialize(void);
    unsigned long long int GetTotalSize(void);
    unsigned long long int GetUsingBlock(void);
    unsigned long long int malloc(unsigned long long int Size);
    void free(void *Address);
}

#pragma pack(pop)

#endif