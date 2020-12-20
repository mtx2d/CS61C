Discussion 13: Coherence, Atomics, Thread Level Parallelism

Pre-Check
1. Each hardware thread in CPU uses a shared cache. FALSE
- Each hardware thread indeed has its own cache w/in the CPU

2. Atomicity can only be guaranteed within a single RISC-V instruction. FALSE
- Atomicity: property of being executable without being interrupted
- A single RISCV instruction has atomicity b/c it will not be interrupted
- However, multiple instructions can have atomicit for special instructions: load-reserve, shared conditional
	- The above guarantee uninterrupted execution across multiple instructions

3. Speedup is directly proportional to number of threads. FALSE
- The speedup of the parallel components of the code is directly proportional to number of threads
- However, almost all code will have some serial component
	- The runtime of this portion will be unaffected by increasing number of threads
- Thus, by Ahmdal Law, there will not be a directly proportion speedup

Coherency and Atomics
- 2 common concurrency issues in multithreaded programming: cache incoherence, read-modify-write
- Cache incoherence: each hardware thread has its own core/cache. So when we modify the data in one
  thread, it might not be reflected in all the others
  	- Solution: write directly to memory (bypass the cache)
- Read-modify-write: read from memory, modify that value, and write some value back to memory
	- Interweaving of these processes can result in weird behavior
- Solution: use uninterrupted instructions. In RISC-V, this means: load reserve/store conditional, Amo.swap
	- Purpose of load-reserve/store-conditional: allow for uninterrupted executino across multiple instructions
	- Purpose of Amo.swap: allows uninterrupted memory operations w/in single instruction

Load-reserve (lr): load 4 bytes into memory address (write into rd)--register a conditional in memory
store-conditional (sc): only store if the memory address has a reservation (return 0 if able to successfully execute)
Amoswap rd, rs2, (rs1): atomically swap some value in a register with a value in memory

Example 1: Test and Set
Start:  addi t0, x0, 1 // Load the value 1 into t0 to indicate when lock is obtained
		amoswap.w.aq t1, t0, (a0) // Swap the value of t0 w/ value in memory address in a0
		bne t1, x0, Start // If value in t1 isn't 0, that means some other thread has the lock--so restart process
/** Critical section here */
amoswap.w.r1 x0, x0, (a0) // Release the lock by amoswapping with the value 0

Example 2: Compare and Swap
a0: memory location, a1: expecred value, a2: desired value, a0: 0 = success, failure otherwise

cas:
	lr.w t0, (a0) // Load reserve value in memory from a0 into t0
	bne t0, a1, fail // If loaded value != target value: jump immediately to failure
	sc.w a0, a2, (a0) // Store conditional: try to update with desired value
	// If we're able to successfully execute store conditional, it'll return 0. Else, non-0 error code to be returned
	jr ra
fail:
	li a0, 1 // Value to return is failure (non-zero exit cod)
	jr ra // Return

1. Use of the special instructions sc, lr, amoswap?
- Using lw/sw with compare and swap: if some thread changes memory bits (interrupting execution), then when a 
  new thread takes over, then desired behavior may not be achieved
- If multiple threads use lw/sw, there is no guarantee that the cores will not both think they have access to lock
	- The critical section is useless (we want syncrhonization b/w parallel cores)
  
- Problem with using normal loads/stores:
2. Fill out the following code: execute sequentially according to thread number (execute thread 0 first, then thread 1, etc.)

addi t0, x0, 0
sw t0, 0(a1) // Set up for the 0th thread
/**
Generate 4 threads here
**/
Check:  jal ra, get_thread_num // Get current thread number--threadNum is in register a0
	    lr.w t0, (a1) /** Get ID of next thread to work with--memory in a1 contains the next thread to run
						  Load reserve ensures there is no interference **/
		bne a0, t0, Check // Branch back (keep checking) to Check if t0 != threadNum--a0 = threadNum, t0 = threadToRun
Done:   addi t0, t0, 1 // Increment the next thread to run
		sc.w a0, t0, (a1) // Set the next thread to run. Conditionally store next thread to run
		bne a0, x0, Check // Check store conditional status--repeat process until successfully able to store conditional
/**
Join the 4 threads back together
**/
jr ra
- General process: load reserve from memory, make sure that we got what we wanted from memory, then
  try storing and keep repeating until store conditionalsuccessful

Thread Level Parallelism

1. Analyze the following segments of code

a.
	#pragma omp parallel
	{
		for (int i = 0; i < n; i++)
			arr[i] = i;
	}
- Correct, but slower than serial
- Each process gets forked, and execute independent tasks that can share variables. In the end, the result
  gets joined back together
- Each of the threads in this code will execute the same code from index 0 to n-1
- Overhead of threading will result in slower performance

b.
	arr[0] = 0;
	arr[1] = 1;
	#pragma omp parallel for
	for (int i = 2; i < n; i++)
		arr[i] = arr[i-1] + arr[i-2];
- Incorrect result because of data dependency
- To calculate the nth fibonacci number, requires fib(n-1) and fib(n-2)
- For example, one thread might be trying to compute arr[2] = arr[1] + arr[0] while another
  is working on arr[8] = arr[7] + arr[6]. But clearly, arr[7] cannot have been properly updated if
  another thread is working on arr[2]. Thus, this will yield an incorrect result
- Indeed has speedup, but wrong result
c.
	int i;
	#pragma omp parallel for
	for (i = 0; i < n; i++)
		arr[i] = 0;
- Correct and faster than serial
- There is no data dependency b/c we are simply setting each element to 0 (do not require any other values)
- OpenMP deals w/ privatizing i variable w/ the directive


2. Identify/describe the issue
#pragma omp parallel
{
	int threadCount = omp_get_num_threads;
	int threadID = omp_get_thread_num();
	for (int i = 0; i < n; i++) {
		if (i % threadCount == threadID)
			arr[i] -= 1;
	}
}
- #pragma omp parallel makes this execute on multiple threads
- Want to separate the task into 4 different threads (based on threadID)
	- Typical way to divide tasks b/w multiple threads
	- Thread 0 deals w/ elements 0, 4, 8... Thread 1: 1, 5, 9...
- This can yield to false sharing: situation where each thread has its own copy of memory blocks
	- But then each thread can try to write to same block/cache at the same time
	- Even if they are writing to different parts of the same cache line (that do not interfere),
	  each thread has no way of knowing they did not intefere. Thus, everything must be trashed
	  and the cache will be invalidated when write performed
	- Bad pattern of cache access b/c requires excessive memory accesses

3. 
a. Identify/describe the issue
	double fastProduct(double* arr, int n) {
		double product = 1;
		#pragma omp parallel for
		for (int i = 0; i < n; i++) {
			product *= arr[i];
		}
		return product;
	}
- This could result in a data race: each thread is trying to update product
- Variables declared outside #pragma omp parallel for are shared among all threads (referring to product)
- Example: thread1 thinks product = 1, trying to compute product *= 4 while at the same time, thread2 thinks
  product = 1, trying to compute product *= 5, so there if they both try to update product while obtaining the
  wrong value, problems arise!

b. Fix with #pragma omp critical
	double fastProduct(double* arr, int n) {
		double product = 1;
		#pragma omp parallel for
		for (int i = 0; i < n; i++) {
			#pragma omp critical
			product *= arr[i];
		}
	}
- The issue with this code is it is essentially serial
- Every thread has to wait its turn to be able to update product (one at a time)
- The performance may even suffer b/c of the overhead and the wait time

c. Fix with #pragma omp reduction(operation: var)
	double fastProduct(double* arr, int n) {
		double product = 1;
		#pragma omp parallel reduction(*: product)
		for (int i = 0; i < n; i++) {
			product *= arr[i];
		}
	}
- Essentially creates a private version fo the variable specified (in this case, product)
- So each thread is independently computing its own version of product, and only when each thread
  has computed its own version of product will we "reduce" the result using operation specified in reduction
- Correct and faster than serial!

Amdahl Law

Key Formula: Speedup = 1/(S + (1-S)/P)
- S is the portion of the code that is serial
- P is the factor by which the parallelizable code is sped up
	- P is influenced by number of cores, threads, etc.

1. Classify 100k images across 32 threads. 99% of code is parallelizable. Speedup?
- Using the formula: S = 1-0.99 = 0.01, P = 32 --> Speedup = 1/(0.01 + (0.99/32)) = 24.4275

2. Program takes the following times across functions: f is 30%, g is 10%, h is 60%

a. Functions parallelizability is unknown. Which benefits most from parallelism?
	- h requires the most time, so if we are able to reduce time drain for that, then we would see greatest performance increase

b. Speedup for program applied to chosen function with 8 threads?
	- We chose h to be sped up (so we treat f and g as serial components of the code)
	- Speedup = 1/(0.4 + 0.6/8) = 2.105 (increasing threads by 8-fold only results in ~2.1x speedup)
	




