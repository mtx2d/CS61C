Load Datapath
- Has PC, IMEM, Imm Gen, Register File, ALU just like R and I type instructions
- Also has Data Memory block, which contains an address to read the data from, which appears at DataR port
	- That data is then used to write back into destination register (in the register file)
- Use a multiplexer w/ WBSel to select b/w the load word & result of the ALU (depending on instruction)

Phases of Execution for Load Datapath
- Program Counter points to address in instruction memory, which is retrieved
- The instruction is decoded, loading the appropriate registers, and immediate generator is i type
- WriteEnable = 1, BSel = 1, ALUSel = Add, DMem query for MemRW = Read, WBSel = 0
	- Want to be able to write back into destination register, we want to use RS2 (not immediate), 
	  ALU should be performing addition, ultimately selecting to read from memory
	- Using Immediate Generator to extend the 12 bits in the instruction field
	- Prepare ALU operands: RS1, extend the immediate value 
- Phase 3: ALU performs actual computation, providing an address to point to in data memory
- Phase 4: perform memory access, Phase 5: Write back to destination register
- Different load instructions: lb, lh, lw, lbu, lhu--funct3 field encodes size/signedness of load data

Store Datapath
- sw: read 2 registers (rs1 for base memory address, rs2 for data to be stored) and an immediate offset
- Support 2 immediates: need to handle immediate being split into 2 parts for store instruction (will need
  to modify the immediate generator--immediate select set to S)
	- I type immediates easy to handle by just extending the MSB to the upper 20 bits
	- S-type support: use a 5 bit mux to choose the lower 5 bits of the immediate (that way, can handle
	  both I-type and S-type immediates)
- Store instructions: MemRW = write (not read--to be able to write to memory), add port to memory (DataW, 
  which refers to location to which we will write to the memory). DataR is now rendered irrelevant

Phases of Execution for Load Datapath
- Step 1: fetch the instruction via the program counter
- Step 2: decode the instruction to obtain relevant registers while control logic occurs simultaneously
	- Control logic: Immediate Select = S-type, WriteEnable = 0 (not writing back to register file), 
	  Bselect = 1 (pick immediate, not rs2), ALUSelect = add, MemRW = Write, WBSel = * (do not care)
- Step 3: ALU performs computation w/ the immediate and RS1. RS2 is loaded into DataW port to provide
  the address for which to write back into memory
- New clock cycle: update the program counter and write back into memory

Branch Datapath
- B-format mainly same as S-format (2 source register, 12 bit immediate-now repr 2 byte increments)
- Functionality of the branch: change the program counter (PC = PC + 4 or PC = PC + imm if branch used)
	- Need to calculate PC + immediate AND compare the values of rs1 & rs2
- Modifications to the datapath: multiplexer before PC, immediate generator (ImmSel = Branch type), branch compare block,
  multiplexer to choose b/w PC and branch compare block to feed into the ALU
  - Input for BranchCompare: take in input for whether signed or unsigned --> 2 single bit outputs (less than, equal)
- Multiplexer before ALU: feed either RS1 (to perform other instructions) or the program counter
	- Output of ALU: gets written back into the multiplexer feeding into Program Counter
- Branch Comparator: compared RS1 and RS2 (just checks if they are equal, or less than--remaining implicit)
- Branch Immediate: shift left 1 (b/c want to multiply by 2 byte increments)
	- Need 2 way MUX for each bit b/c immediate bits can appear in 2 potential places now
- RISC-V approach: 11 immediate bits are in fixed position, change 1 bit b/w S and B (only 1 MUX)
- Immediate generation: move the bytes around so that a 32 bit immediate cna be produced
	- Commonalities: sign extend upper bits. Differences: lowest bit field differs for I, S, B-type

Phases of Execution for Branch Instructions
- Step 1: fetch an instruction (program counter pointers to an address in instruction memory)
	- Program counter is incremented by 4, but prior to that, sent to pre-ALU MUX in case it needs updating
- Step 2: decode instruction, obtaining relevant registers in register file, implementing control logic & generate immediate
	- Immediate Select = BType, RegisterWriteEnable = 0, BranchUnsigned = signed or unsigned (output of which is BLT, BEQ), 
	  BSel = 1 to choose immediate over RS2, ASel = 1 to indicate working with PC, not a register, ALUSelect = add,
	  MemoryReadWrite = read
- Step 3: Perform the branch comparison b/w RS1 and RS2 --> want to find the next value for PC
	- ALU adds immediate offset to the program counter --> value gets fed back into PC (only change PC according 
	  to branch if the condition met from branch comparison block)

JALR
- JALR is I Format: Jalr, RD, RS, immediate. Desires: write PC+4 to RD, set PC = rs1 + immediate (no need for multiply by 2 bytes)
- Add a path that allows for writing back from DataR in the data memory block to the destination register
	- Accomplished w/ another field in WBSel MUX (toggle b/w 0, 1, 2 now)--writeback might be coming from data memory,
	  the ALU, PC + 4 value
- Need to modify the control logic (no other modifications needed)
- PCSelect = taken (always take a branch), ImmediateSelect = I-type, WriteEnable = 1 (want to write back to dest reg),
  BranchUnsignedSelect = * (do not care), Bselect = 1, Aselect = 0, ALUSelect = Add, MemoryReadWrite = Read,
  WriteBackSelect = 2 (indicating working with PC + 4 to write back into destination register)

Phases of Execution for JALR
- Start off with fetching and decoding the instruction (key change: PC + 4 gets fed into WBSelect MUX)
- Step 2: decode instruction (preparing register file) & creating the right framework for the control logic
	- Also includes the immediate generation and preparation for ALU to perform operation
- Output of ALU is fed into PC, output of WBSelect block (PC + 4) fed into destination register 
	- Update these states on the next clock tick (PC, RD)

JAL (Jump and Link)
- J format desires: save PC+4 into RD (return address), set PC = PC + offset (PC-relative jump)
	- Reach +/- 2^19 locations, each 2 bytes apart --> Reach +/- 2^18 32 bit instructions
- Change: immediate generator = j-type. Rest can be borrowed from JALR and branch
- Send PC+4 value into WBSelect register to be written back into the destination register
- PCSelect value set to taken (borrowing from branch type

U Type Datapath
- 20 bit immediate, 1 destination register, 2 instructions: lui (load upper immediate), auipc (add upper immediate to PC)
	- lui: load 20 bit imm into RD (clear lower 12 bits), auipc: PC + immediate --> store result in RD
- Change: immediate generator block should now be able to select U-type
- LUI datapath: fetch instruction, parse & increment PC, generate immediate--implement appropriate logic
  to update destination register to reflect the immediate value (on next clock tick--update RD)
- auipc datapath: same steps as LUI, but ALU takes in PC for one of the inputs (other input is the immediate)
	- ALU performs the addition, then writes back into destination register on next clock tick





