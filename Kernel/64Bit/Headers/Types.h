#ifndef _TYPES_H_
#define _TYPES_H_

typedef char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef unsigned long long int QWORD;

typedef unsigned int size_t;
typedef int BOOL;

#define OS_VERSION "0.1.2"

#define TRUE 1
#define FALSE 0

#define NULL 0x00

#pragma pack(push , 1)

struct VBEMODEINFOBLOCK {
	WORD Attributes;
	BYTE WinA;
	BYTE WinB;
	WORD Granularity;
	WORD WinSize;
	WORD SegmentA;
	WORD SegmentB;
	DWORD ForRealMode1;
	WORD BytesPerScanLine;
	WORD Width;
	WORD Height;	
	BYTE WidthCharSize;
	BYTE HeightCharSize;
	BYTE NumberOfPlane;
	BYTE BitsPerPixel;
	BYTE NumberOfBanks;
	BYTE MemoryModel;
	BYTE BankSize;
	BYTE NumberOfImagedPages;
	BYTE Reserved1;

	BYTE RedMaskSize;
	BYTE RedFieldPosition;
	BYTE GreenMaskSize;
	BYTE GreenFieldPosition;
	BYTE BlueMaskSize;
	BYTE BlueFieldPosition;

	BYTE ReservedMaskSize;
	BYTE ReservedFieldPosition;
	BYTE DirectColorModeInfo;

	DWORD Address;
	DWORD Reserved2;
	DWORD Reserved3;

	WORD LinearBytesPerScanLine;
	BYTE BankNumberOfImagePages;
    BYTE LinearNumberOfImagePages;
    BYTE LinearRedMaskSize;
    BYTE LinearRedFieldPosition;
    BYTE LinearGreenMaskSize;
    BYTE LinearGreenFieldPosition;
    BYTE LinearBlueMaskSize;
    BYTE LinearBlueFieldPosition;
    BYTE LinearReservedMaskSize;
    BYTE LinearReservedFieldPosition;
    DWORD MaxPixelClock;

    BYTE Reserved4[189];
};

#pragma pack(pop)

#endif