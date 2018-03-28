
echo ""
echo ""
echo "starting new test for real full precision"
echo ""
echo ""

cd build

cmake -DCMAKE_C_COMPILER="mpicc" -DCMAKE_CXX_COMPILER="mpicxx" -DCMAKE_CXX_FLAGS="-std=c++11" -DBLAS_blas_LIBRARY="/usr/lib64/libblas.so.3" -DLAPACK_lapack_LIBRARY="/usr/lib64/atlas/liblapack.so.3" ..

make

echo
echo checking J1 full precision
echo ----------------------------------------------------
echo

./bin/check_wfc -f J1

echo
echo checking J2 full precision
echo ----------------------------------------------------
echo

./bin/check_wfc -f J2

echo
echo checking J3 full precision
echo ----------------------------------------------------
echo

./bin/check_wfc -f J3

echo
echo checking Spline SPO full precision
echo ----------------------------------------------------
echo

./bin/check_spo

echo
echo checking Determinant update full precision
echo ----------------------------------------------------
echo

./bin/check_determinant
