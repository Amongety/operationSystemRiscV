ENTRY(main);

SECTIONS
{
	. = 0x80400000;
	.text : {
		*(.text)
	}

	.rodata : ALIGN(4) {
		*(.rodata)
	}

	.data : ALIGN(4) {
		*(.data)
	}

	.bss : ALIGN(4) {
		*(.bss)
	}

	. = ALIGN(4);
	. += 128 * 1024; /* 128KB */
	_stack = .;

	/DISCARD/ : { *(.eh_frame) }
}
