#ifndef _FAT32_H_
#define _FAT32_H_

#include <Types.h>

namespace FAT32 {
	typedef BOOL(*ReadSectorFunction)(QWORD Sector , BYTE *Buffer);
	typedef BOOL(*WriteSectorFunction)(QWORD Sector , BYTE *Buffer);
	typedef struct {
		ReadSectorFunction ReadSector;
		WriteSectorFunction WriteSector;
	}DRIVE;
	typedef struct {
		BYTE JumpCode[3];
		BYTE OEMID[8];
		WORD BytesPerSector;
		BYTE SectorPerCluster;
		WORD ReservedSectorCount;
		BYTE NumberOfFAT;
		WORD RootDirectoryEntryCount;
		WORD TotalSector16;
		BYTE MediaType;
		WORD FATCount;
		WORD SectorPerTrack;
		WORD NumberOfHeads;
		DWORD HiddenSectors;
		DWORD TotalSector32;
		DWORD FATSize32;
		WORD EXTFlags;
		WORD FileSystemVersion;
		DWORD RootDirectoryCluster;
		WORD FilesystemInformation;
		WORD BackupBootRecord;
		BYTE NoUse1[12];
		BYTE DriveNumber;
		BYTE NoUse2;
		BYTE BootSignature;
		DWORD SerialNumber;
		BYTE VolumeLabel[11];
		BYTE FileSystem[8];
		BYTE BootLoaderCore[420];
		WORD Signature;
	}BPB;
	class Manager {
		public:
			void SetDrive(DRIVE *Drive);
			DRIVE *Drive;
		private:

	};
	BOOL Initialize();
};

#endif