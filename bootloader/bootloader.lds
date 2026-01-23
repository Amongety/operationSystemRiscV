ENTRY(boot)

SECTIONS
{
	. = 0x80200000;
	.text : { *(.text) }
	
	.rodata : ALIGN(4) {
		*(.rodata)
	}

	.data : ALIGN(4) {
		_bootStartData = .;
		*(.data)
	}

	.bss : ALIGN(4) {
		*(.bss)
	}

	. = ALIGN(16);
	. += 128 * 1024; /* 128KB */ 
	_bootStack = .;
}
