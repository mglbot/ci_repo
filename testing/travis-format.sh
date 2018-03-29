#!/bin/bash

echo ""
echo ""
echo "checking the code against clang-format"
echo ""
echo ""

cd src/

diff -u <(cat **/*.cpp **/*.hpp **/*.c **/*.h) <(/usr/bin/clang-format-6.0 -style=file **/*.cpp **/*.hpp **/*.c **/*.h)

exit $?
