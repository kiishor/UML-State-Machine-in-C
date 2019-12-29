cd ../demo\simple_state_machine
md build
cd build
del /s/q MinGW
md MinGW
cd MinGW
cmake -G "MinGW Makefiles" ../..
cmake --build .
pause