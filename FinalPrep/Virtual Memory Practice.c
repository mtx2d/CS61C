Virtual Memory Practice Problems

Fall 2017 Question 12
- Spec: Page Size: 4KiB, Virtual Address Space: 1MiB, Physical Address Space: 1 GiB,
  TLB can store 4 entries, fully associative, LRU replacement
  - Each TLB entry contains: valid bit, permission bits, LRU bits, VPN, PPN

- VA: 1 MiB = 2^20 B
- PA: 1 GiB = 2^30 B
- Page Size: 4 KiB = 2^12 B
- VA = VPN + Offset, PA = PPN + Offset, Offset = log(page size)
	- Page Size = 2^12 B --> Offset = 12
	- VPN = 20 - 12 = 8
	- PPN = 30 - 12 = 18
12.1: Bits needed for VPN and offset?
- VPN: 8 bits, Offset: 12 bits

12.2: Bits needed for PPN and offset?
- PPN: 18 bits, offset: 12 bits

12.3
- Preliminary notes: 655356 = 2^16 (number of entries in a, b)
	- Each int is 4 bytes --> contains 2^18 B apiece
- 256 = 2^8 --> Each loop is striding by 2^8 entries in the array
	- a is accessing every 64 entries (2^6--0, 64, 128...)
	- b accesses every 256th entry and the 4 entries around it
		- (255, 254, 253, 252), (511, 510, 509, 508)...
/* Process 0 */
int a[65536];
for (int i = 0; i< 65536; i += 256) {
	a[i] = i;
	a[i + 64] = i + 64;
	a[i + 128] = i + 128;
	a[i + 192] = i + 192;
}

/* Process 1 */
int b[65536];
for (int j = 0; j < 65536; j += 256) {
	int x = j + 256;
	b[x-1] = j;
	b[x-2] = j + 2;
	b[x-3] = j + 2;
	n[x-4] = j + 3;
}

Time = 0
- Process 0: a[i] = i;
- Process 1: N/A
Time = 1
- Process 0: a[i+64] = i + 64;
- Process 1: N/A 
Time = 2
- Process 0: N/A
- Process 1: int x = j + 256;
Time = 3
- Process 0: N/A
- Process 1: b[x-1] = j;
Time = 4
- Process 0: N/A
- Process 1: b[x-2] = j + 1;
Time = 5
- Process 0: a[i+128] = i + 128;
- Process 1: N/A
Time = 6
- Process 0: a[i+192] = i + 192;
- Process 1: N/A
Time = 7
- Process 0: N/A
- Process 1: b[x-3] = j + 2;
Time = 8
- Process 0: N/A
- Process 1: b[x-4] = j + 3;

What is the TLB hit rate assuming the TLB starts out cold?
- Assume i, j, x are stored in registers and do not require memory access
- TLB must be flushed when switching from one process to another
- Only consider data accesses (ignore effect of fetching instructions)

ANALYSIS
- Access a[0] --> brings in VPN translation for a[64] which is next access
	- This needs to be trashed when accessing b[x-1]
- Access b[x-1] --> brings in b[x-2]
	- Trash to access a[i+128]
- Access a[i+128] --> brings in a[192] which is next access
- This pattern results in a hit rate of 50%

Tagged TLB Entry contains: valid bit, permission bits, LRU bits, ASID, VPN, PPN
- Address space ID (ASID) uniquely identifies virtual address space of a process
- Allows us to not have to flush TLB between processes

12.4: Calculate hit rate for tagged TLB if it starts out cold
- We know that the TLB can store 4 entries (which does not need to be trashed immediately)
- Access a[0] (which is a MISS --> bring in page0 of array for process 0)
	- This contains 2^12 bytes --> 2^10 ints (striding by 2^8, but accessing 4 ints per access)
		- a[64], a[128]... a[2^10-64] are all accessed w/o issue
		- Once we reach a[2^10] which is at byte 2^10 --> we need to pull in new page
		- For every initial miss, we have 15 hits --> a access has a hit rate of 15/16
- Access b[255] which is a MISS --> bring in page1 of array b for process 1
	- Once again, this is 2^12 bytes --> 2^10 ints (b[0, 2^10-1]) are all accessible
	- The next time we need to pull in a new page is after 4 iterations of the 4 loop (striding by 2^8)
	- Thus, hit rate yet again is 15/16

12.5: How many entries can the TLB have to maintain the hit rate in 12.4?
- To maintain a hit rate of 15/16, we just need to be able to keep at least 1 entry for each array
- We never go back to prior entry --> do not need more than 2 entries


Spring 2018: Problem 12

12.1: Think of main memory as cache for a disk. Machine has following spec: 64 bit address,
	  16 KiB pages, 4 way fully associative TLB, 8B words

a. Associativity?
- TLB is analogous to our cache layout --> We want fully associative
b. Block size?
- Pages are analogous to the blocks w/in the cache --> 16 KiB in size
c. Address layout for tag, index, offset
- Total address length = 64 bits
- Block Size = 16 KiB = 2^14 --> 14 bits for offset: Offset is [0:13]
- We are using a fully associative cache, so no bits are needed for the index
- Hence, all the remaining bits go to the tag: Tag is [14:63]

d. Write policy?
- We want write back
	- Write through: data updated? --> write to cache and memory
	- Write back: data updated? --> write to cache only
		- In our analogy, write back is updating memory anyway
		- Write through would therefore be redundant

e. Allocate Policy?
- We want write allocate
	- Write allocate: following write miss, block is loaded

Write Through, Write Allocate
- Hit? --> Write to cache & main memory
- Miss? Update block in main memory --> pull that block into cache
	- Does not make sense b/c next hit will write to memory b/c of write through

Write Through, No Write Allocate
- Hit? Write to cache and main memory
- Miss? Update block in main memory, but do not update cache
	- Time is saved by not bringing block to cache b/c it would be useless anyhow

Write Back, Write Allocate
- Hit? Write to cache, set dirty bit of block, do NOT update main memory
- Miss? Update block in main memory and drag it into cache
	- In the future, writing to this block will hit the cache & set dirty bit

Write Back, No Write Allocate
- Hit? Write to cache, set dirty bit, do not update main memory
- Miss? Update block in main memory & do NOT drag block into cache
	- Writign to that same block will result in misses if block caused a miss --> inefficient

12.2
# define iter 10 * 1024 * 1024
# define T ??? // See below
int mysterySum(int *arr) {
	int i = 0;
	int sum = 0;
	for(; i < iter/2; i++) {
		int p = (i % T) * 4096;
		int b = i % 4096;
		sum += arr[p + b];
	}
	for (; i < iter; i++) {
		int p = (i % T) * 4096;
		int b = i % 4096;
		sum += arr[p + b];
	}
	return sum;
}


Specification: 64 bit address, 4 KiB pages, 1 MiB FAC w/ 64 B blocks, 2 entry fully associative TLB,
4 level page table w/ 8 B entries, LRU for paging to disk, 4 byte words, 4 GiB of main memory
	- Page Size = 2^12 B
	- Cache Size = 2^20 B, Block Size = 2^6 B
	- Memory Size = 2^32 B
	- iter = 2^10 * 2^10 * 10 = 5 * 2^21 --> Each occupies 5 * 2^20

- The value of T changes our stride
- T = 1 --> arr[0], arr[1], arr[2]...
	- 2nd for loop: same access pattern as above b/c of the modulo
- T = 2 --> arr[0], arr[4096+1], arr[1], arr[4096+3]...
- T = 3 --> arr[0], arr[4096+1], arr[4096*2+2], arr[3]...
- T = 4 --> arr[0], arr[4096+1], arr[4096*2+2], arr[4096*3+3]...

a. T = 1, 2, 3, 4: Compare the efficiency of each in the context of the second for loop assuming fist has run
- 3 = 4 < 1 = 2: we see the same performance for 1 & 2, 3 & 4
- Meanwhile, 3&4 both do worse than 1 & 2 (1 and 2 are faster)


b. What can we change to maximize performance for T = 27?
- TLB capacity has the biggest impact
- By increasing TLB capacity, we are able to hold more entries
	- We are better able to cope with accesses that are far apart (2^12)


c. Find VPN, PPN, Physical Address, and label TLB Hit/Page Table Hit/Page FAult

0x10
- VA = 0001 0000 --> VPN = 0001, L1 = 00, L2 = 01, offset = 0000 (0x0)
- TLB empty --> go to page table
	- Entry at index 0 --> 0x00 --> M[0x00] = 0x20 (which provides where to start looking for L2)
	- Offset of 1 from 0x20 --> Look at M[0x24] --> 0x12 --> PPN
	- PA = PPN + (offset) --> 0x120
	- Update TLB: VPN of 0x1 --> PPN of 0x12

0x5C
- VA = 0101 1100 --> VPN = 0101, L1 = 01, L2 = 01, offset = 1100 (0xC)
- TLB does not contain our VPN --> go to page tbale
	- Entry at 01 --> Address of 0x04 (which is empty) --> page fault

0x39
- VA = 0011 1001 --> VPN = 0011, L1 = 00, L2 = 11, offset = 1001 (0x9)
- TLB does not contain this VPN --> go to page table
	- Entry at M[0x00] --> 0x20 (provides starting location for L2)
	- Offset of 3 from entry 0x20 --> M[0x2C] --> 0x5C --> PPN
	- PA = PPN + (offset) --> 0x5C9
	- Update TLB: VPN of 3 --> PPN of 0x5C

0x1F
- VA = 0001 1111 --> VPN = 0001, L1 = 00, L2 = 01, offset = 1111 (0xF)
- TLB has previously encountered this VPN --> TLB hit!
	- VPN of 0001 maps to a PPN of 0x12
	- Combine w/ offset to obtain physical address: 0x12F

