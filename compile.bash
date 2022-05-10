#!/bin/bash

nasm -f elf64 -o cpuid.o fast_C_standard_library/x86/cpuid.asm
gcc test/*.c fast_C_standard_library/*.c fast_C_standard_library/*.h fast_C_standard_library/x86/*.c fast_C_standard_library/x86/*.h -mavx *.o 
