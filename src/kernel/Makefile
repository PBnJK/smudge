# Codename Smudge
# Kernel Makefile

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
BOOTDIR ?= $(EXEC_PREFIX)/boot
INCLUDEDIR ?= $(PREFIX)/include

BINDIR := $(SMUDGEPATH)/bin
OBJDIR := $(SMUDGEPATH)/obj

KERNEL_IMAGE ?= $(BINDIR)/smudge.kernel

CFLAGS := $(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS := $(CPPFLAGS) -D__is_kernel -Iinclude -I$(SMUDGEPATH)/include
LDFLAGS := $(LDFLAGS)
LIBS := $(LIBS) -nostdlib -lk -lgcc

ARCHDIR := arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS := $(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS := $(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS := $(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS := $(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS := \
	$(KERNEL_ARCH_OBJS) \
	kernel.o

OBJS := \
	$(ARCHDIR)/crti.o \
	$(ARCHDIR)/crtbegin.o \
	$(KERNEL_OBJS) \
	$(ARCHDIR)/crtend.o \
	$(ARCHDIR)/crtn.o

LINK_LIST := \
	$(LDFLAGS) \
	$(ARCHDIR)/crti.o \
	$(ARCHDIR)/crtbegin.o \
	$(KERNEL_OBJS) \
	$(LIBS) \
	$(ARCHDIR)/crtend.o \
	$(ARCHDIR)/crtn.o

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .s

all: $(KERNEL_IMAGE)

$(KERNEL_IMAGE): $(OBJS) $(ARCHDIR)/linker.ld
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $(KERNEL_IMAGE)

$(ARCHDIR)/crtbegin.o $(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

.c.o:
	$(CC) -MD -c $< -o $@ -std=gnu99 $(CFLAGS) $(CPPFLAGS)

.s.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

clean:
	rm -f $(KERNEL_IMAGE)
	rm -f $(OBJS) *.o */*.o */*/*.o
	rm -f $(OBJS:.o=.d) *.d */*.d */*/*.d

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps $(SMUDGEPATH)/include/kernel/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(KERNEL_IMAGE)
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $(KERNEL_IMAGE) $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
