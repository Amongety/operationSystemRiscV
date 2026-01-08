BOOTLOADER = build/bootloader/bootloader.o
KERNELSPACE = build/kernelSpace/kernel.o
DISK_ISO = osRiscV.iso
DISK_IMG = osRiscV.img
FLASH = osRiscV.bin

all: fileIso

.PHONY: fileIso bootloader kernelSpace

bootloader:
	make -C bootloader

kernelSpace:
	make -C kernelSpace

fileIso: bootloader kernelSpace
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1  seek=0
	dd conv=notrunc if=$(KERNELSPACE) of=$(DISK_IMG) bs=512 count=2 seek=1

.PHONY: qemu
qemu:
	qemu-system-riscv32 -machine virt -bios default -kernel build/bootloader/bootloader.elf -drive file=$(DISK_IMG),format=raw,if=none,id=hd0 -device virtio-blk-device,drive=hd0 -gdb tcp::26000 -S \
	       	-device loader,file=build/kernelSpace/kernel.bin,addr=0x80400000


.PHONY: clean
clean:
	make -C bootloader clean
	make -C kernelSpace clean
	rm $(DISK_IMG)
