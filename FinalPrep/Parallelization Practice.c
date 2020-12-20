
Summer 2018 Final Q9

Compute the inner product between SM, an MxN matrix, and SV,
a Nx1 Vector. Result stored in D: a Mx1 vector

void innerProduct(double *d, double *sm, double *sv, unsigned m, unsigned n) {
	__m256d matrix_part;
	__m256d vector_part;
	__m256d total;
	double arr[4];
	for (int i = 0; i < m; i++) {
		total = _mm256_setzero_pd(); // Initialize a total of 0
		for (int j = 0; j < n/4 * 4; j += 4) { // Increment by 4 as typical for SIMD
			/* 
			Below, we make sure that we're starting from an address that's a multiple of 4. 
			*/
			if ((i * n + j) % 4 == 0) { // Check for aligned address 
				/* 
				If it is aligned, we can use our normal load operation, which is optimized when
				compared to the unaligned version
				*/
				matrix_part = _mm256_load_pd(sm + (i * n) + j);
			}
			else {
				/* If we don't have aligned data, we have no choice but to use the unaligned version */
				matrix_part = _mm256_loadu_pd(sm + (i * n) + j);
			}
			vector_part = _mm256_load_pd(sv + j);
			total = _mm256_fmadd_pd(matrix_part, vector_part, total);
		}
		_mm256_store_pd(arr, total);
		d[i] = arr[0] + arr[1] + arr[2] + arr[3];
	}
	
	if (n % 4 != 0) { // Check for tail case--is it divisible by 4?
		for (int i = 0; i < m; i += 1) {
			for (int j = n/4 * 4; j < n; j+= 1) {
				n[i] += *(sm + (i * n) + j) * *(sv + j);
			}
		}
	}

}


Q13: Spark

- Input: [1, 2, 1, 2, 3, 4, 5, 6, 4, 2, 1, 3, 3, 1, 1, 2, 1]

- Output: (1, 6)
- We want to find the mode of the list and how often it occurs
	- In the example above, 1 occurs 6 times --> return (1, 6)
- Have access to: map. flatmap, reduce, reduceByKey

/* This function returns the 1 tagged onto the initial input */
def output_data(val):
	return (val, 1)

After this function, we get an intermediate list: [(1, 1), (2, 1), (1, 1)...]

/* This function is responsible for aggregating the values */
def compute_count(a, b);
	return a + b;

After this function, we have another intermediate list: [(1, 6), (2, 4), (3, 3)...]
	- We have all the keys & how often they each occur

def find_max_occurrence(a, b);
	if a[1] > b[1]:
		return a
	return b

After this function: we return the tuple corresponding to the highest key value


#values = list(numbers)
modeData = sc.parallelize(values)
modeData.map(output_data)
		.reduceByKey(compute_count)
		.reduce(find_max_occurrence)

- reduceByKey applies the function to all elements of the list with the same "key"
	- (1, 1), (1, 1), (1, 1) after compute_count --> (1, 3)
	- (2, 1), (2, 1) --> (2, 1)
- Meanwhile, reduce just takes the entirety of all the elements and spits out a single value
	-(1, 5), (2, 3), (4, 4), (6, 10), (7, 3) if we just look at the 2nd number in the tuple max --> return (6, 10)






