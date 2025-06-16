/* Codename Smudge
 * LibC -- Standard library
 */

#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void abort(void) {
#if defined(__is_libk)
	/* TODO: kpanic */
	printf("kernel panic: abort()\n");
	asm volatile("hlt");
#else
	/* TODO: Abnormally terminate the process */
	printf("abort()\n");
#endif // is defined(__is_libk)

	while( 1 ) { }
	__builtin_unreachable();
}
