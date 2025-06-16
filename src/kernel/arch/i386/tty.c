/* Codename Smudge
 * TTY implementation for i386
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include "kernel/tty.h"
#include "kernel/arch/i386/vga.h"

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t *const VGA_MEMORY = (uint16_t *)0xB8000;

static size_t term_row;
static size_t term_column;
static uint8_t term_color;
static uint16_t *term_buffer;

static uint16_t _get_entry(size_t x, size_t y);
static void _put_entry(char c, uint8_t color, size_t x, size_t y);

static void _write_newline(void);
static void _do_screen_scroll(void);

void term_put_char(char c) {
	switch( c ) {
	case '\n':
		_write_newline();
		break;
	default:
		_put_entry(c, term_color, term_column++, term_row);
	}
}

void term_write(const char *data, size_t size) {
	for( size_t i = 0; i < size; i++ ) {
		term_put_char(data[i]);
	}
}

void term_write_string(const char *data) {
	term_write(data, strlen(data));
}

void term_init(void) {
	term_row = 0;
	term_column = 0;
	term_color = vga_create_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	term_buffer = VGA_MEMORY;

	for( size_t y = 0; y < VGA_HEIGHT; y++ ) {
		for( size_t x = 0; x < VGA_WIDTH; x++ ) {
			_put_entry(' ', term_color, x, y);
		}
	}
}

static uint16_t _get_entry(size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	return term_buffer[index];
}

static void _put_entry(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	term_buffer[index] = vga_create_entry(c, color);

	if( term_column == VGA_WIDTH ) {
		_write_newline();
	}
}

static void _write_newline(void) {
	term_column = 0;
	++term_row;
	if( term_row == VGA_HEIGHT ) {
		term_row = (VGA_HEIGHT - 1);
		_do_screen_scroll();
	}
}

static void _do_screen_scroll(void) {
	for( size_t y = 0; y < VGA_HEIGHT - 1; ++y ) {
		for( size_t x = 0; x < VGA_WIDTH; ++x ) {
			uint16_t entry = _get_entry(x, y + 1);

			const char c = (entry & 0xFF);
			const uint8_t color = (entry >> 8);
			_put_entry(c, color, x, y);
		}
	}

	for( size_t x = 0; x < VGA_WIDTH; ++x ) {
		_put_entry(' ', term_color, x, VGA_HEIGHT - 1);
	}
}
