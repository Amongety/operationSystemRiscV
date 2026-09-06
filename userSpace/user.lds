ENTRY(_start)

SECTIONS
{
	. = 0x40000000;
	
	.text : ALIGN(4096) {
		*(.text)
	}

	.rodata : ALIGN(4096) {
		*(.rodata)
	}

	.data : ALIGN(4096) {
		_Udata = .;
		*(.sdata)
		*(.data)
	}

	.bss : ALIGN(4096) {
		*(.sbss)
		*(.bss)
	}
	
	. = 0xf0000000;

	. = ALIGN(16);
	. += 64 * 1024;
	_Ustack = .;
}
