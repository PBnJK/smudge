.set ALIGN,    1<<0             /* Align loaded modules on page boundaries */
.set MEMINFO,  1<<1             /* Provide memory map */
.set FLAGS,    ALIGN | MEMINFO  /* This is the Multiboot 'flag' field */
.set MAGIC,    0x1BADB002       /* 'Magic number' lets bootloader find the header */
.set CHECKSUM, -(MAGIC + FLAGS) /* Checksum of above, to prove we are multiboot */

.set K_ADDRESS, 0xC0000000

/* Multiboot header */
.section .multiboot.data, "aw"
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

/* Initial stack */
.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

/* These are the pages used for paging */
.section .bss, "aw", @nobits
	.align 4096
boot_page_directory:
	.skip 4096
boot_page_table1:
	.skip 4096

/* Kernel entry point */
.section .multiboot.text, "a"
.global _start
.type _start, @function
_start:
	/* Map boot_page_table1 to physical address */
	mov $(boot_page_table1 - K_ADDRESS), %edi

.section .text
	mov $stack_top, %esp

	call _init
	call kmain

	cli
loop_forever:
	hlt
	jmp loop_forever

/* .size _start, . - _start */
