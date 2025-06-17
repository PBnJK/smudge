/* Codename Smudge
 * LibC -- Standard I/O
 */

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#if defined(__is_libk)
#include "kernel/tty.h"
#endif /* if defined(__is_libk) */

int puts(const char *str) {
	return printf("%s\n", str);
}

int putchar(const int c) {
#if defined(__is_libk)
	const char character = (char)c;
	term_write(&character, sizeof(character));
#else
	/* TODO: Implement stdio and the write syscall */
#endif
	return c;
}
