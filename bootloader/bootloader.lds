ENTRY(boot)

SECTIONS
{
	. = 0x80200000;
	.text : { *(.text) }
	
	. = ALIGN(4);
	. += 128 * 1024; /* 128KB */ 
	_bootStack = .;
}
