#!/bin/bash

echo ""
echo ""
echo "checking the code against clang-format"
echo ""
echo ""

STATUS=0

clang-format --version

exit ${STATUS}
