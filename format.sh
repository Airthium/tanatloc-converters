#!/usr/bin/env bash

CLANG_FORMAT="clang-format -i"

# Define targets
# Exclude dxflib
TARGETS=$(find ./ -regex ".*\.\(hpp\|cpp\|c\|h\)$" -not -path "./src/dxflib/*" -not -path "./build/*")

# Run clang-format
for target in $TARGETS; do
    echo "$target"
    $CLANG_FORMAT "$target"
done

