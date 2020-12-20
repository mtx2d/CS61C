Lecture 12: Instructions II 

RISC-V Conditional Branches
- Branches read 2 registers but do not write to register (resemble stores)
- Typically used for loops while functional calls use j-format
- Address of current instructions stored in program counter (PC) & branch
  distance is limited by size of code
- PC-Relative Addressing: Use immediate field to offset from PC when branching
- One idea would be to *4 the offset to increase reach compared to byte offset
	- Branch: PC = PC + immedate * 4; No Branch: PC = PC + 4
- However RISCV has to support 16 bit instructions, so offset calculated by 2 bytes (not 4)
	- Can only reach +/- 2^10 32-bit instructions from PC

B Format 
- B format: 31: imm[12], 25-30: imm[10:5], 20-24: rs2, 15-19: rs1, 12-14: funct3, 8-11:imm[4:1], 7:imm[11], 0-6:opcode
	- Similar to the S-format but the immediate values locations are scrambled,
	and the 12-bit immediate corresponds to 13 bit byte offsets b/c lowest bit = 0
- Example code: Loop: beq x19, x10, END
				add x18, x18, 10
				addi x19, x19, -1
				j Loop
				END
	--> 0: imm[12], 00000: imm[10:5], 01010: rs2 = 10, 10011: rs1 = 19, 000: funct3 = BEQ, 
  	    1000: imm[1:4] = 1000, 0: imm[11] = 0, 1100011: opcode = BRANCH
  	    - Immediate translation: 0 0 00000 1000 0 --> Offset = 0000000010000 = 16; made up
  	      of 13 bits b/c the last bit is always 0
- Branch Instructions: BEQ, BNE, LT, BGE, BLTU, BGEU
	- opcode = 1100011, funct3 varies depending on the branch instruction

U-Format: Upper Immediate Instructions
- Format: 12-31: immediate value, 7-11: rd, 0-6: opcode (either LUI or AUIPC)
	- LUI: load upper immedate, AUIPC: add upper immediate to PC
- LUI: adds to upper 20 bits of destination & clears lower 12 bits
	- Can manually set those 12 bits using addi to create any 32 bit value
	- Ex: lui x10, 0x87654 # x10 = 0x87654000--bottom 12 bits cleared
		  addi x10, x10, 0x321 #x10 = 9x87654321
	- addi sign extends, so it subtracts 1 from upper 20 bits if top bits set
		lui x10, 0xDEADB # x10 = 0xDEADB000
		addi x10, x10, 0xEEF # x10 = 0xDEADAEEF--B bit got subtracted
		# EEF--MSB = 1 so it sign extends to 111... for the upper bits--equivalent to adding -1 to upper 20 bits--Hence, DEADB --> DEADAkb
		lui x10, 0xDEADC # x10 = 0xDEADC000--Solution is to preincrement 4th byte
		addi x10, x10, 0xEEF # x10 = 0xDEADBEEF
		li x10, 0xDEADBEEF # Better pseudoinstruction to deal w/ this
- Label: AUIPC x10, 0 # Puts address of label in x10

J-Format: Jump Instructions
- Jal Format: 31: imm[20], 30-21: imm[10:1], 20: imm[11], 19-12: imm[19:12], 11-7: rd, 0-6: opcode
	- Jal saves PC+4 into register rd, j psuedo instruction uses jal w/ rd = x0
	  to discard the return address
	- After, set PC = PC + offset  w/ +/- 2^19 locations from +/-2^18 instructions
	j label # Equivalent to: jal x0, label
	jal ra, FuncName # Call function w/in 2^18 instructions of PC
- Jalr Format: 20-31: imm[11:0], 15-19: rs1, 12-14: ffunc3, 7-11: rd, 0-6: opcode
	- Result: rd = PC + 4, PC = rs + immediate, immediates are not multiplied by 2 bytes
	ret # Equivalent to: jr ra and jalr x0, ra, 0
	# Call function at arbitrary 32 bit address
	lui x1, <upper20bitseq>
	jalr ra, x1, <lower12bitseq>
	# Jumps to PC relative w/ 32 bit offset 
	auipc x1, <upper20bitseq>
	jalr x0, x1, <lower12bitseq>


