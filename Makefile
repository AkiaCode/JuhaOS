NASM = nasm
KERNELFOLDER = Kernel
TEMPRORYFOLDER = Temprory
IMGFILESFOLDER = Floppy/*
KERNELLOADER = KernelLoader.asm
KERNELFILE = Kernel.bin
GRUBFOLDER = iso

MEMSIZE = 8192

CREATEKERNELBINFOLDER = CreateKernelBin
CREATEKERNELBINSOURCE = $(CREATEKERNELBINFOLDER)/CreateKernelBin.c
CREATEKERNELBIN = $(CREATEKERNELBINFOLDER)/CreateKernelBin.out

TARGET = OS.iso

prepare:
	rm -rf iso

all: prepare Build32BitKernel Build64BitKernel BuildTarget

Build32BitKernel:
	make -C $(KERNELFOLDER)/32Bit all

Build64BitKernel:
	make -C $(KERNELFOLDER)/64Bit all

BuildTarget:
	mkdir $(GRUBFOLDER)
	mkdir $(GRUBFOLDER)/boot/
	mkdir $(GRUBFOLDER)/boot/grub
	cp $(KERNELFOLDER)/grub.cfg $(GRUBFOLDER)/boot/grub/grub.cfg 
	cp $(TEMPRORYFOLDER)/Kernel16.bin $(GRUBFOLDER)/boot/
	cp $(TEMPRORYFOLDER)/Kernel32.bin $(GRUBFOLDER)/boot/
	cp $(TEMPRORYFOLDER)/Kernel64.bin $(GRUBFOLDER)/boot/

	grub-mkrescue --output=$(TARGET) $(GRUBFOLDER)/

	rm -rf $(GRUBFOLDER)

clean:
	make -C $(KERNELFOLDER)/32Bit clean
	make -C $(KERNELFOLDER)/64Bit clean
	rm -f $(TARGET)
	rm -f BootLoader.bin

qemurun:
	qemu-system-x86_64 -cdrom $(TARGET) -boot d -m $(MEMSIZE)

debugrun:
	qemu-system-x86_64 -cdrom $(TARGET) -boot d -m $(MEMSIZE) -d int -no-reboot 
