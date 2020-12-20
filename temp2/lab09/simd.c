#include <time.h>
#include <stdio.h>
#include <x86intrin.h>
#include "simd.h"

long long int sum(int vals[NUM_ELEMS]) {
	clock_t start = clock();

	long long int sum = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS; i++) {
			if(vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	long long int sum = 0;

	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		for(unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			if(vals[i] >= 128) sum += vals[i];
			if(vals[i + 1] >= 128) sum += vals[i + 1];
			if(vals[i + 2] >= 128) sum += vals[i + 2];
			if(vals[i + 3] >= 128) sum += vals[i + 3];
		}

		//This is what we call the TAIL CASE
		//For when NUM_ELEMS isn't a multiple of 4
		//NONTRIVIAL FACT: NUM_ELEMS / 4 * 4 is the largest multiple of 4 less than NUM_ELEMS
		for(unsigned int i = NUM_ELEMS / 4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] >= 128) {
				sum += vals[i];
			}
		}
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return sum;
}

long long int sum_simd(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);		// This is a vector with 127s in it... Why might you need this?
	long long int result = 0;				   // This is where you should put your final result!
	/* DO NOT DO NOT DO NOT DO NOT WRITE ANYTHING ABOVE THIS LINE. */
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* YOUR CODE GOES HERE */
		__m128i sumVector = _mm_setzero_si128();
		int arr[4] = {0, 0, 0, 0};
		for (unsigned int i = 0; i < NUM_ELEMS / 4 * 4; i += 4) {
			//printf("Reached 1\n");
			__m128i temp = _mm_loadu_si128((__m128i*) (vals + i));
			//printf("Reached 2\n");
			__m128i compare = _mm_cmpgt_epi32(temp, _127);
			compare = _mm_and_si128(temp, compare);
			sumVector = _mm_add_epi32(compare, sumVector);
		}
		_mm_storeu_si128((__m128i*) arr, sumVector);
		/* You'll need a tail case. */
		for (unsigned int i = NUM_ELEMS/4 * 4; i < NUM_ELEMS; i++) {
			if (vals[i] > 127) {
				arr[0] += vals[i];
			}
		}
		result += arr[0] + arr[1] + arr[2] + arr[3];
	}
	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}

long long int sum_simd_unrolled(int vals[NUM_ELEMS]) {
	clock_t start = clock();
	__m128i _127 = _mm_set1_epi32(127);
	long long int result = 0;
	for(unsigned int w = 0; w < OUTER_ITERATIONS; w++) {
		/* COPY AND PASTE YOUR sum_simd() HERE */
		/* YOUR CODE GOES HERE */
		__m128i sumVector = _mm_setzero_si128();
		int arr[4] = {0, 0, 0, 0};

		for (unsigned int i = 0; i < NUM_ELEMS / 16 * 16; i+= 16) {
			// Fetch, compare, get new values, add
			__m128i temp = _mm_loadu_si128((__m128i*) (vals + i));
			__m128i compare = _mm_cmpgt_epi32(temp, _127);
			compare = _mm_and_si128(temp, compare);
			sumVector = _mm_add_epi32(compare, sumVector);

			temp = _mm_loadu_si128((__m128i*) (vals + i + 4));
			compare = _mm_cmpgt_epi32(temp, _127);
			compare = _mm_and_si128(temp, compare);
			sumVector = _mm_add_epi32(compare, sumVector);

			temp = _mm_loadu_si128((__m128i*) (vals + i + 8));
			compare = _mm_cmpgt_epi32(temp, _127);
			compare = _mm_and_si128(temp, compare);
			sumVector = _mm_add_epi32(compare, sumVector);

			temp = _mm_loadu_si128((__m128i*) (vals + i + 12));
			compare = _mm_cmpgt_epi32(temp, _127);
			compare = _mm_and_si128(temp, compare);
			sumVector = _mm_add_epi32(compare, sumVector);
		}
		_mm_storeu_si128((__m128i*) arr, sumVector);
		
		/* You'll need 1 or maybe 2 tail cases here. */
		for (unsigned int i = NUM_ELEMS/16 * 16; i < NUM_ELEMS; i++) {
			if (vals[i] > 127) {
				arr[0] += vals[i];
			}
		}

		result += arr[0] + arr[1] + arr[2] + arr[3];	

	}

	clock_t end = clock();
	printf("Time taken: %Lf s\n", (long double)(end - start) / CLOCKS_PER_SEC);
	return result;
}