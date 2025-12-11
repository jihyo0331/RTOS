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
    /* Dedicated stress region at 11KB~12KB */
    . = 0x00002c00;
    .stress :
    {
        *(.stress)
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
