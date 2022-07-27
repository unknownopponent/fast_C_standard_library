#!/bin/bash

if [ $# -ne 1 ] ; then
	echo "usage ./compile.sh test_file_path"
	exit 1
fi

nasm -f elf64 -o cpuid.o ../fast_C_standard_library/x86/cpuid.asm

if [ $? -ne 0 ] ; then
	exit $?
fi

gcc \
	-I ../fast_C_standard_library/ \
	-I ../fast_C_standard_library/x86 \
	../fast_C_standard_library/*.c \
	../fast_C_standard_library/x86/*.c \
	./*.o \
	$1 \
	-O3 \
	-o test.exe

if [ $? -ne 0 ] ; then
	echo "failled to build"
	exit $?
fi

./test.exe

if [ $? -ne 0 ] ; then
	echo "test failled"
	exit $?
fi

exit 0
