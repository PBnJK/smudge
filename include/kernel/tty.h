#ifndef GUARD_SMUDGE_KERNEL_TTY_H_
#define GUARD_SMUDGE_KERNEL_TTY_H_

#include <stddef.h>

void term_init(void);
void term_put_char(const char c);
void term_write(const char *data, size_t size);
void term_write_string(const char *data);

#endif // !GUARD_SMUDGE_KERNEL_TTY_H_
