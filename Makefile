BOOTLOADER = build/bootloader/bootloader.o
KERNELSPACE = build/kernelSpace/kernel.o
USERSPACE = build/userSpace/user.o
DISK_ISO = osRiscV.iso
DISK_IMG = osRiscV.img
FLASH = osRiscV.bin
SD_DEVICE = /dev/sde

PREFIX_DPK := /opt/riscv
SRC_DPK := riscv-gnu-toolchain
PATHLINE_DPK := export PATH=$(PREFIX)/bin:$$PATH

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
	dd if=/dev/zero of=loadSD/osRiscVSD.img bs=1M count=256; \
	BOOT_SD_SIZE=$$(stat -c%s loadSD/boot.sd); \
	FIP_SIZE=$$(stat -c%s loadSD/fip.bin); \
	BOOT_SIZE=$$((BOOT_SD_SIZE + FIP_SIZE + 8 * 1024 * 1024)); \
	ALIGN=$$((4 * 1024 * 1024)); \
	BOOT_SIZE=$$(((BOOT_SIZE + ALIGN - 1) / ALIGN * ALIGN)); \
	BOOT_SIZE_MB=$$((BOOT_SIZE / 1024 / 1024)); \
	START_P2=$$((2048 + BOOT_SIZE_MB * 2048)); \
	printf "label: dos\nstart=2048,size=%dM,type=0xC,bootable\n start=%d, type=0x83\n" $$BOOT_SIZE_MB $$START_P2 | sudo sfdisk loadSD/osRiscVSD.img; \
	LOOP=$$(sudo losetup --show -Pf loadSD/osRiscVSD.img); \
	P1=$${LOOP}p1; \
	P2=$${LOOP}p2; \
	sudo mkfs.vfat $${P1}; \
	sudo mkfs.minix -3 $${P2}; \
	sync; \
	mkdir -p loadSD/boot; \
	sudo mount $${P1} loadSD/boot; \
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
	sudo rm loadSD/boot.sd
	sudo rm -r loadSD/boot
	sudo rm loadSD/osRiscVSD.img

.PHONY: clean
clean:
	make -C bootloader clean
	make -C kernelSpace clean
	make -C userSpace clean
	rm $(DISK_IMG)

.PHONY: dpk
dpk:
	sudo apt-get update
	sudo apt-get install -y autoconf automake autotools-dev curl python3 \
		python3-pip libmpc-dev libmpfr-dev libgmp-dev gawk build-essential \
		bison flex texinfo gperf libtool patchutils bc zlib1g-dev \
		libexpat-dev ninja-build git cmake libglib2.0-dev libslirp-dev
 
	sudo mkdir -p $(PREFIX_DPK)
	sudo chown $$USER:$$USER $(PREFIX_DPK)
 
	test -d $(SRC_DPK) || git clone https://github.com/riscv-collab/riscv-gnu-toolchain $(SRC_DPK)
 
	cd $(SRC_DPK) && ./configure --prefix=$(PREFIX_DPK) --enable-multilib
 
	$(MAKE) -C $(SRC_DPK) -j$$(nproc)

	grep -qxF '$(PATHLINE_DPK)' $(HOME)/.bashrc || echo '$(PATHLINE_DPK)' >> $(HOME)/.bashrc
