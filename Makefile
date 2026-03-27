BOOTLOADER = build/bootloader/bootloader.o
KERNELSPACE = build/kernelSpace/kernel.o
USERSPACE = build/userSpace/user.o
DISK_ISO = osRiscV.iso
DISK_IMG = osRiscV.img
FLASH = osRiscV.bin
SD_DEVICE = /dev/sde

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
		-device loader,file=build/userSpace/user.bin,addr=0x80800000

.PHONY: setupSD
setupSD:
	mkimage -f loadSD/boot.its loadSD/boot.sd; \
	dd if=/dev/zero of=loadSD/osRiscVSD.img bs=1M count=128; \
	printf "label: dos\nstart=2048,size=120M,type=0xC,bootable\n" | sudo sfdisk loadSD/osRiscVSD.img; \
	LOOP=$$(sudo losetup --show -Pf loadSD/osRiscVSD.img); \
	PART=$${LOOP}p1; \
	sudo mkfs.vfat $${PART}; \
	sync; \
	mkdir -p loadSD/boot; \
	sudo mount $${PART} loadSD/boot; \
	sudo cp loadSD/fip.bin loadSD/boot/; \
	sudo cp loadSD/boot.sd loadSD/boot/; \
	sync; \
	sudo umount loadSD/boot; \
	sudo losetup -d $${LOOP}; \
	sudo dd if=loadSD/osRiscVSD.img of=$(SD_DEVICE) bs=4M status=progress; \
	sync; \
	make cleanSD

.PHONY: cleanSD
cleanSD:
	rm loadSD/boot.sd
	rm -r loadSD/boot
	rm loadSD/osRiscVSD.img

.PHONY: clean
clean:
	make -C bootloader clean
	make -C kernelSpace clean
	make -C userSpace clean
	rm $(DISK_IMG)
