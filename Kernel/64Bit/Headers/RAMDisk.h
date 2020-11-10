#ifndef _RAMDISK_H_
#define _RAMDISK_H_

#include <Types.h>
#include <FAT32.h>

#define RAMDISK_STARTADDRESS 0xC00000
#define RAMDISK_MAXSIZE 128*1024*1024
#define RAMDISK_BYTESPERSECTOR 512

namespace RAMDisk {
	class Manager {
		public:	
			QWORD MaxSize;
	};
	
	void Initialize(QWORD Size);
	BOOL WriteSector(QWORD Sector , BYTE *Buffer);
	BOOL ReadSector(QWORD Sector , BYTE *Buffer);
}

#endif