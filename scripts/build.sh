#!/bin/bash

set -e
. ./scripts/headers.sh

for PROJECT in $PROJECTS; do
	(cd src/$PROJECT && DESTDIR="$SYSROOT" $MAKE install -j$(nproc))
done
