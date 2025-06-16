/* Codename Smudge
 * LibC -- String library
 */

#include <stddef.h>
#include <string.h>

size_t strlen(const char *str) {
	size_t len = 0;
	while( str[len] ) {
		len++;
	}

	return len;
}

void *memmove(void *dst, const void *src, const size_t size) {
	unsigned char *dstp = (unsigned char *)dst;

	const unsigned char *srcp = (const unsigned char *)src;
	if( dstp < srcp ) {
		for( size_t i = 0; i < size; ++i ) {
			dstp[i] = srcp[i];
		}
	} else {
		for( size_t i = size; i != 0; --i ) {
			dstp[i - 1] = srcp[i - 1];
		}
	}

	return dst;
}

int memcmp(const void *a, const void *b, const size_t size) {
	const unsigned char *ap = (const unsigned char *)a;
	const unsigned char *bp = (const unsigned char *)b;
	for( size_t i = 0; i < size; ++i ) {
		if( ap[i] < bp[i] ) {
			return -1;
		}

		if( bp[i] < ap[i] ) {
			return 1;
		}
	}

	return 0;
}

void *memset(void *buf, const int value, size_t size) {
	unsigned char *bufp = (unsigned char *)buf;
	for( size_t i = 0; i < size; ++i ) {
		bufp[i] = (unsigned char)value;
	}

	return buf;
}

void *memcpy(void *restrict dst, const void *restrict src, const size_t size) {
	unsigned char *dstp = (unsigned char *)dst;
	const unsigned char *srcp = (const unsigned char *)src;
	for( size_t i = 0; i < size; ++i ) {
		dstp[i] = srcp[i];
	}

	return dst;
}
