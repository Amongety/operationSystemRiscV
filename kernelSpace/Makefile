ifeq ($(origin CC), default)
	CC = riscv64-unknown-elf-gcc
endif

ifeq ($(origin LD), default)
	LD = riscv64-unknown-elf-objcopy
endif

CFLAGS ?= -ffreestanding -nostdlib -gdwarf-4 -march=rv32imaczicsr -mabi=ilp32 -ggdb3
ASFLAGS ?= -march=rv32imaczicsr -mabi=ilp32 -nostdlib
OUT_ELF_DIR ?= ../build/kernelSpace
LDFLAGS ?= -T main.lds
COMMONINC =  -I ../include/kernelSpace/ -I ../include/kernelSpace/arch/riscV -I ../include/kernelSpace/libsbi/ -I ../include/kernelSpace/arch/riscV/memory

override CFLAGS += $(COMMONINC)

CSRC = kernel.c libsbi/sci.c arch/riscV/htrap.c arch/riscV/memory/alloc.c arch/riscV/memory/mmu.c
COBJ = $(addprefix $(OUT_ELF_DIR)/, $(CSRC:.c=.o))

SSRC = libsbi/dbcn.S libsbi/le.S arch/riscV/panic.S arch/riscV/trap.S arch/riscV/kernel_init.S arch/riscV/common.S
SOBJ = $(addprefix $(OUT_ELF_DIR)/, $(SSRC:.S=.o))

ALLOBJ = $(COBJ) $(SOBJ)

.PHONY: all
all: $(OUT_ELF_DIR)/kernel.bin

$(COBJ): $(OUT_ELF_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(SOBJ): $(OUT_ELF_DIR)/%.o : %.S
	@mkdir -p $(@D)
	$(CC) $(ASFLAGS) -c $< -o $@

$(OUT_ELF_DIR)/kernel.elf: $(ALLOBJ) main.lds
	$(CC) $(CFLAGS) $(LDFLAGS) $(ALLOBJ) -o $@

$(OUT_ELF_DIR)/kernel.bin: $(OUT_ELF_DIR)/kernel.elf
	$(LD) -O binary $< $@

.PHONY: clean
clean:
	rm -rf $(OUT_ELF_DIR)
