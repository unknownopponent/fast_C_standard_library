#!/bin/bash

if [ $# -ne 1 ] ; then
	echo "usage ./compile.sh function"
	exit 1
fi

gcc \
	-I ../fast_C_standard_library/ \
	../fast_C_standard_library/*$1*.c \
	../fast_C_standard_library/x86/*$1*.c \
	../test/*$1*.c \
	-O3 -march=native -funroll-loops \
	-Wno-incompatible-function-pointer-types -Wno-incompatible-pointer-types \
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
