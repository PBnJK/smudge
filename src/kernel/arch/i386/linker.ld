/* Codename Smudge
 * Linker script
 */

OUTPUT_FORMAT(elf32-i386)

ENTRY(_start)

SECTIONS
{
	. = 2M;

	K_ADDRESS = 0xC0000000;

	_kernel_start = .;

    .multiboot.data : {
		*(.multiboot.data)
	}

	.multiboot.text : {
		*(.multiboot.text)
	}

	. += K_ADDRESS;

	.text ALIGN(4K) : AT (ADDR(.text) - K_ADDRESS)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT (ADDR(.rodata) - K_ADDRESS)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT (ADDR(.data) - K_ADDRESS)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT (ADDR(.bss) - K_ADDRESS)
	{
		*(COMMON)
		*(.bss)
		*(.bootstrap_stack)
	}

	_kernel_end = .;
}
