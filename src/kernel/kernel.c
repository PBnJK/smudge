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
	printf("Hello, Kernel World!\n");
	printf("What the %c%c%c%c %s\n", 'h', 'e', 'c', 'k', "is this?!");
}
