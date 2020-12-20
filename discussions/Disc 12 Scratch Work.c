kDiscussion 12: Data Level Parallelism

1. Pre-Check

1.1 SIMD is a form of instruction-level parallelism
- False, SIMD stands for single instruction, multiple data
	- Only ever going to be running one instruction (though it is across multiple pieces of data)
- An example of instruction level parallelism is the pipelined CPU
	- Pipelined CPU was capable of executing different instructions simultaneously

1.2 SIMD is ideal for flow-control heavy tasks (tasks w/ branches & if statements)
- False, SIMD works better when there is minimal branching and if statements
	- Rationale: SIMD works best when performing the exact same operations across multiple streams of data
	- Flow control disrupts the continuous flow of execution that SIMD optimizes for

1.3 Intel SIMD intrinsic instructions invoke large registers available on the architecture in order
    to perform one operation on multiple values at once.
- True, SIMD intrinsic instructions utilize 128-bit registers
	- Can treat these as 4 "packed" 32-bit integers that can thus perform an operation like add, sub, mul

2. Flynn Taxonomy

2.1: Single Instruction, Single Data (SISD)
- Idea: every instruction is executed sequentially, working on one stream of data
- Example: traditional computer programs (un-pipelined CPU)

2.2 Single Instruction, Multiple Data (SIMD)
- Idea: one instruction is executed across multiple data streams (instructions executed sequentially)
- Example: Intel Intrinsics (performing vector operations)

2.3 Multiple Instruction, Single Data (MISD)
- Idea: perform different operations on the same piece of data at the same time
- Example: N/A as this model is not really used anymore

2.4 Multiple Instruction, Multiple Data (MIMD)
- Idea: multiple instructions are executed simultaneously across multiple streams of data
- Example: MapReduce or multithreaded programs

3. Data Level Parallelism

int arr[8] = {1, 2, 3, 4, 5, 6, 7, 8};
__m128i vector = _mm_loadu_si128((__m128i*) arr);

a. Multiply vector by itself, set vector to result
vector = _mm_mullo_epi32(vector, vector);

b. Add 1 to first 4 elements of arr --> {2, 3, 4, 5, 5, 6, 7, 8}
__m128i onesVector = _mm_set1_epi32(1); // Four 32-bit ints all equal 1
__m128i result = _mm_add_epi32(vector, onesVector); // Adding onesVector to first half of vector
_mm_storeu_si128((__m128i*) arr, result); // Store the results back in arr

c. Add second half of array to first half --> arr = {1 + 5, 2 + 6, 3 + 7, 4 + 8, 5, 6, 7, 8}
__m128i result = _mm_add_epi32(_mm_loadu_si128((__m128i*) (arr + 4)), vector);
_mm_storeu_si128((__m128i*) arr, result);

d. Set every element of array not equal to 5 to 0 --> arr = {0, 0, 0, 0, 5, 0, 0, 0}
__m128i fives = _mm_set1_epi32(5);
__m128i mask = _mm_cmpeq_epi32(vector, fives);
__m128i result = _mm_and_si128(mask, vector);
_mm_storeu_si128((__m128i*) arr, result);
vector = _mm_loadu_si128((__m128i*) (arr + 4));
mask = _mm_cmpeq_epi32(vector, fives);
result = _mm_and_si128(mask, vector);
_mm_storeu_si128((__m128i*) arr, result);

4. Vectorize the product function

/* Naive Solution */

static int naiveProduct(int n, int *a) {
	int product = 1;
	for (int i = 0; i < n; i++) {
		product *= a[i];
	}
	return product;
}

/* Vectorized Product */

static int vectorizedProduct(int n, int *a) {
	int result[4];
	__m128i prodV = _mm_set1_epi32(1); // Initialize a vector product of 1
	/* Find the vectorized product */
	for (int i = 0; i < n/4 * 4; i += 4) {
		prodV = _mm_mullo_epi32(prodV, (__m128i*) a[i]);
	}
	__mm_storeu_si128((__m128i*) result); // Array contains partial vectorized product
	/* Tail case--Account for n % 4 != 0 */
	for (int i = n/4 * 4; i < n; i++) {
		result[0] *= a[i];
	}
	return result[0] * result[1] * result[2] * result[3];
}


