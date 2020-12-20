Fall 2019 Final

Question 1: CALL: Role of compiler, assembler, linker, or loader?


a. Copying program from disk to physical memory
- Loader: the expressed function is to load the program into physical memory

b. Remove pseudoinstructions
- Assembler is in charge of removing pseudoinstructions
- Assembler receives the assembly code from the compiler

c. Determine increment size for pointer arithmetic
- Compiler does the dirty work of converting the original program code into assembly language

d. Incorporate statically-linked libraries (SLL
- The task of the linker is to combine all the object files and make them into a single executable
	- Need to add in the SLLs during this phase

e. Incorporate dynamically-linked libraries (DLL)
- Dynamically linked libraries can only be incorporated at runtime
	- Hence, it is the job of the loader



Question 2: Number Representations for 0xFF000003

a. Interpreted as an array of unsigned 8 bit numbers (assume big endian)
- Big Endian means the most significant byte gets stored in the lowest memory location
- Conversion to 8 bits: 0: 255, 1: 0, 2: 0, 3: 3
--> [255, 0, 0, 3]

b. Interpreted as IEEE-754 single precision floating point number
- 1 sign bit, 8 exponent bits, 23 signifcand bits
- Binary representation: 1111 1111 0000 0000 0000 0000 0000 0011
- Sign: 1, Exponent: 111 1111 0, Significand: 000 0000 0000 0000 0000 0011
- Sign: 1, Exponent: 254, Significand: 3
- Result = (-1)^1 * 2^(254-127) * 1.000 0000 0000 0000 0000 0011
--> -1 * 2^127 * 1.00000000000000000000011
--> -1 * 2^127 * (1 - 2^-22 - 2^-23)

c. Interpreted as a RISC-V Instruction
- OPCODE by extracting lowest 7 bits: 000 0011 --> load instruction
- Format: 20-31: offset, 15-19: rs1, 12-14: funct3, 7-11: rd, 0-6: opcode = LOAD
- opcode: 000 0011, rd: 00000, funct3: 000, rs1: 00000, offset: 1111 1111 0000
	- funct of 000 translate to: lb
	- rs1 of 0 translates to x0, rd of 0 translates to x0
	- Offset of 1111 1111 0000 becomes -1 * (10000) = -16
--> lb x0, -16(x0)

d. Interpreted as uint32_t* variable c which is in little-endian, when calling printf((char*) &c)
- We know that c corresponds to a 32 bit value --> &c yields the string to be printed
- Because we are working in little endian, we read the bytes from right to left
	- Proper ordering: 03, 00, 00, 0xFF
	- We read 03 --> ETX, then we read 00 (which corresponds to NULL)
	- When we reach 00, printf interprets this as the nul terminator and is finished

Question 8: Caches
Specs: 1 KiB L1 (DMC) Cache w/ 16 B blocks, 4 GiB memory, integer is 4 B

#define LEN 2048
int arr[len];
int main() {
	for (int i = 0; i < LEN - 256; i += 256) {
		arr[i] = arr[i] + arr[i+1] + arr[i+256];
		arr[i] = arr[i] + 10;
	}
}

Numerical Conversions
- Cache Size: 2^10 B, Block Size: 2^4 B, Memory: 2^32 B, Int: 2^2 B
- length of array: 2^11 (of 2^2 B ints) --> total size: 2^13
- Striding by 2^8, accessing arr[i], arr[i+1], arr[i+256], arr[i] each time
- There are 2^11/2^8 - 1 runs = 7 iterations of the for loop


a. Calculate T/I/O bits
- Offset bits (using the block size): 4
- Index bits (because it is a DMC): log(2^10/2^4) = 6
- Tag bits = 32 - (4+6) = 22

b. What is the hit rate?
- On the first round of accesses: arr[i] is a MISS, arr[i+1] is a HIT, arr[256] is a MISS
  b/c block size too small to contain it, arr[i] needs to loaded back to write to it (MISS)
  - In the 2nd line, we already have arr[i] in memory, so the 2 accessses are both HITS
- On the second round of accesses, we have to reload arr[i+256] back into the cache and repeat
- For every iteration of the for loop, there is exactly the same access pattern
- Hit Rate = MHMMHH = 50%

c. Add in an L2 cache. 3 cycles per access in L1, 50 cycles per access in L2. L1 Hit Rate: 25%,
   L2 hit rate: 90%, 500 cycles per physical memory access. Average memory access time?
- Formula: Access Time + Miss Rate * Miss Penalty
- L1Access + L1Miss * L1Penalty = 3 + 0.75 * (50 + 0.1 * 500) = 3 + 0.75 * (50 + 50) = 3 + 0.75 * 100 = 78
- Thus, the average memory access time is 78 cycles


Question 9: Virtual Memory
- Specs: 32 TiB VA Space, Page Size: 256 KiB, TLB Miss Rate: 0.2, Page Fault Rate: 0.002, Access Time to TLB: 5 cycles,
  transfer time from page to disk: 10^6 cycles, PA Space: 4 GiB, Access time to PA Space: 500 cycles, L1 physically indexed
  w/ tagged cache taking 5 cycles to access and hit rate of 50%. TLB miss? Check physical memory

Numerical Conversions
- VA Space: 2^45 B, Page Size: 2^18 B, PA Space: 2^32 B

a. Number VPN bits
- Formula: VA Space = VPN + Offset
- Offset is determined by the number of page size bits = 18
- 45 = 18 + VPN bits --> VPN bits = 27

b. Total size of page table (in bits)
- PA = PPN + Offset --> 32 = 18 + PPN --> PPN = 14
- Thus, for each page table entry, we need 14 bits to rach each PPN
- We need to be able to map to each entry from all possible virtual page numbers
	- We need 2^27 entries in page table to be able to accommodate each VPN
	- Each VPN must be 14 bits long --> 14 * 2^27


c. Average memory access time for current process
- We need the sum of the average translation time and data access time
	- Translation performed by the TLB. If miss, go to physical mmeory
	- Data access performed by cache 
- Key Formula: AMAT = Access + MR * MP

Calculation for Translation
AMAT = 5 + 0.2 * (500 + 0.002 * 10^6) = 5 + 0.2*(500 + 2000) = 5 + 500 = 505
	- TLB lookup automatically takes 5 cycles
	- 0.2 probability of a miss --> We go to checking physical memory
	- It automatically takes 500 cycles to access physical address space
	- Once there, we have a 0.002 chance of page faulting and needing to go to the disk
	  which has a miss penalty of 10^6 cycles


Calculation for Data Access
AMAT = 5 + 0.5*(500) = 5 + 250 = 255

Total Access Time: 505 + 255 = 760


d. What do we need to do when switching to a different process?
- When switching to a different process, we need to make sure that the old TLB info that is needed
  is wiped clean, make sure that the page table base register is saved, any other user registers are stored
  properly, invalidate current entries in TLB (Lecture 30)


	i. Update page table address register
	- Yes: see above
	ii. Evict pages from previous process from RAM
	- No: there is no need to alter the actual pages
	iii. Clear TLB dirty bits
	- No: if we invalidate the TLB entry bit, there is no need to clear the dirty bit (it is irrelevant)
	iv. Clear cache valid bits
	- No: there is nothing inherently wrong with the cache when switching processes
	v. Clear TLB valid bits
	- Yes: the entries in the TLB are no longer valid




