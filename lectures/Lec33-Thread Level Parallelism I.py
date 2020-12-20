Lecture 33: Thread Level Parallelism I

Parallel Computer Architectures
- Ways to improve performance: increase clock rate, lower CPI, perform multiple tasks simultaneously
	- Higher clock rate: Have reached max clock rate for practical purposes (<5 GHz for general purpose computers)
		- Limited by power issues
	- Lower CPI: achieved w/ SIMD which operates on more data per instruction (instruction level parallelism)
	- Simultaneous multi-task performance: Multiple CPUs executing dif programs; can be related or unrelated tasks
- Parallel data: operating on > 1 data item simultaneously (ex: add 4 pairs of words), parallel instructions: performing > 1
  instruction at the same time (ex: 5 pipelined instructions), parallel threads: dif cores work on dif threads 
- Multicore CPU: 1 datapath in single chip, shared L3 cache, memory (ex: CPU with 2 cores)
	- Each core has own distinct control, datapath (PC, registers, ALU)--2 cores are completely autonomous
	- Shared component b/w the cores is the memory w/ I/O interfaces. Need to deal w/ memory accesses
- Multiprocessor Execution Model: each processor/core executes own instructions
	- Some resources are NOT shared (datapath, highest level caches) and others are shared (Memory-DRAM, 3rd level cache)
	- Multicore processor: single CPU with multiple cores pon it
		- Example: 4 core CPU which can execute 4 dif instruction streams at the same time

Multicore
- Over time, number of transistors (Moores Law), sequential app performance, frequency, and power has steadily increased
	- Issue: eventually power demands grew excessive--could not cool down sufficiently. Clock frequency & power levels off
	  in 2005, so sequential app performance leveled off
	  - Solution: number of cores increased since 2005 --> parallel app performance increased
- Shared memory: every "core" has access to all memory in processor w/ hardware to keep caches consistent
	- Upside: communication in program made easier using shared variables (each core communicates through memory)
		- Example: they could all be writing to the same aray
	- Downside: scales poorly b/c the slower memory shared by many "users" (multiple cores that rely on it)
		- Slow b/c going to memory takes a long time & bottleneck: only one piece of memory (cannot write/read from mem at same time)
- Multiprocessor can be used for job level parallelism or dividing a single task b/w multiple cores
	- Job Level parallelism: each processor works on its own problem w/ no communication b/w each processor
	- Partition of a single task: each processor does some work as a part of a larger operation
- Big Picture: Parallel processing is difficult but the only practial method of increasing performance & lowering energy needs
	- Used extensively in mobile systems & warehouse scale computers
- 2009 to 2021 change in performance: Number of cores increased 8x, SIMD bits/core increased 2.5x --> 20x increase in FLOPS/cycle 

Threads
- Thread: thread of execution, which is a single stream of instructions
	- Each program/process can split (aka fork) into multiple, separate threads that can run at the same time
	- Threads provide a useful conceptual framework for dealing with parallelism
	- Single CPU w/ single core executes >1 thread by using time sharing
	- Time Sharing: spend some time on first thread, pause it, work on second thread, etc. This constant
	  rotation ultimately gets perceived as performing all tasks simultaneously
- Thread: sequential flow of instructions that executes a task
- Threads contain: dedicated program counter (PC), separate registers, access to shared memory
- Physical core provides 1+ hardware thread (actively performing instructions) 
	- Hardware thread: thread running on a specific core (once thread is loaded onto a core, it becomes a hardware thread)
- Operating System multiplexes multiple software threads onto available hardware threads
	- Threads not mapped to hardware threads are in state of waiting
	- Idea: There can be 100s of software threads--all of the created threads (w/ only 4 hardware threads)
- OS Threads: provide illusion of simultaneous active threads by multiplexing software threads onto hardware
  threads, removing software threads from hardware threads, & starting to execute dif software threads when needed
  - Multiplexing software threads onto hardware threads: switch out blocked threads & use a timer
  	- Blocked threads-threads that stalled (might be from cache miss, using user input, etc.)
  	- Timer-might want to switch active threads w/ regular time intervals
  - Removing software threads from hardware threads: interrupt execution & save register/PC to memory 
  - Start executing dif software threads: load its registers onto hardware thread registers & go to the saved PC
- Thread pool: list of threads competing for the attention of the processor
	- OS is repsonsible for mapping threads to cores & scheduling software threads 


Multithreading
- Sample Problem: assume active thread has cache miss --> active thread waits 1k cycles for data from DRAM
	- Can instead switch out this thread and run a dif one until data is available again
	- However, this requires saving thread state & loading new thread which might be even more taxing
	- Solution: take advantage of hardware assisted software multithreading
- Hardware Assisted Software Multithreading Big Idea: 1 core, 2 threads
	- Use 2 PCs, 2 registers, and 1 ALU. From software view: looks like 2 different hardware threads
		- Both threads can be active simultaneously; referred to as hyperthreading
- Simultaneous Multithreading (HT): 1 physical core but multiple logical processors are visible to OS
	- Multiple treads can be run at the same time (each w/ own state-PC, regs but sharing certain resources-caches, ALU)
- Logical threads: + ~1% hardware --> + ~10% performance; Multicore: +50% hardware --> ~2x performance
	- Typically utilize both (multiple cores w/ multiple threads per core)
- Thread: sequence of instructions w/ its own PC & processor state (register file)
- Multicore: model consists of physical CPU, Logical CPU, and hyperthreading 
	- Physical CPU: one thread per CPU, OS switching b/w threads in response to I/O events
	- Logical CPU: thread siwtching in hardware in response to thread blocks (ex: cache miss, memorya access)
	- Hyperthreading (aka simultaneous multithreading-SMT):  uses superscalar architecture for starting instructions
	  on dif threads simultaneously (multiple hardware threads working on one core)

