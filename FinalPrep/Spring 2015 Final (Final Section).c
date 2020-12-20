Parallelization Practice

Spring 2015 Final

QUESTION F1: Virtual Memory
- Spec: 13 bit physical address space, 2 pages in memory, 16 MiB VM, LRU replacement
	- VM Size: 16 MiB --> 2^24 B
	- Formula 1: VPN + Offset = Virtual Address = 24
	- Formula 2: PPN + Offset = Physical Address = 13
	- The offset in both cases should be the same
a. Determine the size of the following:
	Virtual Page Number (VPN): 24
		- Need 24 bits to encompass entirety of 16MiB virtual memory
		- VPN = Virtual Address - Offset = 24 - 12 = 12
	Physical Page Number (PPN): 1
		- We are given that the physical address requires 13 bits
		- PPN = Physical Address - Offset = 13 - 12 = 1
	Offset: 12
		- Offset = log2(page size) b/c we want to be able to fully access info w/in a single page
		- Physical address holds 2 pages --> Each page holds 1/2 of physical address space
			--> 2^12 B needed per page --> Offset = 12



b. The following program takes 4096B. What is the maximum value of array_size to allow this
   program to execute with 0 page faults in best case scenario?

void enumerate(int* a, int* b) {
	for (int i = 0; i < array_size; i++) {
		a[i] = i; // Access a at i and set it to i
		b[i] = array_size - i; // Access b at i and set it equal to array_size - i
	}
}

- Code "weight": 2^12 B
- To have no page faults in best case scenario, all the pages that are required are in TLB and in page table
	- In this scenario, arrays a, b have been previously accessed
	- However, one of the pages (2^12 B) has been used to support the original code (2^12 B)
	- We have one page that we can still use to hold a, b --> can hold 2^11 B each w/ 2^12 B in the original page
		--> 2^11 B --> 2^9 ints

c. How can we accommodate a larger value of array_size and execute w/ fewest page faults in best case scenario
   by modifying the code above?
- We cannot change anything fundamentally about how the memory functions
- Thus, we instead want to change how we are accessing our data
	- One pattern to help out might to be access all of a, then access all of b
	- In the above code, we needed to access a, b alternating --> both are needed in the page

void enumerate(int* a, int* b) {
	for (int i = 0; i < array_size; i++) {
		a[i] = i;
	}
	for (int i = 0; i < array_size; i++) {
		b[i] = array_size - i;
	}

}


QUESTION F2
a. Optimize factorial w/ SIMD intrinsics
/* Original version */
double factorial(int k) {
	int i;
	double f = 1.0;
	for (i = 1; i <= k; i++) {
		f *= (double) i;
	}
	return f;
}

/* SIMD version */
Helpful instructions
__m256d _mm256_loadu_pd(double *s)
void _mm256_store_pd(double *s, __mm256d v)
__m256d _mm256_mul_pd(__m256d a, __m256d b)
double factorial(int k) {
	int i, j;
	double f_int[] = {1.0, 1.0, 1.0, 1.0};
	double f_res[4];
	double f = 1.0;
	/* Initialize f_vec--each sub-product w/ initial values provided by f_int */
	__m256d f_vec = __mm256_loadu_pd(f_int);
	/* Vectorize factorial */
	for (i = 1; i <= k/4 * 4; i += 4) {
		/* l[] provides us w/ the doubles we'll be using to multiply in current iteration */
		double l[] = {
			(double) i,
			(double) i+1,
			(double) i+2,
			(double) i+3,
		};
		__256d data = _mm256_loadu_pd(l); /* Store the array of doubles as __m256d */
		_mm256_mul_pd(f_vec, data); // Multiply current product w/ new doubles
	}
	_mm256_store_pd(f_res, f_vec); // Store the temporary products in f_res
	for (j = 0; j < 4; j++) {
		f = f * f_res[j]; // Multiply together each of the temporary products
	}
	/* Tail Cases */
	for (; i <= k; i++) {
		f *= i; // Account for any non-multiples of 4 entries
	}
	return f;
}

b. Analyze the coherence of the following program

#include <stdio.h>
#include "omp.h"
void counter(int *A, int size, int threads) {
	int result[2] = {0, 0};
	int i, j;
	omp_set_num_threads(threads);
	#pragma omp parallel for
	for (j = 0; j < size; j++) {
		result[(A[j] % 2 == 0) > 0 : 1] += 1;
	}
	printf("Even: %d\n", result[0]);
	printf("Odd: %d\n", result[1]);
}

1. Will this run correctly?
- In the end, result will only contain 2 entries: even/odd
- If multiple threads are accessing same entry in result array (both updating simultaneously),
  this becomes a data race
- Thus, this will not run correctly b/c multiple threads can write to same location in memory
2. Can false sharing occur if cache block size is 8 bytes?
- Result array contains 2 ints --> 8 bytes in size
- Say for instance that one thread is trying to update result[0] while another is writing to result[1]
	- This can result in false sharing b/c after updating result[index] --> there is no way of saying
	  that the result[other index] has not been touched
3. Can false sharing occur if cache block size is 4 bytes?
- No, false sharing cannot occur b/c the block size is 1 int
- Each thread is not working on the same block b/c they are working with distinct ints

QUESTION F3

1.Potpourri
	a. CPUs need separate instructions to access I/O devices
	- False, To obtain data from input-output devices, can just use regular load/store instructions

	b. Segmentation has fragmentation problems
	- Segmentation: each process starts working w/ a different component in memory
		- True, If components are deleted --> memory becomes fragmented
	c. Exceptions in early pipeline stages override exceptions in later stages
	- True, exceptions encountered earlier must be handled earlier
	d. Exceptions are handled in pipeline stage when they occur
	- False, exceptions when raised go to a special exception handler
		- This cannot occur during the pipeline stage directly

2. Polling, Interrupts, DMA
2.1: Polling or Interrupt?
- Polling is good for small data (examples: keyboards, mouses, etc.)
	- When we do not receive new data that frequently, then polling works just fine
- Interrupts more effective for large amounts of data per unit of time
	a. Data Rate: 80B/s, Transfer Block Size: 4B
	- 80B/s * 1 cycle/4B = 20 cycles/s
	- Polling would be more appropriate
	b. Data Rate: 400MB/s, Transfer Block Size: 4B
	- 400MB/s * 1 cycle/4B = 10^8 cycles/s
	- Thus, interrupts would be more appropriate
	c. Data Rate: 400MB/s, Transfer Block Size: 2KB
	- 400MB/s * 1 cycle/2KB = 2*10^5 cycles/s
	- Again, interrupts would be more appropriate

2.2: Interrupts require  save/restore current state. Which of the following is required for saving to correctly
	 handle interrupts to execute properly?

Choices: Program Counter, User Registers, TLB, Caches
- Program Counter: Because we are leaving the current program (but plan on coming back to it), we do indeed to save the
  current instruction at which we are operating (stored in PC)
- User Registers: Interrupt handler can potentially change the values stored in registers, so we do want to save the
  values that they currently contain
- TLB: TLB is used for efficient execution regarding virtual page numbers, but will not be needed for correctness (strictly speaking)
- Cache: Once again, caches help increase performance but are not needed for correctness sake
	- Instead, can just go to memory directly to fetch our values if we do not want to save cache

2.3: When is direct memory access most beneficial?
- Direct memory access (DMA): CPU asks for memory, performs tasks while waiting for the memory
- Once memory is accessible, then can just raise an interrupt
- When we bring a lot of data back from Sacramento, that is when DMA really shines

3. Warehouse Scale Computing, Ahmdahl Law

3.1. 90% of a program is parallelizable, but the rest takes 2 times as long due to overhead. Max Speedup?
- Ahmdahl Law: New time = Fraction not speedupable * new time for that fraction = 0.1 * (2t) = 0.2t
--> Speedup = Original Time/New Time = t/(0.2t) = 5

3.2. How can we increase the speedup above in scenario 1?
a. Use more instances
- Using more instances (increasing the number of threads) will not change the maximum speedup
	- In the above scenario, we have already achieved maximum threads for speedup
b. Deploy application across multiple arrays (RAID)
- If we split up the work among multiple arrays, that will indeed increase performance for parallelizable region
	- However, we already assumed that maximum parallel performance is achieved, so does not help
c. Reduce number of reduce operations
- The reduce component of a program involves the serial portion (combining the results post-parallelizable components)
- As a result, we can indeed increase performance by reducing number of a serial operation
d. Increase network bandwidth
- Increasing the network bandwidth enables us to reduce the overhead --> increase performance
	- This in effect allows us to do more tasks in the reduction phase
e. Increase disk capacity
- Increasing the memory does not affect the serial component of the program

4. Hamming Error Correction Code

a. Assume we are given 1 byte value: 0110. Fillin the following table:
- Parity bits at bit 1, 2, 4
	- Parity bit @ 1 checks 1, 3, 5, 7: XOR(Bits at 3, 5, 7) --> 1
	- Parity bit @ 2 checks 2, 3, 6, 7: XOR(Bits at 3, 6, 7) --> 1
	- Parity bit @ 4 checks 4, 5, 6, 7: XOR(bits at 5, 6, 7) --> 0

Bit:      1 2 3 4 5 6 7
Encoding: 1 1 0 0 1 1 0
P1:       1 X 0 X 1 X 0
P2:       X 0 0 X X 0 0
P4:       X X X 0 1 1 0

b. Assume 1001110 has a single bit error. Determine parity bit values & correct the value
  1 2 3 4 5 6 7
  1 0 0 1 1 1 0
- P1 checks bits at: 1, 3, 5, 7 --> XOR(1, 0, 1, 0) --> 0
- P2 checks bits at: 2, 3, 6, 7 --> XOR(0, 0, 1, 0) --> 1
- P4 checks bits at: 4, 5, 6, 7 --> XOR(1, 1, 1, 0) --> 1
- To correct the data, we know that we want a bit that is correct in P1, wrong in P2, wrong in P4
--> By flipping bit 6, we are able to correct P2/P4 while not changing P1
--> New correct data: 1001100


5. Dependability and RAID
- Key formula: availability = MTTF/(MTTF + MTTR)
a. How to increase availability?
	1. Increase MMTF (mean time to failure)
	- True: Increase MTTF --> availability inc b/c numerator inc faster than denominator
	2. Decrease MTTF
	- Incorrect: dec MTTF --> availability decreases
	3. Increase MTTR (mean time to retrieval)
	- Incorrect: inc MTTR --> availability decreases
	4. Decrease MTTR
	- True: dec MTTR --> availability increases b/c denominator decreases in formula
	5. Redundant data copies
	- RAID is good for redundancy which increases the availability in turn

b. Why is RAID1 so expensive?
	- RAID1 is expensive b/c it mirrors all the data
	- RAID1 is a 100% redundant copy of memory (which is expensive)
c. How many check disks are needed in RAID3?
	- RAID3 is striping by bytes w/ 1 disk that holds parity
	- 1 check parity disk exists
d. What gives RAID5 a higher throughput than RAID4?
	- RAID4 = RAID3 using block striping instead of byte stripping
	- RAID5 distributes the parity interweaved acrossed the disks
	- RAID4, when trying to write to stripes in different disks would need to write to parity disk
	- Meanwhile, RAID5 avoids this issue b/c potentially can write to different disks simultaneously
	  w/o having to worry about maintaining integrity of the parity disk as in RAID3/RAID4
	  	- Do not need as much waiting b/c of parity interweaving --> higher throughput






