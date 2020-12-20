Lecture 7-Intro to RISC-V

Assembly Language
- Idea: anything can be represented as a number (data/instructions)
- RISC-V exists in assembly language & machine language
	- Typically do not write assembly language code (compiler takes care of it)
- Job of CPU: execute instructions, which are primitive operations
	- Dif. CPU in charge of implementing dif instructions
	- Instruction Set Architecture(ISA): set of instructions a CPU uses
- Ppl initially tried to make CPUs be able to handle more types of
  instructions to perform more complex operations
- RISC-V: Reduced Instruction Set Computing (philosophy dev. in 1980s)
	- Idea: keep instruction set limited, to increase speed. Software will handle
	  more "complex" operations
- Benefits of Risc-V: simplicity & wide adoption (32, 64, 128-bit variants)
- RISC-V initially dev to teach but became more viable in commercial/research


Registers
- Instruction set repr w/ assembly language (each line = 1 instruction)
- Assembly does not have variables (assembly operands: registers)
	- Register: hardware objects that exist w/in processor core
		- Operations occur on the data stored in the registers
- Disadvantage of registers: fixed # of registers (dependent on hardware)
- 32 registers in RISC-V: balance b/w not too many to slow performance
- Each RISC-V register is 32 bits wide (each group of 32 bits = 1 word)
- Registers numbered x0-x31, x0 always holds value 0
	- Can refer to register by number or name
	- Registers have no inherent type
- (#): symbol used for RISC-V comments
- Each statement (instruction) executes 1 of a limited set of commands
	- Each line must contain precisely 1 instruction (usually =, +, -, *, /)

RISC-V Syntax
- Format: one two, three, four
	- Example: add   x1, x2, x3 # Does the following: x1 = x2 + x3
	- Example #2: sub x3, x4, x5 # x3 = x4 - x5
- one: operation by name
- two: operand to receive the result ("destination", x1)
- three: first operand for operation("source #1", x2)
- four: second operand for operation("source #2", x3)
- NOTE: must have 1 operator & 3 operands to maintain consistency
- a = b + c + d - e
	1. add x10, x1, x2 #temp = b + c
	2. add x10, x10, x3 #temp = temp + d
	3. sub x10, x10, x4 #temp = temp - e
- f = (g + h) - (i + j)
	1. add x11, x1, x2 #temp = g + h
	2. add x12, x3, x4 #temp2 = i + j
	3. sub x11, x11, x12 #temp = temp - temp2

RISC-V Immediate
- Immediate: numerical constants that appear often in code
- Example syntax: addi x3, x4, 10 #x3 = x4 + 10
	- Using a number instead of a register to add immediate
- RISC-V does not have subi (can't subtract immediate) b/c can just add negative
- Register 0 (x0): automatically takes the value 0 and is immutable
- add x3, x4, x0 #Result: f = g + 0 --> f = g
- add x0, x3, x4 #Result: x0 = 0 (doesn't change the value)

