Lecture 35: Thread Level Parallelism III

Hardware Synchronization
- OpenMP: C extension that allows for thread level programming in C
	- Breaks for loop into different pieces, that are executed in separate threads
- OpenMP requires simple shape so the compiler can parallelize easily. Also not allowed to exit
  from the loop prematurely (banned list: break, return, exit, goto)
- Data Race Condition: 2+ threads try to access same location w/ 1+ writes
	- Result: the program will depend on chance (based on which thread reaches data first)
	- Want to avoid this non-deterministic behavior by using user-level routines
- Hardware Synchronization Solutions: atomic read/write (only allowing read/write in single instruction)
	- Atomic read/write: instruction that performs read & write in a single instruction
	- Implementation: functions as a swap b/w register and memory & pair of instructions for linked read/write
- Atomic Memory Operations (AMOs): perform operation on operand in memory & set destination register to original
  value in memory
- Idea of locks & atomic operations are commonly used with other parallel programming languages
- OpenMP offers #pragmas for cirtical, atomic, barrier, ordered
- Deadlock: system state that does not allow for any progress
	- Multiple actors are waiting for one another (leading to no action being able to be taken)

Example 1: AMO add
/* 
amoadd.w rd, rs, (rs1)
Idea: have a value in rs2 which we want to add to the value stored at the mem location in rs1 
Step 1: Read the value stored at the memory location pointed to by rs1
Step 2: Store that value into rd (represented by t)
Step 3: Add value in rs2 
*/
amoadd.w rd, rs, (rs1):
	t = M[x[rs1]]; // Load the value at memory rs1 into t
	x[rd] = t; // rd points to the memory location pointed to by rs1
	M[x[rs1]] = t + x[rs2]; // Add the value to memory location


Example 2: RISCV Critical Section (SWAP)
/*
- Assumptions: lock is stored at memory location pointed to by a0 --> 0 means it's free, 1 means it's locked

*/
li t0, 1 // Set lock to 1 if needed
try: amoswap.w.aq t1, t0, (a0) // t1 gets old lock value
- Value in t0 gets moved into a0 while t1 simultaneously set to original value in a0
bnez t1, try // If t1 != 0 --> go back to try
/* Key insight: if t1 = 1 --> some other thread has lock so need to keep trying using atomic operation */
/* Insert critical section HERE--do whatever is needed w/ the exclusive access */
amos.w.r1 x0, x0, (a0) // Release the lock: store 0 in lock to release 

Example 3: OpenMP Locks
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
int main()
{
	omp_lock_t lock; // Create a lock--arbitrary data struct
	omp_init_lock(&lock); // Initialize the lock w/ function call
#pragma omp parallel
	{
		int id = omp_get_thread_num();
		/* Parallel operations */
		omp_set_lock(&lock); // Ensures that only this thread will execute
		/* Start sequential operations */
		printf("ID: %d\n", id);
		/* End sequential operations */
		omp_unset_lock(&lock); // Pass in a pointer to lock
		/* Can resume more parallel operations */
	}
	omp_destroy_lock(&lock);
}

Example : OpenMP critical section
#include <stdio.h>
#include <omp.h>
void main()
{
	const int numThreads = 10; // Set number of software threads
	const long numSteps = 1000; // Set number of iterations
	double step = 1.0/((double)numSteps); // Calculate the step size
	double sum[numThreads]; // Initialize sum to add individual contributions
	for (int i = 0; i < numThreads; i++) {
		sum[i] = 0; // Start w/ sum of 0
	}
	double pi = 0;
	omp_set_num_threads(numThreads); // Set number of threads
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		for (int i = id; i < numSteps; i += numThreads) {
			double x = (i + 0.5) * step;
			sum[id] += 4.0 * step/(x * x); // Parallel add to sum
		}
		#pragma omp critical // Only one thread can enter critical region--threads have to wait
		/* Within this section, only one thread can execute at a given time 
		Benefit: avoid having to use a lock!
		*/
		pi += sum[id]; // Parallel summing of the array
	}
	// At this point, pi should contain value of pi
}

double omp_get_wtime(void)
	- Function returns elapsed wall clock time in seconds, measuring time per thread
	- Subtract two calls of this function to find the elapsed time

Shared Memory and Caches
- SMP: Shared Memory Symmetric Multiprocessor
	- Contains 2+ identical CPU/cores w/ a single shared COHERENT memory
- Key questions: how is data shared? How to coordinate operations? Max num processor supported?
	- Data shared across single address space by every core/processor, shared variables in memory
	  are utilized to communicate/coordinate but requires locking mechanism to prevent clobbering
- Performance bottleneck occurs w/ memory: fetching data from memory is a very taxing operation
	- Introduction of caches lessens the demands on main memory 
- Every core has local private cache for recently accessed data --> cache misses are only accesses of shared memory
- Situation 1: If 2 processors want to read from the same memory location [1000] which holds value of: 20
	- Assume processor 1 does not have 1000 in cache --> Goes to memory, fetches 20 from location, & stores in cache
	- Assume processor 2 likewise does not have it --> Goes to memory, fetches 20, stores it in its local cache
    - Each processor has a copy in its cache
- Situation 2: processor 0 now wants to write 40 to memory[1000]
	- Processor 0 does not have 1000 in local cache --> write to local cache w/ write through
	- Memory now contains 40 at memory[1000] while processor1 and processor2 caches believe memory[1000] = 20

Cache Coherency
- Task at hand: want to keep cache values coherent across the shared memory
	- Solution: need a way for a processor to notify others through a interconnection network when it
	  experiences cache miss or writes
	  - Processors have a way to ask for values at caches of each other & request for others to invalidate values
	  - Read only? Processors can have copies. Write? Other copies are now invalid
	- During write transactions from one processor, other caches check for common tags
- Hardware mechanisms to keep cache coherent: caches track state of every block in cache (track the state)
	- Shared: data is up to date, other caches can also have a copy
	- Modified: my state is up to date, value is changed, memory outdated (other caches do not have a copy)
- Exclusive State: up to date data, other caches still do not have copy, memory is NOT outdated
	- Do not have to write to memory if block is replaced. Do not have to go to memory for read operations
	  b/c memory up to date
- Owner: up to date data, other cahces can have a copy only when they are in a shared state
	- Owner is the only one that can make changes. Memory might be inconsistent. If owner changes value from 20 to 40,
	  owner can share the value of 40 to other caches
	  - Now, all the other caches have 40 who were using it
	  - Idea: try to find the owner of a data to pass the value to the core that previously did not have it in cache
	  	- After issuing the request, the requester is part of the "shared" family
	  - Solution 1: Owner can write data back to memory:  owner state --> shared state (in which case ownership no longer matters)
	  - Solution 2: Change the state to exclusive (invalidate all the other copies)
- Memory access to cache is one of the following: Modified, Owned, Exclusive, Shared, Invalid (not in cache)

Example 1: Invalidate Other Copies
- Processors 1/2 read memory[1000], value = 20, & store that in their cache
- Processor 0 writes 40 to memory[1000], choosing to invalidate other copies
	- Processor 0 cache contains [1000, 40], memory contains [1000, 40] & other caches values are erased
	- Prevents the issue of inconsistent values in blocks across processors

Example 2: An inconsistency...
- Assume block size = 32 bytes, processor 0 read/write variable X (addr = 0x4000), processor 1 read/write Y (addr = 0x4012)
	- Result: block will ping pong b/w 2 caches despite the variables being disjoint, referred to as false sharing
	- Checking at the block level prevents one from taking advantage from accessing X and Y separately
- Solution 2: Mitigating this issue is by using a smaller block size. Solution 2: programmer can design fetches to prevent this
- Coherence Miss: 2 different cores do not realize that they are modifying discrete variables, invalidating other processor copies

