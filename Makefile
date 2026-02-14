BOOTLOADER = build/bootloader/bootloader.o
KERNELSPACE = build/kernelSpace/kernel.o
USERSPACE = build/userSpace/user.o
DISK_ISO = osRiscV.iso
DISK_IMG = osRiscV.img
FLASH = osRiscV.bin

all: fileIso

.PHONY: fileIso bootloader kernelSpace userSpace

bootloader:
	make -C bootloader

kernelSpace: userSpace
	make -C kernelSpace

userSpace:
	make -C userSpace

fileIso: bootloader kernelSpace userSpace
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=2880
	dd conv=notrunc if=$(BOOTLOADER) of=$(DISK_IMG) bs=512 count=1  seek=0
	dd conv=notrunc if=$(KERNELSPACE) of=$(DISK_IMG) bs=512 count=2 seek=1
	dd conv=notrunc if=$(USERSPACE) of=$(DISK_IMG) bs=512 count=3 seek=3

.PHONY: qemu
qemu:
	qemu-system-riscv64 -machine virt -bios default -kernel build/bootloader/bootloader.elf -drive file=$(DISK_IMG),format=raw,if=none,id=hd0 -device virtio-blk-device,drive=hd0 -gdb tcp::26000 -S \
		-monitor stdio \
	       	-device loader,file=build/kernelSpace/kernel.bin,addr=0x80400000 \
		-device loader,file=build/userSpace/user.bin,addr=0x20000000

.PHONY: clean
clean:
	make -C bootloader clean
	make -C kernelSpace clean
	make -C userSpace clean
	rm $(DISK_IMG)
