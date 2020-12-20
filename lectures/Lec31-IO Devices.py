Lecture 31: I/O Devices

I/O Devices
- I/O interface provides mechanism for program on CPU to interact w/ outside world
	- Examples of I/O devices: keyboards, network, mouse, display
	- Functionality: connect many devices--control, respond, & transfer data b/w devices
	- User programs should be able to build on their functionality
- Tasks of processor for IO: input (read bytes), output (write bytes)
- Can either use: special I/O instructions/hardware OR memory mapped I/O
	- Special instructions/hardware inefficient b/c constantly have to change as hardware changes
	- Memory mapped I/O: allocate address space for IO, which contain IO device registers
- Addresses 0x7FFFFFFF and below are reserved for memory mapped IO
	- Each IO device has a copy of its control reg & data reg in region of memory-mapped IO
- 1GHz IO throughput: 4 GiB/s (for load/store word operations)
	- I/O data rates: 10 B/s (keyboard), 3 MiB/s for bluetooth, 64 GiB/s (HBM2 DRAM)

I/O Polling
- Device registers have 2 functions: control reg (gives go-ahead for R/W operations) & data reg (contains data)
- Polling: Processor reads from control reg in loop: if control reg readyBit = 1 --> data is available or ready 
  to accept data. Then, load data from input or write output to data reg. Last, reset control reg bit to 0

Memory map: Input control reg-0x7ffff000, Input data reg-0x7ffff004, Output control reg-0x7ffff008, Output data reg-0x7ffff00c

INPUT: read into a0 from IO Device
lui t0, 0x7ffff # IO Address: 7fffff000
wait:
	lw t1, 0(t0) # Read the control 
	andi t1, t1, 0x1 # Check the ready bit of control
	beq t1, x0, wait # Keep waiting if ready bit != 1
lw a0, 4(t0) # Once we have valid ready bit, load input data reg

OUTPUT: write to display from a1
lui t0, 0x7ffff # Same address as above
wait:
	lw t1, 0(t0) # REad the control
	andi t1, t1, 0x1 # Check ready bit of control
	beq t1, x0, wait # Keep waiting if ready bit != 1
sw a1, 12(t0) # Store output data from a1

- Assume processor has specifications: 1 GHz clock rate, 400 clock cycles per polling operation
- % Processor for polling = Poll Rate * Clock cycles per poll/Clock Rate
	- Example: mouse that conducts 30 polls/s --> % Processor for Polling = 30 * 400/(10^9) = 0.0012%

I/O Interrupts
- Idea: polling is wasteful of finite resources b/c constantly waiting for an event to occur
	- Not a great idea when dealing with large quantities of input/output data
- Alternative: interrupt which "interrupts" the current program and transfers control to trap handler
  when I/O is ready to be dealth with
  	- Throw an interrupt when delivering data or need have relevant information
  	- No IO activity? Regular program continues. Lots of IO? Interrupts are expensive b/c caches/VM are garbage,
  	  requiring saving/restoring state often
- Devices w/ low data rate (ex: mouse, keyboard): use interrupts (overhead of interrupt is low)
- Devices w/ high data rate (ex: network, disk): start w/ interrupt then switch to direct memory access (DMA)
- Programmed I/O: used for ATA hard drive which has processor that initiates all load/store instructions for data movement
  b/w device. CPU obtains data from device and delivers it to main mem & also performs computation on that data
  	- Disadvantages: CPU in charge of transfers (better spent doing smth else), device & CPU speeds misaligned,
  	  high energy cost of using CPU when alternative methods exist

Direct Memory Access (DMA)
- DMA allows IO devices to read/write directly to main memory, utilizing DMA Engine (piece of hardware)
	- DMA engine intended to move large chunks of data to/from data, working independently
	- DMA engine registers contain: mem addr for data, num bytes, I/O device num, direction of transfer, unit of transfer, amount to transfer
- Steps for the DMA transfer:
	Step 1: CPU intiaites transfer (writing addr, count, and control into DMA controller)
	Step 2: DMA requests transfer to memory (goes through the disk controller, which contains a buffer)
	Step 3: Data gets transferred to main memory from the disk controller (through its buffer)
	Step 4: Disk controller send an acknowledgement back to the DMA controller
	Step 5: Interrupt the CPU when all the above operations are completed
	- CPU interrupted twice: once to start the transfer (meanwhile, CPU can do whatever else), and then at the end
	  to indicate that the transfer is complete
- Procedure DMA uses for dealing w/ incoming data: receive interrupt from device, CPU takes interrupt/start transfer (place data at right address), 
  device/DMA engine handles transfer, Device/DMA Engine interrupts CPU to show completion
- Procedure for outgoing data: CPU initiates transfer/confirms external device ready, CPU initiates transfer, Device/DMA engine
  handles transfer, Device/DMA engine interrupts CPU to indicate completion
- DMA Engine can exist b/w L1$ and CPU: which allows for free coherency but trashes CPU working set for the data transferred
	- Free coherency: processor memory/cache system is going to have coherency 
- Can also exist b/w last level cache and main memory: does not mess w/ caches but need to manage coherency explicitly

Networking
- I/O devices can be shared b/w computers (ex: printers), communicate b/w computers (file transfer protocol-FTP), communicate
  b/w ppl (ex: email), communicate b/w computer networks (ex: www, file sharing)
- Internet conceptualized in 1963 when JCR Licklider writes about connecting computers b/w universities
	- 1969: 4 nodes deployed at colleges, 1973: TCP invented, part of internet protocol suite
- World Wide Web: system of interlinked hypertext documents on the internet
	- 1989: Sir Tim Berners Lee develops Hypertext Transfer Protocol (HTTP) allowing for client & server for the internet
- Software protocol to send/receive:
	1. SW SEND: copy data to OPS buffer, calculate checksum w/ timer, send data to network interface hardware to start
	2. SW RECEIVE: OS copies data from network interface hardware to OS buffer, OS calculates checksum--if fine, send ACK, else delete msg
	   If fine, copy data into user address space & tell application it can continue
- Requires a network interface card (NIC) that can be wired or wireless


