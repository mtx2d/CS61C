Lecture 34: Thread Level Parallelism II

Parallel Programming Languages
- Examples of parallel programming languages: Go, Concurrent C, Scala, Stackless Python, Java
- SIMD is supported by compilers but difficult to translate C into parallel code
	- Large number of choices b/c there is not a neat universal solution

OpenMP
- OpenMP is a C extension that is multithreaded, shared memory parallelism
	- Compiler directive: #pragma, runtime library routine: #include <omp.h>
	- #pragma: ignored by compilers that do not use OpenMP & same source code across architectures
	- OpenMP only works w/ shared memory
- OpenMP programs begin as single process (the main thread)
	- Parallel region encountered? Main thread "forks" into parallel threads which are
	  executed simultaneously, which are "joined" together once they finish
	- Serial regions require joining and can re-fork when another parallel region encountered
- OpenMP threads are OS (software) threads. OS multiplexes requested OpenMP threads onto hardware threads

Example use of OpenMP

#include <stdio.h>
#include <omp.h>
void main()
{
	omp_set_num_threads(4); // Want to use 4 software threads (don't know # cores/hardware threads)
	int arr[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; // Array of 10 elements: value = index
	int N = sizeof(arr)/sizeof(int); // N = 10 (ten elements)

	#pragma omp parallel for // Signals for loop is parallelized
	for (int i = 0; i < N: i++) {
		printf("Thread: %d, i = %2d\n", omp_get_thread_num(), i);
		a[i] = a[i] + 10 * omp_get_thread_num(); // a[i] clobbers 10s column with thread number
	}
	for (int i = 0; i < N; i++) {
		printf("%02d", a[0]);
	}
	printf("\n");
}

/*
Run the above code with the following:
gcc-5 -fopenmp -o sample sample.c
./sample
*/

RESULT
thread 0, i = 0
thread 1, i = 3
thread 2, i = 6
thread 3, i = 8
thread 0, i = 1
thread 1, i = 4
thread 2, i = 7
thread 3, i = 9
thread 0, i = 2
thread 1, i = 5
- The above ordering is not necessarily accurate for different executions
	- Reality: each of the threads do not necessarily finish in any particular order
[00 01 02] [13 14 15] [26 27] [38 39]
- 1st 3 are part of thread 0, 2nd set of 3 part of thread 1, 6/7 part of thread 2, 8/9 part of thread 3

Computing Pi /* Idea: integrate from 0 to 1 of 4.0/(1+x^2) = pi */

/* Sequential Method */
#include <stdio.h>
void main()
{
	/* Can increase accuracy by increasing numSteps (but slows it down) */
	const long numSteps = 10; // Number of rectangles used to compute 
	double step = 1.0/((double)numSteps); // Width of rectangle to compute area
	double sum = 0.0;
	for (int i = 0; i < numSteps; i++) {
		double x = (i + 0.5) * step; // Find the x value of that rectangle
		sum += 4.0 * step/(1.0 + x * x); // Find the height in respect to the x value --> add to sum
	}
}
- Result: sum = 3.14245985001, which is ~pi but not very accurate. So the idea is to increase
  steps & parallelize to increase the accuracy

/* Faulty parallelization */
#include <stdio.h>
void main() /* Function is essentially the same as above */
{
	const long numSteps = 10;
	double step = 1.0/((double)numSteps);
	double sum = 0.0;
	#pragma parallel for
	for (int i = 0; i < numSteps; i++) {
		double x = (i + 0.5) * step;
		sum += 4.0 * step/(1.0 + x * x);
	}
}
- Issue with the above: each thread needs to access sum (which is a shared variable)
	- Code will run sequentially as they all wait for sum

/* Computing pi with parallelization */
#include <stdio.h>
void main()
{
	const long numSteps = 10; // Can change numSteps to increase accuracy
	const int numThreads = 4; // Divide sum into 4 regions to be calculated
	double step = 1.0/((double)numSteps);
	double sum[numThreads];
	for (int i = 0 i < numThreads; i++) {
		sum[i] = 0; // Want to initialize each component of sum to 0 to start out with
	}
	omp_set_num_threads(numThreads); // Making a call to set the number of threads
	#pragma omp parallel
	{
		int id = omp_get_thread_num();
		for (int i = id; i < numSteps; i += numThreads) {
			double x = (i + 0.5) * step;
			sum[id] += 4.0 * step/(1.0 + x * x); // Each thread is contributing to a different part of the sum
		}
	}
	double pi = 0; // Initializing the final value of pi
	for (int i = 0; i < numThreads; i++) {
		pi += sum[i]; // Sequentially add up each contribution of sum to pi
	}
}

- Idea: each thread computes a specific part of the integral (0.0, 1.0) gets divided into regions
	- Can thread compute each component of sum in parallel before adding them together at the end
- Is it possible to parallelize computing pi += sum in the end?
	- Issue is the race condition: all the threads that are trying to determine the value of pi are
	  reading intermediate values of pi (which is constantly changing), so non-deterministic behavior
	  can occur, since there is no consistency on who accesses pi when

Synchronization
- Main idea: need a way to support parallelization w/o overwriting shared resources
- Key issue: there is limited access to certain resources (example: only 1 entity should be able to
  edit a file at any given time)
	- Solution: need a way to ensure that only one entity using the resouce at any given time
- Lock: mechanism that allow computers to control access to shared resources (usually a variable)

Fake Solution
while (lock != 0); // Checking to see if the lock is "released"
lock = 1; // Locking access to the resource once using the resource
lock = 0; // Unlock access to the resource once don't need it anymore
- Problem: multiple threads trying to lock/unlock can think they have access to lock
  if the timing does not work out
  	- Result: there is going to be overwriting for the shared resource
- There is not a convenient resolution in C (need other possible assembly solutions)

