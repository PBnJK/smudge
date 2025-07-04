# Codename Smudge
# LibC Makefile

DEFAULT_HOST != $(SMUDGEPATH)/scripts/default_host.sh
HOST ?= DEFAULT_HOST
HOSTARCH != $(SMUDGEPATH)/scripts/target_triple_to_arch.sh $(HOST)

CFLAGS ?= -O2 -g
CPPFLAGS ?=
LDFLAGS ?=
LIBS ?=

DESTDIR ?=
PREFIX ?= /usr/local
EXEC_PREFIX ?= $(PREFIX)
INCLUDEDIR ?= $(PREFIX)/include
LIBDIR ?= $(EXEC_PREFIX)/lib

LIBCDIR := $(SMUDGEPATH)/src/libc
BINDIR := $(SMUDGEPATH)/bin
OBJDIR := $(SMUDGEPATH)/obj

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_libc -Iinclude -I$(SMUDGEPATH)/include
LIBK_CFLAGS := $(CFLAGS)
LIBK_CPPFLAGS := $(CPPFLAGS) -D__is_libk

ARCHDIR := arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS := $(CFLAGS) $(ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(ARCH_CPPFLAGS)
LIBK_CFLAGS := $(LIBK_CFLAGS) $(KERNEL_ARCH_CFLAGS)
LIBK_CPPFLAGS := $(LIBK_CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)

FREEOBJS := \
	$(ARCH_FREEOBJS) \
	stdio/stdio.o \
	stdio/printf.o \
	stdlib/stdlib.o \
	string/string.o

HOSTEDOBJS := \
	$(ARCH_HOSTEDOBJS)

OBJS := \
	$(FREEOBJS) \
	$(HOSTEDOBJS)

LIBK_OBJS := $(FREEOBJS:.o=.libk.o)

#BINARIES=libc.a libk.a # Not ready for libc yet.
BINARIES := libk.a

.PHONY: all clean install install-headers install-libs
.SUFFIXES: .o .libk.o .c .S

all: $(BINARIES)

libc.a: $(OBJS)
	$(AR) rcs $@ $(OBJS)

libk.a: $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

clean:
	rm -f $(BINARIES)

install: install-headers install-libs

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps $(SMUDGEPATH)/include/libc/. $(DESTDIR)$(INCLUDEDIR)/.

install-libs: $(BINARIES)
	mkdir -p $(DESTDIR)$(LIBDIR)
	cp $(BINARIES) $(DESTDIR)$(LIBDIR)

-include $(OBJS:.o=.d)
-include $(LIBK_OBJS:.o=.d)
