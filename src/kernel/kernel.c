/* Codename Smudge
 * Higher-level kernel entry-point
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "kernel/tty.h"

#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

void kmain(void) {
	term_init();
	printf("Hello, World!\n");
	printf("%d + %d is %d\n", 123, 456, 123 + 456);
}
