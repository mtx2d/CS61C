1. Given: 0xFA000003

a. Array of unsigned, 8 bit numbers, assuming little endian
- Little Endian: we read the values from right to left
- 03, 00, 00, 0xFA --> A = {3, 0, 0, 250}

b. IEEE-754 floating point number w/ 7 bit exponent (bias = -64)
- Convert to binary: 1111 1010 0000 0000 0000 0000 0000 0011
- Divided into sign, exponent, significand: 1 [111 1010] [0000 0000 0000 0000 0000 0011]
- Sign: 1, Exponent < 127, Significand: 3
- Since the exponent is less than 127, we are dealing w/ an ordinary number (not denorm/infinite/NaN values)
- (-1)^1 * (111 1010 - 64) * 1.000...11 = -1 * (011 1010) * 1.000...11 = -1.000000000000000000000011 * 2^58

c. RISC-V instruction
- Convert to binary: 1111 1010 0000 0000 0000 0000 0000 0011
- OPCODE: 000 0011 --> load instruction
- Format: 20-31: offset, 15-19: rs1, 12-14: funct3, 7-11: rd, 0-6: opcode = LOAD
- Divided into format: [1111 1010 0000] [00000] [000] [00000] [000 0011]
	- Immediate: 1111 1010 0000 --> -1 * (0000 0101 1111 + 1) --> -1 * (110 0000) = -96
	- RS1: 0x00 --> x0
	- funct3: 0x0 --> lb
	- RD: 0x00 --> x0
	- OPCODE: 0x03 --> load instruction
- Final answer: lb x0, -96(x0)

d. (uint32_t *) c in big endian after calling printf("%i", (int)((uint8_t*) &c)[0])
- First we interpret &c as an unsigned 8 bit integer (only taking the first element)
- Then it is cast to an int type to be printed
- Assuming big endian, that means we draw from the most significant 2 bits --> 0xFA
- Therefore, we print: 250

3. Parallelism
Specification: Have access to multiple resources. Pick the one that is most beneficial to the problem at hand
	1. 12 core processor, 3 GHz MIMD computation
	2. SIMD computational engine, 10 teraflop floating point computation
	3. Map/Reduce cluster

Main Ideas
- Intel Intrinsics (SIMD instructions) are good for smaller computations--a form of data parallelization
	- Reasoning: OpenMP has overheard, which inevitably takes time
- For mid-sized computations, we want to use OpenMP (thread parallelism) and SIMD instructions (data parallelism)
- For massive computational tasks, we want to use distributed computing, such as MapReduce
	- When data cannot fit on the memory of one machine, it could benefti from distributued computing

a. 32 floating point matrix multiplication of 100M entry matrices w/ transposed matrix
- SIMD computation b/c matrices are transposed, enabling for division of tasks into SIMD
- Cannot use MIMD level parallelism

b. Finding all references to an Internet post made (assume 5 PB of data)
- This would benefit from Map/Reduce as it would require multiple machines to properly execute

c. Run program written on 40 threads communicating via queues/channels
- The presence of multiple threads clues us into the fact that MIMD parallelism can be used
	- We in fact can take advantage of thread level parallelism

d. 32 B floating point matrix addition of 100M entry matrices
- We would not benefit from significant memory speedup of any of the choices

6. C Memory Model

void foo() {
	int u = 4;
	int* v = malloc(100 * sizeof(u));
	int** w = &v;
}

a. What type does &u evaluate to?
- u is declared as a local variable
- Hence, &u evaluates to the address of u, which lives in STACK space

b. What type does w evaluate to?
- v is a pointer to malloced space
- w is a pointer that points to the address of v
- w therefore evaluates to the pointer v, which exists in STACK space


c. What does sizeof(*v) evaluate to on 32 bit system?
- v is a pointer to a series of 64 bit ints (8 B)
- Thus, dereferencing v leaves us with the first of those 64 bit ints --> 8

d. What does sizeof(v) evaluate to on 32 bit system?
- v is simply a pointer --> sizeof(v) = 4
- Size of the system determines the size of the pointer

e. What type of value does *w evaluate to?
- w is a pointer to the address of v
- v is a pointer to malloced space that lives in stack space
- *w is equivalent to v (a pointer), which when evaluated, yields the memory in HEAP space
  which was malloced

f. What type of value does (v + 1) evaluate to?
- v is a pointer to heap space
- Incrementing the pointer would still leave an incremented pointer
- Thus, when we evaluates (v + 1) --> we get the data in the HEAP space

g. What type does value *(v + 1) evaluate to?
- (v + 1) increments the pointer which points to heap space
- *(v + 1) yields the data in heap space --> if we try to evaluate that further, it is nonsensical

7. C Coding

struct BloomFilter {
	uint32_t size;
	uint16_t itercount;
	uint64_t (*) (void *data, uint16_t iter) hash;
	uint8_t *data;
}


a. Assume 32 bit architecture, word aligned. Find: sizeof(struct bloomFilter)
- We have 4 components: size, itercount, hash, and data
	- size is 32 bits (4 B), itercount is 16 bits (2 B), hash/data are pointers (4 B each)
	- In total we would ordinarily need 4 + 2 + 2 * 4 = 14 bytes
	- However, because each needs to be word aligned, itercount requires 4 B (must take ceil(numBytes needed))
	- Hence, the real total is: 4 + 4 + 2 * 4 = 16 Bytes

b. Complete the function

void insert(struct BloomFilter *b, void *element) {
	uint64_t bitnum;
	int i;
	for (i = 0; i < __b->itercount_, ++i) { // We want to iterate through all of b's itercount
		bitnum = __b->hash(element, (uint16_t) i) % b->size__; // Hash the element for each of the possible buckets this instance of BloomFilter has
		b->data[bitnum >> 3] = __b->data[bitnum >> 3] | (1 << (bitnum & 0x7))__; // Maintaining current bit hash or resolving if needed
	}
}
c. Translate the following to RISC-V. Put ret into s0

struct BloomFilter *alloc(
	uint64_t (*) (void *data, uint16_t iter) hash
	uint16_t itercount, 
	uint32_t size) 
{
	struct BloomFilter *ret = malloc(64);
	ret->size = size;
	ret->data = calloc(size >> 3, 1);
	ret->hash = hash;
	ret->itercount = itercount;
}

alloc: # Prolog
	__addi sp, sp, -20__ # Allocate space on the stack
	# Prologue: we save previous values of ra, s0, a0, a1, and a2
	sw ra 0(sp)
	sw s0 4(sp)
	sw a0 8(sp) # Holds hash function
	sw a1 12(sp) # Holds size
	sw a2 16(sp) # Holds itercount
# body
addi __a0, x0, 64__ # Allocate space for 64 bytes
	jal malloc
	mv s0 a0 # put ret in s0 # s0 now holds the start of the allocated space
	__lw t0, 12(sp)__ # load size into t0
	__sw t0, 0(s0)__ # store it
	__srli a0, t0, 3__ # div size by 8 with a shift
	li a1 1
	jal calloc
	sw a0 12(s0) # store data
	__lw t0, 8(sp)__ # load hash to t0 (4 unsigned bytes)
	__sw t0, 8(s0)__ # store it: Use the right type!
	__lhu t0, 16(sp)__ # load itercount to t0--2 unsigned bytes (halfword)
	__sh t0, 4(s0)__ # store it: Use the right type!
	mv a0 s0 # Result from a0 now in s0
# epilog
	lw ra 0(sp)
	_lw s0, 4(sp)_ # We want to restore the original value of s0
	__addi sp, sp, 20__ # Restore the stack
	jr ra

d. CALL practice: do they need relocation?

	i. PC-relative addressing
	- PC relative addressing does not use relocation
		- We do not need relocation b/c the code is position independent
		- We do, however, need relocation for the other 3 forms of addressing
			- This includes: absolute function, external function, and static data reference
	ii. Static Data Reference
	- Static data references require relocation
		- This is simply a form of addressing that needs relocation (see lecture 13)


e. What are the tasks of the assembler?
- The assembler is responsible for producing machine language and pseudocode replacement
- The compiler is in charge of parsing the C code and translating it into assembly code (can have pseudoinstructions)
- The executable code is actually created by the linker

f. What is the benefits to dynamically linked libaries?
- Saves resources by sharing data (minimize cost for disk space/physical memory)
- Can upgrade easily by only having to replace DLL file instead of needing to recompile
- Multi-language programming: DLLs can be written in different languages
- System independence: we can treat a DLL as an abstraction from the main program. Does not
  only work on any single system

g. What are assembler directives?
- Assembler directives: Directions given to the assembler that do not produce machine instructions



