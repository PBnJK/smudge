/* Codename Smudge
 * LibC -- Standard I/O
 */

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if defined(__is_libk)
#include "kernel/tty.h"
#endif /* if defined(__is_libk) */

static bool print(const char *data, const size_t length);

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

/* TODO: Move to separate file */
int printf(const char *restrict format, ...) {
	va_list args;
	va_start(args, format);

	/* TODO: Implement (previous implementation was from Meaty Skeleton) */
	print(format, strlen(format));

	va_end(args);
	return 0;
}

static bool print(const char *data, const size_t length) {
	const unsigned char *bytes = (const unsigned char *)data;
	for( size_t i = 0; i < length; ++i ) {
		if( putchar(bytes[i]) == EOF ) {
			return false;
		}
	}

	return true;
}
