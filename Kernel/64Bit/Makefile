NASM = nasm
CC = g++
LD = ld
OBJCOPY = objcopy

SOURCESFOLDER = Sources
HEADERSFOLDER = Headers
KERNELFOLDER = Kernel
TEMPRORYFOLDER = ../../Temprory

CCOPTION = -m64 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -nostdinc -masm=intel
OCCOPTION = -m64 -O5 -mcmodel=large -ffreestanding -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore -Wno-write-strings -Wno-char-subscripts -Wno-unused-parameter -mno-red-zone -mno-mmx -mno-sse -mno-sse2 -W -Wall -I $(HEADERSFOLDER)/ -nostdlib -nostdinc -masm=intel
TARGETOBJECTS = \
Main.obj \
TextScreen.obj \
string.obj \
Math.obj \
Hal.obj \
DescriptorTables.obj \
ISR.obj \
IRQ.obj \
Common.obj \
Shell.obj \
ShellFunctions.obj \
Task.obj \
RAMDisk.obj \
FAT32.obj \
MemoryManagement.obj \
WindowSystemLibrary.obj
ENTRYFILE = Entry.asm
ASSEMBLYFILE = Assembly.asm

prepare:
	mkdir $(TEMPRORYFOLDER)/64Bit

clean:
	rm -rf $(TEMPRORYFOLDER)/64Bit

all: prepare $(TARGETOBJECTS)
	nasm $(SOURCESFOLDER)/$(ENTRYFILE) -f elf64 -o $(TEMPRORYFOLDER)/64Bit/Entry.obj
	nasm $(SOURCESFOLDER)/$(ASSEMBLYFILE) -f elf64 -o $(TEMPRORYFOLDER)/64Bit/Assembly.obj
	$(CC) $(SOURCESFOLDER)/Graphics.cpp -c $(OCCOPTION) -o $(TEMPRORYFOLDER)/64Bit/Graphics.obj
	$(CC) $(SOURCESFOLDER)/WindowSystem.cpp -c $(OCCOPTION) -o $(TEMPRORYFOLDER)/64Bit/WindowSystem.obj
	cd $(TEMPRORYFOLDER)/64Bit; $(LD) -m elf_x86_64 -T ../../$(KERNELFOLDER)/64Bit/Linker.ld -o ../Kernel64.bin Entry.obj $(TARGETOBJECTS) Assembly.obj Graphics.obj WindowSystem.obj -nostdlib

%.obj:
	$(CC) $(SOURCESFOLDER)/$*.cpp -c $(CCOPTION) -o $(TEMPRORYFOLDER)/64Bit/$@