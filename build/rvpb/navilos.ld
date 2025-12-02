ENTRY(vector_start)
SECTIONS
{
	. = 0x0;
	
	
	.text :
	{
		*(vector_start)
		*(.text .rodata)
	}
	.data :
	{
		*(.data)
	}
	.bss :
	{
		*(.bss)
		*(COMMON)
	}

    /* Place framebuffer in low RAM after code/data (within 1MB memory) */
    . = 0x00040000;
    .fb :
    {
        *(.fb)
    }
}
