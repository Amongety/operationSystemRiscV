ifeq ($(origin AS), default)
	AS = riscv64-unknown-elf-gcc
endif

ifeq ($(origin LD), default)
	LD = riscv64-unknown-elf-objcopy
endif

ASFLAGS ?= -march=rv64imaczicsr -mabi=lp64 -nostdlib -mcmodel=medany
OUT_ELF_DIR ?= ../build/bootloader
LDFLAGS ?= -T bootloader.lds
SSRC = bootloader.S
OBJS = $(addprefix $(OUT_ELF_DIR)/, $(SSRC:.S=.o))

.PHONY: all
all: $(OUT_ELF_DIR)/bootloader.bin

$(OBJS): $(OUT_ELF_DIR)/%.o : %.S
	@mkdir -p $(@D)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OUT_ELF_DIR)/bootloader.elf: $(OBJS) bootloader.lds
	$(AS) $(ASFLAGS) $(LDFLAGS) $(OBJS) -o $@

$(OUT_ELF_DIR)/bootloader.bin: $(OUT_ELF_DIR)/bootloader.elf
	$(LD) -O binary $< $@

.PHONY: clean
clean:
	rm -rf $(OUT_ELF_DIR)
