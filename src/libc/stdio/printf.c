/* Codename Smudge
 * LibC -- Formatted printing (it's huuuuuuuuge!)
 */

#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

#define PRINT_FN(C)                                                            \
	static int _print_##C(struct FormatSettings settings, va_list *args)

#define PRINT_CHECK(C, F)                                                      \
	case C:                                                                    \
		written += _print_##F(settings, &args);                                \
		break

/* Holds the formatting settings */
struct FormatSettings {
	/* Flags */
	bool left_justified; /* Justify left */
	bool force_sign; /* Always show number sign */
	bool space_signed; /* Add a space to signed numbers */
	bool alt; /* Alternate conversion (depends on format) */
	bool leading_zeroes; /* Adds leading zeroes */

	/* Minimum field width */
	bool has_min_width;
	int min_width;

	/* Precision */
	bool has_precision;
	int precision;

	/* Length modifiers */
	int len_short;
	int len_long;
	bool len_long_double;
	bool len_intmax;
	bool len_size_t;
	bool len_ptrdiff;
};

PRINT_FN(percent);
PRINT_FN(c);
PRINT_FN(s);

/* Prints formatted output to stdout
 * Returns the number of characters written
 */
int printf(const char *restrict format, ...) {
	va_list args;
	va_start(args, format);

	int written = 0;

	struct FormatSettings settings = { 0 };

	for( const char *fmt = format; *fmt; ++fmt ) {
		/* Conversion character? */
		if( *fmt == '%' && fmt[1] ) {
			do {
				++fmt;
				switch( *fmt ) {
				case '-':
					settings.left_justified = true;
					continue;
				case '+':
					settings.force_sign = true;
					continue;
				case ' ':
					settings.space_signed = !settings.force_sign;
					continue;
				case '#':
					settings.alt = true;
					continue;
				case '0':
					if( settings.leading_zeroes ) {
						break;
					}

					settings.leading_zeroes = true;
					continue;
				case '.':
					++fmt;

					if( *fmt == '*' ) {
						settings.precision = va_arg(args, int);
						break;
					}
					do {
						settings.precision *= 10;
						settings.precision += (*fmt) - '0';
					} while( *++fmt );
					continue;
				case 'h':
					if( fmt[1] == 'h' ) {
						++fmt;
						settings.len_short = 2;
					} else {
						settings.len_short = 1;
					}
					continue;
				case 'l':
					if( fmt[1] == 'l' ) {
						++fmt;
						settings.len_long = 2;
					} else {
						settings.len_long = 1;
					}
					continue;
				case 'j':
					settings.len_intmax = true;
					continue;
				case 'z':
					settings.len_size_t = true;
					continue;
				case 't':
					settings.len_ptrdiff = true;
					continue;
				case 'L':
					settings.len_long_double = true;
					continue;
				}

				if( *fmt >= '0' && *fmt <= '9' ) {
					do {
						settings.min_width *= 10;
						settings.min_width += (*fmt) - '0';
					} while( *++fmt );
				}

				if( *fmt == '*' ) {
					settings.min_width = va_arg(args, int);
				}

				break;
			} while( fmt[1] );

			switch( *fmt ) {
				PRINT_CHECK('%', percent);
				PRINT_CHECK('c', c);
				PRINT_CHECK('s', s);
			}
		} else {
			putchar(*fmt);
			++written;
		}
	}

	va_end(args);
	return written;
}

PRINT_FN(percent) {
	(void)settings;
	(void)args;

	putchar('%');

	return 1;
}

PRINT_FN(c) {
	(void)settings;

	unsigned char c = (unsigned char)va_arg(*args, int);
	putchar(c);

	return 1;
}

PRINT_FN(s) {
	const char *str = va_arg(*args, const char *);

	int max_chars = (settings.has_precision ? settings.precision : INT_MAX);
	int written = 0;
	while( *str && written < max_chars ) {
		putchar(*str++);
		++written;
	}

	return written;
}

#undef PRINT_FN
#undef PRINT_FN
#undef PRINT_CHECK
