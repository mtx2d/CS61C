Fall 2017 Final

Question 1

a. What is wrong with the following ode?

int floorDivByTwo(int m) {
	return m >> 1;
}

Does not work for negative numbers
- When we shift the bits left by one, it removes the sign bit and replaces it w/ 0
	- This gets interpreted as a positive number (one floor div by 2)
	- We get the incorrect result for negative numbers

b. Complete the function to calculate if a given integer is a power of 16
   Can use ==, !=, and up to 1 subtraction. Constants allowed, no new variables

10011100
11001111
11000000
00100000

// Assume sizeof(int) == 4
int pow16(int m) {
	// Check sign bit & number of 1-bits in binary representation
	if (__(m >> 31 & 1) __ != ___0__ || ___(m & (m-1)) != 0___) {
		/*
			(m >> 31 & 1 != 0) makes sure sign bit isn't one (must be positive)
			(m & (m-1) != 0) ensures that the number of ones equals 1
		*/
		return 0;
	}
	else {
		/*
		At this point, we know that m is a positive integer and a power of 2
		We still need to check to make sure that it is indeed a power of 16
		*/
		/*
		0000 0000 0000 0000 0000 0000 0001 0000
		0001 0000 0000 0000 0000 0001 0000 0000
		0000 0000 0000 0000 0001 0000 0000 0000
		0000 0000 0000 0001 0000 0000 0000 0000
		- Pattern: we observe that the valid representations are 0x10, 0x100...
		*/
		return __(m & 0x11111110) != 0__;
	}
}


Question 2: MapReduce

Return review with highest word count per star label using Spark

Sample Input (reviewID, numStars, text):
rev1 5 great!
rev2 3 decent, would go again
rev3 1 terrible service

Sample output: (numStars, (reviewID, numWords))
(5, (rev1, 1))
(3, (rev2, 4))
(1, (rev3, 2))

def parseLine(line):
	tokens = line.split(" ") // Split input into list: reviewID, numStars, text
	text = tokens[2:] // 2nd element of the list corresponds to the review's text
	return (___tokens[1]__, (___tokens[0]__, __len(text)___)) //

def findMax(val1, val2):
	// Takes in one of the sample output entries in
	if ___val1[0]__ > __val2[0]___:
		return val1
	else:
		return val2

if __name__ == '__main__':
	sc = SparkContext()
	reviews = sc.textFile("data.txt")
	// We want to map the parseLine function onto all possible entries in original dataset
	// ReduceByKey to only return a single entry
	result = reviews.__map___(parseLine).___reduceByKey__(findMax)

Question 3: Create a minheap using create, insert, and freeHeap operations

char* arr[] = {"d", "b", "c", "ae", "a"};
minHeap transformation: ["a", "ae", "c", "d", "b"]
- The basic idea is that the child is greater than or equal to the parent


/* minHeap struct */
#define INIT_CAP 10
struct minHeap {
	char** data;
	size_t size; // Current number of elements
	size_t capacity; // Max elements heap can hold
}

/* Create Function */
struct minHeap* create(char** arr, size_t, size) {
	struct minHeap* m = malloc(sizeof(struct minHeap)); // Malloc space for new minheap
	m->size = m->capacity = 0; // Initialize the sizes to 0
	for (size_t i  = 0; i < size; i++) {
		insert(m, arr[i]); // For each string in the array, insert it into the minHeap
	}
	return m;
}

/* FreeHeap function */
- We know that we want 

#define INIT_CAP 10
#define RESIZE_FACTOR 2
void freeHeap(struct minHeap* m) {
	for (size_t i = 0; __i < m->size___; __i++___) {
		__free(m->data[i]);___ // Free the elements in each array of chars
	}
		___free(m->data);__ // Free the space allocated for the array of chars
		___free(m);__ // Free the original poiner
	
}

/* Insert function */
- We insert at back of array, then bubble up as needed
void insert(struct minHeap* m; char* elem) {
	if (m->capacity == 0) {
		m->data = ___malloc(INIT_CAP * sizeof(char*))__; // Allocating space for data
		m->capacity = INIT_CAP;
	}
	else if (m->size >= __m->capacity___) { // Is resize needed?
		m->data = ___realloc(m->data, m->size * 2)__; // Use realloc function to accommodate existing data
		__m->capacity *= RESIZE_FACTOR___; // Our capacity has icnreased b/c of allocation for more data
	}
	// We can't directly copy elem over b/c we want a permanent (not stack) pointer
	char* temp = __malloc((strlen(elem) + 1) * sizeof(char))___; // Allocate space for a new string

	___strcpy(temp, elem, (strlen(elem) + 1));__ // Copy chars from elem into temp
	int iter = m->size++;
	/* This bubbles up inserted element to correct position in heap */
	while (iter = 0 && strcmp(temp, m->data[(iter-1)/2] < 0)) {
		m->data[iter] = m->data[(iter-1)/2];
		iter = (iter - 1)/2;
	}
	m->data[iter] = __temp___; // We need to insert temp back into its new location
}

4. RISC-V
- ecall numbers: 1 = print integer, 4 = print string, 10 = exit
	- ecall uses value in a1 as function argument
- read_input: provide a 32 bit integer stored in a0
1. .data
2. Boom: .asciiz "Ayy, man's not dumb." # strlen(this string) == 20
3. Skraa: .asciiz "The ting goes skkkraaa." # strlen(this string) == 23
4.
5. .text
6. MAGIC: # prologue
7. 			la s0, Risc-tery
8. 			la s1, Boom
9. 			addi s2, x0, 0x61C
10. Get: jal read_input # provide either 0 or 1 (USER_IN_1)
11. 		beq a0, x0, Default
12. Risc-tery: jal read_input # provide any integer (USER_IN_2)
13. 		beq a0, x0, QuickMaths # Q2
14. 		addi t0, x0, 9
15. 		slli t0, t0, 2
16. 		add s0, s0, t0
17. 		lw t1, 0(s0)
18. 		slli a0, a0, 20 # shift user input by 20
19. 		add t1, t1, a0
20. 		sw t1, 0(s0)
21. QuickMaths: addi a1, s1, 0
22. 		addi a0, x0, 4
23. 		ecall
24. 		j Done
25. Default: addi a0, x0, 1
26. 		add a1, s2, x0
27. 		ecall
28. Done: # epilogue
29. 		jalr ra

a. What registers should be saved in the prologue and restored in the epilogue?
- Save according to caller/callee convention: s0, s1, s2, ra
	- ra is in weird territory where it is just safer to save it universally


b. Convert the instruction beq a0, x0, QuickMaths into machine code
-B format: 31: imm[12], 25-30: imm[10:5], 20-24: rs2, 15-19: rs1, 12-14: funct3, 8-11:imm[4:1], 7:imm[11], 0-6:opcode
- rs2: x0 = 00000
- rs1: a0 = 10 = 01010
- Immediate: jump foward by 8 (but need to account for trailing 0) --> 0000 0001 0000
	- imm[4:0]: 10000
	- imm[11:5]: 0000000
- funct3: using the fact that it is beq --> 0x0 --> 000
- OPCODE: for branch instructions --> 1100011

c. Call MAGIC, providing input 0 for call to read_input in line 10. What gets printed?
- After we call jal read_input, we obtain the result 0 in a0
- On line 11, we branch directly to Default b/c a0 is indeed equal to 0
- a0 is now equal to 1, a1 = s2 (which is equal to 0x61C)
- ecall with parameter 1 prints the integer stored in a1 --> 0x61C gets printed


d. Call MAGIC again, what input values to read_input result in printing "Ayy, man's not dumb"?
- USER_IN_1 = 1 in order to prevent the direct leap to DEFAULT that we saw above
- USER_IN_2 equals 0 guarantees that we branch to QuickMaths, during which we can print the correct value
	- Given any value to read_input, the values of a1 and a0 get reset anyway
	- However, we want the correct length string to be printed
	- Thus, USER_IN_2 = 21 is the only possible value that guarantees this correct behavior









