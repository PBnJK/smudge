#!/bin/bash

export SMUDGEPATH=$(pwd)

set -e
. ./scripts/build.sh

mkdir -p iso/boot/grub

cp sysroot/boot/smudge.kernel iso/boot/smudge.kernel
cat >iso/boot/grub/grub.cfg <<EOF
menuentry "Smudge" {
	multiboot /boot/smudge.kernel
}
EOF
grub-mkrescue -o iso/smudge.iso iso
