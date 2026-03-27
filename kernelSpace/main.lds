ENTRY(main);

SECTIONS
{
	. = 0x80400000;

	.text : ALIGN(4096) {
		_text_start = .;
		
		*(.text)

		_text_end = .;
	}

	.rodata : ALIGN(4096) {
		_rodata_start = .;

		*(.rodata)

		_rodata_end = .;
	}

	.data : ALIGN(4096) {
		_data_start = .;
		
		*(.sdata)
		*(.data)

		_data_end = .;
	}

	.bss : ALIGN(4096) {
		_bss_start = .;
		
		*(.sbss)
		*(.bss)

		_bss_end = .;
	}
	
	. = ALIGN(16);
	. += 128 * 1024; /* 128KB */
	_stack = .;

	. = ALIGN(4096);
	
	_free_ram_start = .;
	_free_ram_end = 0x80FFFFFF;

	/DISCARD/ : { *(.eh_frame) }
}
