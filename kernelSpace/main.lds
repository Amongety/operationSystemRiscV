ENTRY(main);

SECTIONS
{
	. = 0x80400000;

	.text : {
		_text_start = .;
		
		*(.text)

		_text_end = .;
	}

	.rodata : ALIGN(4) {
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

	.bss : ALIGN(4) {
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
	. += 64 * 1024 * 1024; /* 64MB */
	_free_ram_end = .;

	/DISCARD/ : { *(.eh_frame) }
}
