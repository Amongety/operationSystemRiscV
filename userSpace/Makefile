ifeq ($(origin CC), default)
	CC = riscv64-unknown-elf-gcc
endif

ifeq ($(origin LD), default)
	OBJCOPY = riscv64-unknown-elf-objcopy
endif

CFLAGS ?= -ffreestanding -nostdlib -gdwarf-4 -march=rv64imac_zicsr_sstc -mabi=lp64 -ggdb3 -mcmodel=medany
ASFLAGS ?= -march=rv64imac_zicsr_sstc -mabi=lp64 -nostdlib -mcmodel=medany
OUT_ELF_DIR ?= ../build/userSpace
LDFLAGS ?= -T user.lds
COMMONINC = -I ../include/userSpace/

override CFLAGS += $(COMMONINC)

CSRC = user_test.c
COBJ = $(addprefix $(OUT_ELF_DIR)/, $(CSRC:.c=.o))

SSRC = user.S
SOBJ = $(addprefix $(OUT_ELF_DIR)/, $(SSRC:.S=.o))

ALLOBJ = $(SOBJ) $(COBJ)

.PHONY: all
all: $(OUT_ELF_DIR)/user.bin

$(COBJ): $(OUT_ELF_DIR)/%.o : %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(SOBJ): $(OUT_ELF_DIR)/%.o : %.S
	@mkdir -p $(@D)
	$(CC) $(ASFLAGS) -c $< -o $@

$(OUT_ELF_DIR)/user.elf: $(ALLOBJ) user.lds
	$(CC) $(CFLAGS) $(LDFLAGS) $(ALLOBJ) -o $@

$(OUT_ELF_DIR)/user.bin: $(OUT_ELF_DIR)/user.elf
	$(OBJCOPY) -O binary $< $@

.PHONY: clean
clean:
	rm -rf $(OUT_ELF_DIR)
