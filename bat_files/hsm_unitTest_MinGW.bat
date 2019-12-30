cd ../test/hsm_test
md build
cd build
del /s/q MinGW
md MinGW
cd MinGW
cmake -G "MinGW Makefiles" -DCOVERAGE=1 ../..
cmake --build .
mingw32-make.exe test
.\hsm_UnitTest.exe
pause