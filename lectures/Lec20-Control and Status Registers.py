Lecture 20: Control and Status Registers

CSRs and System Instructions
- Control and Status Registers (CSRs): separate from register file; used to monitor status/performance
- Needed for counters/timers, & communicate with peripherals using control words
	- CSRs not part of base ISA but needed in most implementations
- CSR instructions: source/dest = csr[20:31], RS1 = source[15:19], funct3 = instr[12:14]
  rd = rd[7:11], opcode = SYSTEM 1110011[0:6]
- csrrw (csr read write): copies CSR and stores result in destination register. Takes source reg and puts it in CSR
	- Exception: x0 which does not read CSR
- csrrs/c: deals w/ RS instead of RD. csrrwi, csrrs/ci: immediate variation of csrrw/csrrs; replace source w/ immediate
- csrrw x0, csr, rs1 # RESULT: rd = x0, write rs1 value to csr
- csrwi csr, uimm functions as psuedoinstruction for csrrwi x0, csr, uimm
- ecall: makes requests to support execution envr (OS) like system calls
- ebreak: used by debuggers to transfer control to debugger
- fence: separates memory & I/O access from viewpoint of threads/processors

Datapath Control: sw, beq
- Components of the datapath: PC, regfile, IMEM, DMEM, branch comparator, ALU
- sw: rising edge of clock: PC gets instruction (fetch instrution from IMEM) and PC gets incremented
	- Then, prepare for immediate generation & decode instruction (access register file)
	- Control Logic: PCSel = PC + 4, ImmSel = S-type, WEn = 0, BrUn = *, BSel = 1 (use imm), ASel = 0 (Use RS1)
	  ALUSel = add (add offset), MemRW = write (write back to memory), WBSel = *
	- RS1, RS2, and the immediate value are all accessible --> sent through ALU to add values
	- Data memory block has address (ALU: RS1 + offset) & value to write into location @ datawrite port
- beq: start on rising edge to get instruction & increment PC. Then, gen imm + decode instruction (RS1, RS2, RD)
	- Control logic: PCSel = PC + 4, ImmSel = B-type, WEn = 0, BrUn = *, BrLT = *, BSel = 1 (use imm), ASel = 1 (Use PC)
	  ALUSel = add (add offset), MemRW = read (no need to write back to memory), WBSel = *
	- Push through the ALU--allows MUX right before PC to det whether branch should be taken or not

Instruction Timing: Add Timing 
- Step 1 on rising edge of clock: fetch instruction from IMEM using PC & increment PC by 4
- Step 2: start retrieving RS1, RS2, RD from register file, immediate generation, and control logic
	- Control logic: PCSel = PC + 4, ImmSel = *, WEn = 1, BrUn = *, BrLT = *, BSel = 0, ASel = 0,
	  ALUSel = add, MemRW = Read, WBSel = 1 (write back ALU value)
	- Instruction decode only complete once valid outputs from register file are generated
- Step 3: execute and put values of RS1 and RS2 into ALU and spit out a result
- Step 4: Write data back into the destination register (written back on the next rising edge of the clock)
- PC timing = time(CTQ) + max(tAdd + tMUX, tIMEM + tReg + tMUX + tALU + tMUX) + time(setup)
- Execution timing = tCTQ + tIMEM + tReg + tMUX + tALU + tMUX + tSetup

Instruction Timing: lw timing 
- Step 1 on rising edge of clock: fetch instruction from IMEM using PC & increment PC by 4
- Step 2: start retrieving RS1, RS2, RD from register file, immediate generation, and control logic
	- Control logic: PCSel = PC + 4, ImmSel = I-type, WEn = 1, BrUn = *, BrLT = *, BSel = 1 (take imm), ASel = 0 (RS1),
	  ALUSel = add, MemRW = Read, WBSel = 0 (send memory output to be written)
	- Instruction decode only complete once valid outputs from register file are generated
- Step 3: execute and put values of immGen and RS2 into ALU and spit out a result. New value prepared for new PC
- Step 4: Write data back into the destination register (written back on the next rising edge of the clock)
- PC timing = tCTQ + tAdd + tMUX + tSetup
- Immediate Gen timing = tCTQ + tIMEM + tIMM + tMUX + tALU + tDMEM + tMUX + tSetup
- Alt Path timing = tCTQ + tReg + tMUX + tALU + tDMEM + tMUX + tSetup
- Take the max of the 3 potential paths to obtain the critical path time
- Rough timing: IF(IMEM) = 200ps, ID (reg read) = 100ps, EX (ALU) = 200ps, Mem(DMEM) = 200ps, WB (Reg W) = 100ps

Instruction Phases and Timing
- Add: IF + ID + ALU + WB = 600 ps (no need to access data memory)
- BEQ = IF + ID + ALU = 500 ps (no need to access data memory nor write back to a register)
- JAL = IF + ID + ALU = 500 ps (again, do not need DMEM nor WB)
- lw = IF + ID + ALU + DMEM + WB = 800 ps (need to access DMEM & writes back to a register)
- sw = IF + ID + ALU + MEM = 700 ps (will not be writing back to a register)
- Maximum clock frequency = fMAX = 1/800ps = 1.25 GHz, though most blocks idle most of the time

Control Logic Design
- 2 options to implement control logic truth table for the instructions: ROM (Read Only Memory) & Combinatorial Logic
	- ROM: regular structure, easily reprogrammable, used when designing control logic manually
	- Combinatorial logic: tools exist to convert truth tables into gates
- Instruction type encoded w/ 9 bits: inst[30, 12:14, 2:6]
- ROM takes these 9 bits as input, along w/ BrEQ/BrLT. OUTPUT: PCSel, ImmSel (3 bits), BrUn, ASel, BSel, ALUSel (4 bits),
  MemRW, WEn, WBSel (2 bits)--> Total of 15 output bits
- The address decoder (which takes in inst, BrEQ, BrLT, points to a control word for corresponding instruction)
	- Only one control word is accessed which is then used to generate outputs specified above
- Decode BrUN: BrUN = Inst[13] * Branch (interpretation: must be branch time and middle bit of funct3 field = 1)









