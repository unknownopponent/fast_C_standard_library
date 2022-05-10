section .text

global cpuid_sse
cpuid_sse:

	mov rax, 1

	cpuid

	mov rax, rdx
	shr rax, 25
	and rax, 1

	ret

global cpuid_avx
cpuid_avx:

	mov rax, 1

	cpuid

	mov rax, rcx
	shr rax, 28
	and rax, 1

	ret