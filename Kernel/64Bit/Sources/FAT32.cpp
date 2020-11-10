#include <FAT32.h>
#include <string.h>
#include <RAMDisk.h>
#include <TextScreen.h>

static FAT32::Manager FAT32Manager;

BOOL FAT32::Initialize(void) {
	FAT32::BPB BootSector;
	DRIVE *Drive;
	Drive->ReadSector = RAMDisk::ReadSector;
	Drive->WriteSector = RAMDisk::WriteSector;
	FAT32Manager.SetDrive(Drive);

	RAMDisk::WriteSector(0 , (BYTE*)&(BootSector));
}

void FAT32::Manager::SetDrive(FAT32::DRIVE *Drive) {
	this->Drive = Drive;
}