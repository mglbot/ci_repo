#!/bin/bash -x

echo ""
echo ""
echo "starting new test for real full precision"
echo ""
echo ""

STATUS=0

cd build

cmake ..

make

echo
echo checking J1 full precision
echo ----------------------------------------------------
echo

./bin/check_wfc -f J1 | awk 'BEGIN{status=1} /All checks passed for J1/{status=0} 1; END{exit(status)}'
if [ $? != 0 ] ; then STATUS=1 ; fi

echo
echo checking J2 full precision
echo ----------------------------------------------------
echo

./bin/check_wfc -f J2 | awk 'BEGIN{status=1} /All checks passed for J2/{status=0} 1; END{exit(status)}'
if [ $? != 0 ] ; then STATUS=1 ; fi

echo
echo checking J3 full precision
echo ----------------------------------------------------
echo

./bin/check_wfc -f J3 | awk 'BEGIN{status=1} /All checks passed for J3/{status=0} 1; END{exit(status)}'
if [ $? != 0 ] ; then STATUS=1 ; fi

echo
echo checking Spline SPO full precision
echo ----------------------------------------------------
echo

./bin/check_spo | awk 'BEGIN{status=1} /All checks passed for spo/{status=0} 1; END{exit(status)}'
if [ $? != 0 ] ; then STATUS=1 ; fi

echo
echo checking Determinant update full precision
echo ----------------------------------------------------
echo

./bin/check_determinant | awk 'BEGIN{status=1} /All checks passed for determinant/{status=0} 1; END{exit(status)}'
if [ $?  != 0] ; then STATUS=1 ; fi

exit ${STATUS}
