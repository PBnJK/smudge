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
	movl $(boot_page_table1 - K_ADDRESS), %edi
	movl $0, %esi
	movl $1023, %ecx
1:
	# Only map the kernel.
	cmpl $_kernel_start, %esi
	jl 2f
	cmpl $(_kernel_end - K_ADDRESS), %esi
	jge 3f

	# Map physical address as "present, writable". Note that this maps
	# .text and .rodata as writable. Mind security and map them as non-writable.
	movl %esi, %edx
	orl $0x003, %edx
	movl %edx, (%edi)

2:
	# Size of page is 4096 bytes.
	addl $4096, %esi
	# Size of entries in boot_page_table1 is 4 bytes.
	addl $4, %edi
	# Loop to the next entry if we haven't finished.
	loop 1b

3:
	# Map VGA video memory to 0xC03FF000 as "present, writable".
	movl $(0x000B8000 | 0x003), boot_page_table1 - K_ADDRESS + 1023 * 4

	# The page table is used at both page directory entry 0 (virtually from 0x0
	# to 0x3FFFFF) (thus identity mapping the kernel) and page directory entry
	# 768 (virtually from K_ADDRESS to 0xC03FFFFF) (thus mapping it in the
	# higher half). The kernel is identity mapped because enabling paging does
	# not change the next instruction, which continues to be physical. The CPU
	# would instead page fault if there was no identity mapping.

	# Map the page table to both virtual addresses 0x00000000 and K_ADDRESS.
	movl $(boot_page_table1 - K_ADDRESS + 0x003), boot_page_directory - K_ADDRESS + 0
	movl $(boot_page_table1 - K_ADDRESS + 0x003), boot_page_directory - K_ADDRESS + 768 * 4

	# Set cr3 to the address of the boot_page_directory.
	movl $(boot_page_directory - K_ADDRESS), %ecx
	movl %ecx, %cr3

	# Enable paging and the write-protect bit.
	movl %cr0, %ecx
	orl $0x80010000, %ecx
	movl %ecx, %cr0

	# Jump to higher half with an absolute jump. 
	lea 4f, %ecx
	jmp *%ecx

.section .text
4:
	movl $0, boot_page_directory + 0

	# Reload crc3 to force a TLB flush so the changes to take effect.
	movl %cr3, %ecx
	movl %ecx, %cr3
	mov $stack_top, %esp

	call _init
	call kmain

	cli
loop_forever:
	hlt
	jmp loop_forever
