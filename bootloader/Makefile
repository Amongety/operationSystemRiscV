ifeq ($(origin CC), default)
	CC = riscv64-unknown-elf-gcc
endif

ifeq ($(origin LD), default)
	LD = riscv64-unknown-elf-ld
endif

OBJCOPY = riscv64-unknown-elf-objcopy

CFLAGS ?= -ffreestanding -nostdlib -gdwarf-4 -march=rv64imac_zicsr_sstc -mabi=lp64 -ggdb3 -mcmodel=medany
ASFLAGS ?= -march=rv64imac_zicsr_sstc -mabi=lp64 -nostdlib -mcmodel=medany
OUT_ELF_DIR ?= ../build/bootloader
LDFLAGS ?= -T bootloader.lds
COMMONINC =  -I ../include/bootloader/DTB/ -I ../include/kernelSpace/libsbi/ -I ../include/kernelSpace/libc/

override CFLAGS += $(COMMONINC)

CSRC = DTB/dtb.c ../kernelSpace/libc/byteOrder.c ../kernelSpace/libc/string.c
COBJ = $(addprefix $(OUT_ELF_DIR)/, $(CSRC:.c=.o))

SSRC = bootloader.S ../kernelSpace/libsbi/dbcn.S ../kernelSpace/libsbi/le.S ../kernelSpace/libsbi/te.S 
SOBJ = $(addprefix $(OUT_ELF_DIR)/, $(SSRC:.S=.o))

ALLOBJ = $(SOBJ) $(COBJ)

.PHONY: all
all: $(OUT_ELF_DIR)/bootloader.bin

$(COBJ): $(OUT_ELF_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(SOBJ): $(OUT_ELF_DIR)/%.o : %.S
	@mkdir -p $(@D)
	$(CC) $(ASFLAGS) -c $< -o $@

$(OUT_ELF_DIR)/bootloader.elf: $(ALLOBJ) bootloader.lds
	$(CC) $(CFLAGS) $(LDFLAGS) $(ALLOBJ) -o $@

$(OUT_ELF_DIR)/bootloader.bin: $(OUT_ELF_DIR)/bootloader.elf
	$(OBJCOPY) -O binary $< $@

.PHONY: clean
clean:
	rm -rf $(OUT_ELF_DIR)
