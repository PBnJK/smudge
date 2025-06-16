/* Codename Smudge
 * Higher-level kernel entry-point
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "kernel/tty.h"

#if defined(__linux__)
#error "You are not using a cross-compiler"
#endif

void kmain(void) {
	term_init();
	term_write_string("Hello, Kernel World!\n");
}
