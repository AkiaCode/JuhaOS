NASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
KERNELFOLDER = Kernel
TEMPRORYFOLDER = ../../Temprory

CCOPTION = -m32 -ffreestanding -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -nostdinc -Wno-unused-parameter -Wno-pointer-to-int-cast
TARGETOBJECTS = Main.obj Paging.obj
ENTRYFILE = Entry.asm
ASSEMBLYFILE = Assembly.asm

prepare:
	mkdir $(TEMPRORYFOLDER)/32Bit

clean:
	rm -rf $(TEMPRORYFOLDER)/32Bit

all: prepare $(TARGETOBJECTS)
	nasm $(SOURCESFOLDER)/$(ENTRYFILE) -f elf32 -o $(TEMPRORYFOLDER)/32Bit/Entry.obj
	nasm $(SOURCESFOLDER)/$(ASSEMBLYFILE) -f elf32 -o $(TEMPRORYFOLDER)/32Bit/Assembly.obj
	cd $(TEMPRORYFOLDER)/32Bit; $(LD) -m elf_i386 -T ../../$(KERNELFOLDER)/32Bit/Linker.ld -o ../Kernel32.bin Entry.obj $(TARGETOBJECTS) Assembly.obj -nostdlib

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.c -c $(CCOPTION) -o $(TEMPRORYFOLDER)/32Bit/$@