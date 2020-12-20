Lecture 32: Flynn Taxonomy, SIMD Instructions


Flynn Taxonomy/SIMD Instructions: SIMD Architecture
- Flynn Taxonomy of Parallel Architectures-SIMD, MIMD, SISD, MISD
	- SIMD: single instruction multiple data, MIMD: multiple instruction multiple data, MISD: multiple instruction single data
	- SISD: sequential processor that sequences through instruction pool and processes it one at a time
	- SIMD: computer with multiple processing units that issues one instructions that serves as the operation on multiple pieces of data
	- MIMD: issuing multiple operations that each concurrently operate on multiple pieces of data
		- Each processor is a SIMD that run at the same time
	- MISD: Performing multiple operations on the same piece of data (not really used anymore)
- Intel AVX SIMD Instructions: one instruction fetch that performs an operation on multiple operands at same time
	- Can use C intrinsics in order to implement this


Flynn Taxonomy/SIMD Instructions: SIMD Array Processing


EXAMPLE 1: for each f in array: f = sqrt(f)

Single Instruction, Single Data (SISD) version
for each f in array {
	load f into floating point register
	calculate square root
	write result from register back to memory
}

Single Instruction, Multiple Data (SIMD) version--speedup for 4x
for every 4 members in array {
	load 4 members into SSE register
	calculate 4 sqrt w/ a single vector operation
	write result from register back to memory 
}

EXAMPLE 2: dest.x = v1.x + v2.x; dest.y = v1.y + v2.y; dest.z = v1.z + v2.z; dest.w = v1.w + v2.w;

SSE Instruction Sequence:
	movaps address-of-v, %xmm0 # v1.w | v1.z | v1.y | v1.x --> xmm0
		- Move from mem to XMM register (memory aligned, packed single precision)
	addps address-of-v2, %xmm0 # v1.w + v2.w | v1.z+v2.z | v1.y + v2.y | v1.x + v2.x --> xmm0
		- Add from mem to XMM register (uses packed single precision)
		- Take an operand from an address in memory and adding it directly to a register
			- Can only be done with Intel, not with RISC-V
	movaps %xmm0, address-of-dest 
		- Moves XMM register contents back into memory
- Instrinsics: C functions/procedures for inserting assembly language into C code (includes SSE instructions)
- Intrinsics: vector data type: _m128d, load/store: _mm_load_pd, _mm_store_pd, arithmetic: _mm_add_pd, _mm_mul_pd
	- SSE instructions: load/store: MOVAPD (move aligned packed double), ADDPD, MULPD

Flynn Taxonomy/SIMD Instructions: Matrix Multiplication

Matrix Multiplication--Assume A & B are both 2x2 matrices

Phase 1
	_mm_load_pd to store vectors in memory for the left matrix in column order
		- A = [A(1, 1), A(2, 1)]
	_mm_load1_pd: SSE instruction to load double word, store in high & low double words of XMM register
		- B1 = [B(1,1), B(1,1)], B2 = [B(1, 2), B(1, 2)] --> Used to multipl w/ the A vector
	c1 = _mm_add_pd(c1, _mm_mul_pd(a, b1));
		- C(1, 1) = A(1,1) * B(1,1), C(1, 2) = A(2, 1)* B(1,1) # c1 = vector multiply of A*B1
	c2 = _mm_add_pd(c2, _mm_mul_pd(a, b2)); # Perform multiply + add w/ one instruction
		- C(2, 1) = A(1,1) * B(1,2), C(1, 2) = A(2, 1)* B(1,1) # c2 = vector multiply of A*B2
	- SSE instructions do parallel multiplies, then parallel adds in XMM registers

Phase 2
	- A = [A(1,2), A(2,2)] --> Referring to the second column of A
	- B1 = [B(2, 1), B(2, 1)], B2 = [B(2, 2), B(2, 2)] --> Multiplying against the second row of B
	- C(1,1) = A(1,2) * B(2,1); C(1, 2) = A(2, 2) * B(2, 1); C(2, 1) = A(1, 2) * B(2, 2); C(2, 2) = A(2, 2) * B(2, 2);
		- Add the previous results for c1 + c2 into the new values
		--> C(1,1) = A(1,1)*B(1,1)+A(1,2)*B(2,1)...

2x2 Matrix Multiply with intrinsics
c1 = _mm_load_pd(C + 0 * lda); # c1 = [c_11, c_21]
c2 = _mm_load_pd(C + 1 * lda); # c2 = [c_12, c_22]
for (int i = 0; i < 2; i++) {
	'''
	a = if i = 0 --> [a_11, a_21]; if i = 1 --> [a_12, a_22]
	b1 = if i = 0 --> [b_11, b_11]; if i = 1 --> [b_21, b_21]
	b2 = if i = 0 --> [b_12, b_12]; if i = 1 --> [b_22, b_22]
	'''
	a = _mm_load_pd(A + i * lda); 
	b1 = mm_load1_pd(B + i + 0 * lda);
	b2 = _mm_load1_pd(B + i + 1 * lda);
	'''
	c1 = 
		i = 0 --> [c_11 + a_11 * b_11, c_21 + a_21 * b_11]
		i = 1 --> [c_11 + a_21 * b_21, c_21 + a_22 * b_21]
	c2 = 
		i = 0 --> [c_12 + a_11 * b_12, c_22 + a_21 * b_12]
		i = 1 --> [c_12 + a_21 * b_22, c_22 + a_22 * b_21]
	'''
	c1 = _mm_add_pd(c1, mm_mul_pd(a, b1));
	c2 = _mm_add_pd(c2, mm_mul_pd(a, b2));
}

# Store c1, c2 back into the matrix C
_mm_store_pd(C + 0 * lda, c1);
_mm_store_pd(C + 1 * lda, c2);


