#include <RAMDisk.h>
#include <string.h>
#include <MemoryManagement.h>

static RAMDisk::Manager RAMDiskManager;

void RAMDisk::Initialize(QWORD Size) {
	RAMDiskManager.MaxSize = Size;
}

BOOL RAMDisk::WriteSector(QWORD Sector , BYTE *Buffer) {
	int i;
	BYTE *Disk = (BYTE*)RAMDISK_STARTADDRESS+(Sector*RAMDISK_BYTESPERSECTOR);
	if(Sector*RAMDISK_BYTESPERSECTOR >= RAMDISK_MAXSIZE) {
		return FALSE;
	}
	for(i = 0; i < RAMDISK_BYTESPERSECTOR; i++) {
		*Disk++ = *Buffer++;
	}
	return TRUE;
}

BOOL RAMDisk::ReadSector(QWORD Sector , BYTE *Buffer) {
	int i;
	BYTE *Disk = (BYTE*)RAMDISK_STARTADDRESS+(Sector*RAMDISK_BYTESPERSECTOR);
	if(Sector*RAMDISK_BYTESPERSECTOR >= RAMDISK_MAXSIZE) {
		return FALSE;
	}
	for(i = 0; i < RAMDISK_BYTESPERSECTOR; i++) {
		*Buffer++ = *Disk++;
	}
	return TRUE;
}