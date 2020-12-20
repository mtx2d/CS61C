Lecture 29: Virtual Memory I

Virtual Memory Concepts
- Virtual memory: level in memory hierarchy beyond the caches. Used to provide illusion of large main memory
	- Organized w/ paged in DRAM & disk (contains memory that are not used as often)
	- Allows for programs larger than DRAM to be run
	- Enables OS to share memory (programs protected from one another) since each process has "independent" memory
		- Do not want different processes to interfere with one another
- Memory Hierarchy: CPU/Registers --> Caches --> Physical Memory/RAM --> Virtual Memory
- Programs treat all addresses from 0 to 0xffffffff as accessible, so there needs to be a mapping
	- Memory Manager: a way to map a vritual address to a physical address in memory
- Address space: set of addresses for all available memory locations
	- Virtual Address Space: addresses user program is aware of
	- Physical Address Space: addresses that correspond to a physical location in memory, which user
	  applications cannot view
	- Do not necessarily need the same number of addresses for virtual/physical address space
- Library analogy: book title = virtual address (common among dif versions), book ID = physical address (single unique ID), 
  card catalog = page table (map of book title to book ID), book availability = valid bit (do we need to fetch from main memory),
  library hours = access rights (who is able to do what with the data?)
- Memory Hierarchy Requirements: want to be able to run multiple processes simultaneously using the same memory
  but w/o reading/writing from one another
  	- Each program should be able to treat its memory as private (others cannot read/write to it)

Physical Memory and Storage
- DRAM: contains DRAM bit cells that store bits of information (optimized for the bit cells)
	- DRAM cells are smaller, denser--contains more caches per area compared to SRAM
- DRAM performance: latency for first access ~10ns, each success access: 0.5-1ns, access 64 bits at a time
- Storage (aka Disks) fall into 2 categories: Solid State Disks/Hard Drive Disks
	- Both these forms are non-volatile, so their values do not change/reset w/o power access
	- SSD Performance: 40-100 nanoS access, $0.05-0.5/GB
	- HDD Performance: <5-10 ms access, $0.01-0.1/GB
- SSD: created w/ transistors, does not utilize mechanical mechanisms to "turn", slower than register/DRAM,
  fast access tp every location, read/write in blocks not bytes

Memory Manager
- Assuming no OS addresses from loads/stored use actual physical addresses
	- Any process can access any address (leading to potential conflicts-R/W from "bad" part of memory)
- To avoid conflicts, each process should use a virtual memory space, and before accessing the memory,
  check to make sure section of memory it is trying to access is indeed available
- Memory Manager: maps each process to a section of memory (each process really only using a specific portion in reality)
	- 3 tasks: map virtual --> physical & provide protection (isolation of memory), swap memory to disk 

Paged Memory
- 3 roles of the memory manager: translation, protection, swapping data from DRAM to disk
	- Paged memory: fixed amount of memory (4KiB+) that is moved b/w DRAM and disk
	- Since 12 bits needed to address 4KiB --> virtual address of 32 bits has 12 bits dedicated for the
	  offset & 20 bits used for the page number (2^20 pages possible)
- Each process is assigned a page table that corresponds to physical addresses in memory
	- Each chunk in DRAM do not need to be located consecutively (OS chooses the allocation)
- Memory manager uses the page table entry to find a corresopnding physical address (offset stays the same)
- OS tracks which process is in action (choose its age table) --> memory manager finds page number from virtual addr
	- Then, mem manager looks up page addr in page table --> Physical memory location = Page addr + offset of virtual addr
- Protection: dif pages in DRAM prevents accessing others memories
	- Also allows for sharing b/w memory by assigning same physical page to multiple processes
- Contains a bit that ensures write protection if needed
- Page table size: for 32 bit VA w/ 4 KiB pages --> 4 * 2^20 bytes = 4 MiB (too large for cache)
	- Page tables exist in DRAM... loads and stores require 2 memory accesses (first trip needed to fetch page table)
	- To optimize performance: frequently used page table entries stored in cache

Page Faults
- Caches usually deal w/ blocks (~64B), VM deal w/ pages (~4KiB)
- Ex: 16 KiB DRAM --> contains 4 4KiB pages in VM, 128 Byte blocks for caches, 4B words 
	- Each page has 2^12/128 = 32 blocks, each block has 32 words, each word as 4 bytes
- Each process works with its own page table, where each entry corresponds to somewhere in DRAM or the disk
- Valid page table entry memory reference: valid entry in DRAM? Can directly read/write data. Valid entry on disk?
  Need to allocate page in DRAM (boot another page out of DRAM if out of memory, put booted page on disk, & read
  that page from disk back into memory). Then, after all that, can read/write data
- Invalid page table entry: need to allocate new page in DRAM, proceed with eviction process described above if out
  of memory, then R/W data
 - If valid but on disk or invalid, this is a page fault (requires OS intervention)
 - Page faults are a form of exceptions: require a handler that deals w/ updating page table/transfers/status bits
 	- Instruction re-executed after page fault




