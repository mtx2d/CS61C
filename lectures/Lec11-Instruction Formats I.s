kLecture 11: Instruction Formats 1

Stored Program Computer
- RISCV operates on the level of both assembly language (compiler) & machine langauge (assembler)
- First computer (ENIAC) could compute quickly but required rewiring (2-3 days) for new program setup
- Instructions can be represented as a sequence of bits--advent of von Neumann computers
	- Allowed reprogramming in seconds instead of days associated w/ rewiring the ENIAC
	- EDSAC (1949): first general-stored program electric computer; programs rep. by 35 bit words in 2C
- Data/instructions both stored in memory, so both instructions/data (as words) have memory addresses
	- Pointers = memory addresses in C--leads to errors b/c there are not restrictions on what they point to
- Program Counter (PC): register that holds address of instruction being executed
- Programs distr. in binary form (specification differs depending on instruction set used)
- Requires backward compatibility: new machines should be able to compile/run old instructions
- Our data will be broken up into words (32 bits); each register holds a word
- Instructions, likewise, will be represented as 32 bit words
	- Each instruction divided into fields, which give processor info about instructions
- 6 basic instruction formats: R-register to register arithmetic, I-register to immedate arithmetic/loads,
  S-store, B-branch (variant of S), U-20 bit upper immediate instruction, J-jumps (variant of U)

 R Format
- Divided into funct7(25-31), rs2(20-24), rs1(15-19), funct3(12-14), rd(7-11), opcode(0-6)
 	- rs1, rs2, rd: the destination and 2 source registers (encoded w/ 5 bits apiece)
 	- opcode: gives degree of specification for instruction (0110011 for R-format)
 	- funct7/funct3: used in conjunction w/ opcode to describe operation to perform
 		- funct7 for sub/sra = 0100000--1 in 30th bit specifies that sign extension needed
 		- funct7 = 0000000 for all other R commands
 - Example command: add x18, x19, x10
 	--> [0000000]--funct7 = add, [01010]-rs2 = 10, [10011]--rs1 = 19, [000]-funct3 = add, [10010]-rd = 18, [0110011]-opcode = R
- R format instructions: add, sub, sll, slt, sltu, xor, srl, sra, or, and
	- slt: set less than, sltu: set less than unsigned--sets rd = 1 if rs1 < rs2

I Format
- Instruction will be similar to R-format, but w/ one source register serving as part of the immediate
	- Take additional bits from the funct7 field to allow for greater range for the immediate value
- Opcode for instructions involving immediates (I format): 0010011
- Format: immediate(20-31), rs1(15-19), funct3(12-14), rd(7-11), opcode(0-6)
- Example command: addi x15, x1, -50
	--> [111111001110]-imm = -50, [00001]-rs1 = 1, [000]-funct3 = add, 01111-rd = 15, 0010011-opcode = imm
- slli/srli/srai are special b/c they do not need more than 32 bits for their immediate value
	- Thus, bits 25-31 = 0000000 for slli, srli and bits 25-31 = 01000000 for srali to signal that
	  bit extension should be done for srai but not for slli/srli
	- For all 3 instructions, bits 20-24 serve as the immediate value
- I format instructions: addi, slti, sltiu, xori, ori, andi, slli, srli, srai
	- slti: set less than immedate--analogous to slt w/ an immediate value for comparison

Load Instructions
- Loads are a sub-category of the I type instructions in reality
- Format: 20-31: offset, 15-19: rs1, 12-14: funct3, 7-11: rd, 0-6: opcode = LOAD
- Example command: lw x14, 8(x2)
	--> [000000010000]-offset = 8, [00010]-rs1 = 2, [010]-funct3 = lw, [01110]-rd = 14, [0000011]-opcode = LOAD
- Load instructions: lb, lh, lw, lbu, lhu
	- lbu: load unsigned bit, lh: load half word(2 bytes), lhu: load unsigned half word (zero extends)

S Format
- Format: 25-31: offset part1, 20-24: rs2, 15-19: rs1, 12-14: funct3, 7-11: offset part2, 0-6: opcode
- Example command: sw x14, 8(x2)
	--> 0000000: offset part 1 = 0, 01110: rs2 = 14, 00010: rs1 = 2, 010: funct3 = sw, 01000: offset part 2 = 8, 0100011: opcode = STORE
		- The offsets are merged together: 00000000 01000 for a 12 bit offset of 8
- Store instructions: sb, sh, sw
