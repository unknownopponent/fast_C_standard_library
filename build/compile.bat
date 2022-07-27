@echo off

if "%1" == "" (
	echo usage : compile.bat test_file_path
	exit /b 1
)

nasm -f elf64 -o cpuid.o ../fast_C_standard_library/x86/cpuid.asm

if %errorlevel% neq 0 (
	echo nasm failled
	exit /b 1
)

gcc ^
	-I ../fast_C_standard_library/ ^
	-I ../fast_C_standard_library/x86 ^
	../fast_C_standard_library/*.c ^
	../fast_C_standard_library/x86/*.c ^
	./*.o ^
	-O3 ^
	%1 ^
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