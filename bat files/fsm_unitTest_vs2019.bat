cd ../test/fsm_test
md build
cd build
del /s/q vs2019
md vs2019
cd vs2019
cmake -G "Visual Studio 16 2019" ../..
cmake --build .
.\Debug\fsm_UnitTest.exe
pause