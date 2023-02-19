@echo off

if "%1" == "" (
	echo usage : compile.bat function
	exit /b 1
)

gcc ^
	-I ../fast_C_standard_library/ ^
	../fast_C_standard_library/*%1*.c ^
	../fast_C_standard_library/x86/*%1*.c ^
	../test/*%1* ^
	-O3 -march=native -funroll-loops ^
	-Wno-incompatible-function-pointer-types -Wno-incompatible-pointer-types ^
	-o test.exe

if %errorlevel% neq 0 (
	echo build failled
	exit /b 1
)

test.exe

if %errorlevel% neq 0 (
	echo test failled
	exit /b 1
)

exit /b 0