#include <Paging.h>

void InitPML4(DWORD PML4StartAddress) {
    DWORD i;
    DWORD Flag;
    PML4TABLEENTRY *PML4TEntry;
    PDPTENTRY *PDPTEntry;
    PAGEDIRECTORYENTRY *PageDirectoryEntry;
    DWORD MappingAddress;

    PML4TEntry = (PML4TABLEENTRY*)PML4StartAddress;
    SetPageEntry(&(PML4TEntry[0]) , 0x00 , (PML4StartAddress+0x1000) , PAGE_DEFAULT , 0x00);

    for(i = 1; i < PAGE_MAXENTRYCOUNT; i++) {
        SetPageEntry(&(PML4TEntry[i]) , 0x00 , 0x00 , 0x00 , 0x00);
    }
    PDPTEntry = (PDPTENTRY*)(PML4StartAddress+0x1000);
    for(i = 0; i < 64; i++) {
        SetPageEntry(&(PDPTEntry[i]) , 0x00 , (PML4StartAddress+0x2000)+(i*PAGE_TABLESIZE) , PAGE_DEFAULT , 0x00);
    }
    for(i = 64; i < PAGE_MAXENTRYCOUNT; i++) {
        SetPageEntry(&(PDPTEntry[i]) , 0x00 , 0x00 , 0x00 , 0x00);
    }
    PageDirectoryEntry = (PAGEDIRECTORYENTRY*)(PML4StartAddress+0x2000);
    MappingAddress = 0;
    for(i = 0; i < PAGE_MAXENTRYCOUNT*64; i++) {
        SetPageEntry(&(PageDirectoryEntry[i]) , (i*(PAGE_DEFAULTSIZE >> 20)) >> 12 , MappingAddress , PAGE_DEFAULT|PAGE_PS , 0x00);
        MappingAddress += PAGE_DEFAULTSIZE;
    }
}

void SetPageEntry(PAGETABLEENTRY *Entry , DWORD BaseAddressHigh , DWORD BaseAddressLow , DWORD FlagsLow , DWORD FlagsHigh) {
    Entry->AttribPlusBaseAddressLow = BaseAddressLow|FlagsLow;
    Entry->BaseAddressHighPlusEXB = (BaseAddressHigh & 0xFF)|FlagsHigh;
}