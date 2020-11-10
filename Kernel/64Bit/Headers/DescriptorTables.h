#ifndef _DESCRIPTORTABLES_H_
#define _DESCRIPTORTABLES_H_

#include <Types.h>

#define TSS_STARTADDRESS 0x400000
#define IST_STARTADDRESS 0x700000
#define IST_SIZE 0x100000

#pragma pack(push , 1)

namespace DescriptorTables {
    typedef struct {
        WORD LimitLow;
        WORD BaseAddressLow;
        BYTE BaseAddressMiddle;
        BYTE Type;
        BYTE LimitHigh;
        BYTE BaseAddressHigh;
    }GDTENTRY;
    typedef struct {
        WORD LimitLow;
        WORD BaseAddressLow;
        BYTE BaseAddressMiddle1;
        BYTE Type;
        BYTE LimitHigh;
        BYTE BaseAddressMiddle2;
        DWORD BaseAddressHigh;
        DWORD Reserved;
    }TSSENTRY;

    typedef struct {
        WORD BaseAddressLow;
        WORD Selector;
        BYTE IST;
        BYTE Type;
        WORD BaseAddressMiddle;
        DWORD BaseAddressHigh;
        DWORD NoUse;
    }IDTENTRY;

    typedef struct {
        WORD Limit;
        QWORD Base;
    WORD wPading;
    DWORD dwPading;
    }DESCRIPTORPOINTER;

    typedef struct {
        DWORD NoUse1;
        QWORD RSP[3];
        QWORD NoUse2;
        QWORD IST[7];
        QWORD NoUse3;
        WORD NoUse4;
        WORD IOMap;
    }TSS;

    void Initialize(void);
    void InitGDT(void);
    void InitIDT(void);
    void InitTSS(TSS *Tss);
    void SendToEOI(int InterruptNumber);
    void SetGDTEntry(int Index , DWORD BaseAddress , DWORD Limit , BYTE FlagsHigh , BYTE FlagsLow , BYTE Type);
    void SetIDTEntry(int Index , QWORD BaseAddress , WORD Selector , BYTE Type);
    void SetTSSEntry(TSSENTRY *Tss , QWORD BaseAddress , DWORD Limit , BYTE FlagsHigh , BYTE FlagsLow , BYTE Type);
}

#pragma pack(pop)

#endif