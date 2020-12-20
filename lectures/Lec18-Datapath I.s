Stages of the Datapath
- Processor (CPU): part of computer that does the work (data manipulation & decision making)
	- Datapath: Part of processor that performs operations as required
	- Control: Dictates to the processor what needs to get done
- Need to be able to implement all RISC-V instructions
- On every clock tick: computer executes 1 instruction. Current state goes through combinational
  logic, and on next rising edge of the clock, it gets written back to state elements
- Stages of the datapath: executing all instructions from one block would be inefficient
	- Solution: divide process into stages, connected to create a cohesive datapath
	- Easier to create smaller stages & optimize stages individually w/o others being affected
- Stage 1: Instruction Fetch (IF), Stage 2: Instruction Decode (ID), Stage 3: Execute, 
  Stage 4: Memory Access, Stage 5: Write Back to Register
	- Fetch: Get instruction from memory and store it in processor, Decode: Interpret instruction, Execute: Perform the instruction,
	  often through ALU, Memory Access (MEM): load/store operations, Write Back to Register (WB): write back data as needed

The Datapath
- Instruction execution contains: PC, Instruction Memory, Register, ALU, Data Memory
	- PC gets updated (incremented by 4) OR branch (as controlled by an MUX block) as needed
	- Meanwhile, instruction memory loads instruction from PC to obtain pointers to RD, RS1, RS2 as needed
	- ALU performs the actul operation, which goes into the Data Memory (which contains a path to write back
	  to the register as needed)
		- Instruction Fetch: points to some location in instruction memory (IMEM)
		- Then, decode/register read occurs. Then execute the instruction accessing memory via ALU,
		  write back from data memory to register
- Single Cycle Datapath: Read in instruction on rising edge of clock, write back on next rising edge
- Datapath Combinatorial Elements: Use adder, ALU, MUX w/ clock corresponding to single cycle
- Datapath state elements: need a register w/ data in port (N bits), data out port (N bits), clock, write enable
	- Write enable = 0: Data Out does not change. Write enable = 1: Data Out set to Data In on rising edge
- Register File: Collection of 32 registers. Takes in 3 5 bit sequences (RW, RA, RB), two 32 bit input busses (busW),
  one 32 bit input bus (busW), Clock, and Write Enable
  	- RA: Selects register for busA. RB: Selects register for busB. RW: selects register to be written via busW when write enable = 1
  	- Clock input only affects write operation
- Memory Block: Block contains data in, data out, clock, write enable, and address
	- Input Bus: data in, Output Bus: Data Out
	- READ: Address chooses word to put in data out, WRITE: chooses word to be written via Data In bus
	- Again, clock input only affects write operation
- Instructions must read/update: registers, program counter, memory
- Can treat instruction memory & data memory as separate (memory holds both in actuality)
	- Instructions are read from instruction memroy, load/store access data memory

R Type Add Datapath
- Datapath includes: Program Counter, Instruction Memory, and an Adder Block
	- PC points to an address in Instruction Memory, and gets updated w/ the Adder block (adds 4)
	- From the instruction, different bits of the 32 bit instruction provide RS1, RS2, RD
		- ALU performs addition operation and writes the result back into the register file at RD at next clock tick
- Control: register file has to have write enabled (b/c the result written back into destination register)
- First rising edge: PC updates after adder delay. At same time, instruction memory loads 32 bit binary instruction
  corresponding to the instruction in IMEM.
- After a delay, rs1 and rs2 are obtained from the instruction
- After an additional delay, ALU performs reg[2] + reg[3]
- Next risign edge: reg[2] + reg[3] written back into destination register

R Type Subtract/Addi instructions
- Sub instruction similar to add instruction (only difference is the funct7 field)
- In the datapath, ALU now has a control bit (ALUSel) that determines whether subtract or add
	- Can be generalized to all R-format instructions using the series of control bits in ALU
	- funct3 and funct7 fields are used to determine the operation to execute
- To support adding an immediate, use a multiplexer to select b/w RS2 and immediate
	- Has a control bit, BSel, that selects rs2 if 0, immediate if 1
- Need to add an immediate generator block (because immediates are naturally only 12 bits)
- I type immediates take 12 bits in instruction and put them in 12 lowest bit positions. Then,
  copy the MSB to extend to the remaining 20 bits

