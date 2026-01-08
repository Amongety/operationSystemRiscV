ifeq ($(origin CC), default)
	CC = riscv64-unknown-elf-gcc
endif

ifeq ($(origin LD), default)
	LD = riscv64-unknown-elf-objcopy
endif

CFLAGS ?= -ffreestanding -nostdlib -gdwarf-4 -march=rv32imac -mabi=ilp32 -ggdb3
OUT_ELF_DIR ?= ../build/kernelSpace
LDFLAGS ?= -T main.lds
COMMONINC = -I include

override CFLAGS += $(COMMONINC)

CSRC = kernel.c

COBJ = $(addprefix $(OUT_ELF_DIR)/, $(CSRC:.c=.o))

.PHONY: all
all: $(OUT_ELF_DIR)/kernel.bin

$(COBJ): $(OUT_ELF_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(OUT_ELF_DIR)/kernel.elf: $(COBJ) main.lds
	$(CC) $(CFLAGS) $(LDFLAGS) $(COBJ) -o $@

$(OUT_ELF_DIR)/kernel.bin: $(OUT_ELF_DIR)/kernel.elf
	$(LD) -O binary $< $@

.PHONY: clean
clean:
	rm -rf $(OUT_ELF_DIR)
