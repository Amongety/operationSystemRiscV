ENTRY(_start)

SECTIONS
{
	. = 0x20000000;
	
	.text : {
		*(.text)
	}

	.rodata : ALIGN(4) {
		*(.rodata)
	}

	.data : ALIGN(4) {
		_Udata = .;
		*(.sdata)
		*(.data)
	}

	.bss : ALIGN(4) {
		*(.sbss)
		*(.bss)
	}
	
	. = 0xf0000000;

	. = ALIGN(16);
	. += 64 * 1024;
	_Ustack = .;
}
