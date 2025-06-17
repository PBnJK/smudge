#!/bin/bash

export SMUDGEPATH=$(pwd)

set -e
. ./scripts/config.sh

for PROJECT in $PROJECTS; do
	(cd src/$PROJECT && $MAKE clean)
done

rm -rf ./sysroot/
rm -rf ./iso/
