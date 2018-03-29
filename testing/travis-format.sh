#!/bin/bash

echo ""
echo ""
echo "checking the code against clang-format"
echo ""
echo ""

STATUS=0

/usr/bin/clang-format-6.0 --version

exit ${STATUS}
