Problem 9

16 bits: 1 sign, 5 exponent bits(bias = -15), 10 significand bits

a. Number of representations for NaN?
- To be NaN, we want exponent maxed, significand to be not 0, sign bit can be anything
- Exponent = 31, Sign = 0 or 1, Significand = not 0 (2^11-1 possibilities)
--> 2*(2^11-1)

b. Largest non-infinite number possible?
- Want as positive a number as possible that is not non-infinite
- Sign = 0, Exponent is somewhere from 0-30 (pick 30 to create a very negative number),
  significand can be any value (1111... to make as small as possible)
- Sign: 1, Exp: 30, Significand: 2^12-1
--> (-1)^0 * 2^(30-15) * 1.1111111111
--> + 2^15 * 2*(1-2^-11) = +1 * 2^16 * (1-2^-11) = +1 * (2^16 - 2^5)

c. Positive Number closest to zero that is not zero?
- To get with the itty, bitty numbers, we want to use a denorm
- Denorm form: (-1)^S * 2^(exp +bias + 1) * 0.[significand bits]
- Sign bit: 0, Exponent: 0, Significand: cannot be 0 (choose 1 to minimize)
--> (-1)^0 * 2^(0 - 15 + 1) * 0.00000...1
= + 2^-14 * 2^-10 = 2^-24

d. Number of floating points from 1 to 2 (include 1, exclude 2)
- 1 is represented by: S = 0, Exponent: 15, Significand: 0000...
	--> (-1)^0 * 2^(15-15) * 1.0000
- 2 is represented by: S = 0, Exponent = 16, Significand = 0000...
	--> (-1)^0 * 2^(16-15) * 1.0000
- We want simply alter the significand bits to find the number of values in between
--> 2^10

Problem 10

/* Original Method in C */
void increment(char *d, size_t n) {
	for (int i = 0; i < n; i++) {
		d[i] += 1; // Increment each element in the array
	}
}

/* Original method in RISC-V */
a0: holds value of d
a1: holds value of n
increment:
	beqz a1, END // Size = 0? Go directly to end
	add t0, x0, x0 // Initialize a counter to 0
LOOP:
	lb t1, 0(a0) // Load the char
	addi t1, t1, 1 // Increment the char
	sb t1, 0(a0) // Store new char back in memory
	addi t0, t0, 1 // Increment loop counter
	addi a0, a0, 1 // Increment the address we'll be checking
	blt t0, a1, LOOP // Keep looping if counter < size
END:
	ret


a. Vectorize the C version of increment
void increment_vector(char* d, size_t n) {
	for (int i = 0; i < n/4 *4; i += 4) { // Increment by 4 since 4 chars per int
		*((int*) d + i) += INC;
	}
	/* Tail Case */
	for (int i = ; i < n; i++) {
		d[i] += 1;
	}
}

b. What should the value of INC be to make the method above work properly?
- Want to add 1 to each of the 4 chars
--> INC = [0000 0001] [0000 0001] [0000 0001] [0000 0001]
	INC = 0x01010101

c. Write the loop unrolled, vectorized version of the method in C and in RISC-V

void increment_unroll_vector(char* d, size_t n) {
	int i;
	for (i = 0; i < n/8 * 8; d += 8) {
		*((int*) (d + i)) += INC;
		*((int*) (d + i + 4)) += INC;
	}
	/* Tail Case */
	for (int i = n/8 * 8; i < n; i++) {
		d[i] += 1;
	}
}

increment_unroll_vector:
	beqz a1, END // Size = 0? Go directly to end
	add t2, a1, x0 // t2 holds the size of the array
	slli t2, t2, 3
	slli t2, t2, 3 // Getting rid of 3 LSD's from array size
	beqz t2, TAIL
	add t0, x0, x0 // Initialize a loop counter
	li t3, INC // Let t3 hold the value of inc
LOOP_VEC:
	lw t1, 0(a0) // t1 now contains the value stored in address a0
	add t1, t1, t3
	sw t1, 0(a0) // Store it back now incremented
	addi a0, a0, 4 // Dealing with the second of the 8
	lw t1, 0(a0) // load the 2nd half into t1
	addi t1, t1, t3 // Incrementing 2nd component
	sw t1, 0(a0) // Store incremented result back in original mem location 
	addi a0, a0, 4 // Increment aray address
	addi t0, t0, 8 // Increment loop counter
	blt t0, t2, LOOP_VEC // Branch back to the begining if t0 < t2
TAIL:
	add t0, t2, x0
LOOP_TAIL:
	lbu t1, 0(a0)
	addi t1, t1, 1
	sb t1, 0(a0)
	addi t0, t0, 1
	addi a0, a0, 1
	blt t0, a1, LOOP TAIL
END:
	ret


d. Given a message of length n, how many instructions are needed after loop unrolling?
   Also comment on the speedup as n approaches infinity
- We have 7 RISCV instructions at the begining of the function regardless the value of n
- For the n/8 * 8 elements covered by loop_vec --> they require 10 instructions
- We are obliged to start off with 1 instruction before executing the loop for tail
- For the n % 8 elements in loop_tail --> they use  instructions apiece
- We have 1 final instruction at the end: ret

- In total this amounts to: 7 + 1 + (10 * n/8) + (6 * n % 8) + 1
	- Original Time = val + 6*n (for each char, we needed 6 RISCV instructions)
	- New Time = val + 1.25*n (for each char, we need ~1.2 RISCV instructions on average)
	- Hence, speedup = 6/1.25 = 4.8 as n approaches infinity

e. Use OpenMP directive to speedup the function

void increment_unroll_vector(char* d, size_t n) {
	#pragma omp parallel for // Directive that helps w/ speedup
	for (int i = 0; i < n/8 * 8; d += 8) {
		*((int*) (d + i)) += INC;
		*((int*) (d + i + 4)) += INC;
	}
	/* Tail Case --LOCATION A */
	for (int i = n/8 * 8; i < n; i++) {
		d[i] += 1;
	}
}

What happens if #pragma omp parallel directive is added at location A?
- This is sometimes correct
- There is potentially a data race to increment i (the counter)
- To fix this, we could use #pragma omp parallel for instead

f. If 4 threads can be utilized, what is the maximum speedup compared to the original, non-optimized function?
- Divide each of the cases in the original loop among the 4 threads
- Thus, for the parallelizable sections, they have a speedup of 4 times the original calculated speedup
--> 4 * 4.8 = 19.2

g. Assume increment_unroll_vector (with the SIMD, OpenMP, unrolled implementation) occupies 90% of execution
   and can be parallelized across machines.

Speedup on 16 machines?
- Assume originally took t time to execute
- New Time = (0.9*t)/16 + 0.1*t = 0.15625 * t
--> Speedup = 1/0.15625 = 6.4


Speedup on unlimited machines?
- 10% of the program is still run sequentially (this is our limit from Ahmdals Law)
- We can treat 90% of the program as having run instantaneously --> 10 times speedup



