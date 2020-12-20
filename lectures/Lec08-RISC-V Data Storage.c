Lecture 8-Storing Data in Memory

Data Transfer
- Compiler tries to minimize # of registers used (usually have access to <= 30 at a time)
- Processor (connected to memory) contains registers: used to read/write data w/ memory
	- From processor side: Writing data = store memory, read data = load memory
	- Memory stores the program and data
		- read (load) FROM, write (store) TO data
	- To access the memory, the processor must specify the address (specifying an offset
	  with respect to a base pointer)
- Data usually <32 bits but >= 8 bits; 8 bits = 1 byte; 1 word = 4 bytes
- Memory addresses occur in bytes (word addresses are 4 bytes apart)
	- LSB of a word: provides the smallest address of a word (Little Endian)
	-Big Endian: 0xAB123400--AB (ADDR0), 12 (ADDR1), 34(ADDR2), 00(ADDR3)
	- Little Endian: 0xAB123400--AB (ADDR3), 12 (ADDR2), 34(ADDR1), 00(ADDR0)
- Registers live on the same level as the processor chip (access to the core)
- Register vs. Memory Speed: register made up of 32 words (128 Bytes) vs.
  memory which consists of billions of bytes (2-64 GB)
  	- Result: Registers are 50-500 times faster than DRAM

C code
int A[100];
g = h + A[3];

RISC-V equivalent using Load Word(lw)
lw x10, 12(x15) #Register x10 gets A[3]
add x11,x12,x10- #g = h + A[3]
sw x10, 40(x15) # A[10] = h + A[3]
	- lw: load a word from memory to a register
	- x15: base register (points to A[0])
	- 12: the # of bytes to offset (must be constant at assembly time--gets the 3rd word)
	- sw = Store Word (putting something from a register to memory)
	- x15: base register
	- 12, 40: offset in bytes (must be in multiples of 4 since dealing w/ ints)
- RISC-V also has lb: load byte, sb: store byte
- lb x10, 0(x3): assume x3 = Nxxx xxxx
	- x10 becomes: NNNN NNNN NNNN NNNN NNNN NNNN Nxxx xxxx
	- The sign bit of x3 is copied over to preserve the original magnitude
		- Referred to as sign extension; can use lbu to fill upper bytes with 0
		  to avoid sign extension


Example: addi
addi x11, x0, 0x3F5 # x11 = 0x3F5
	- Equivalent to load word from register, then adding (but requires extra load)
sw x11, 0(x5) # x11 now stores contents of x5
lb x12, 1(x5) # Writes x12 loads the smallest byte of x5 (which pointed to x11)

Decision Making
- RISC-V if statement: beq reg1,reg2, L1
	- Translation: if (value in reg1 == value in reg2) go to L1; else, go to next statement
- beq: branch if equal; bne: branch if not equal, blt: branch if less than, bge: 
  branch if greater than or equal to
- Branch: change control flow; conditional branch: change control flow based on 
  the result of a comparison; unconditional branch: always branch (equivalent to goto)
  	- Conditional branches: beq, bne, blt, bge, bltu (unsigned blt), bgeu (unsigned bge)
	- Unconditional branches: jump (j)--j label = automatically go to label

If Else example
f->x10, g->x11, h->x12, i->x13, j->x14
if (i == j)      bne x13, x14, ELSE
	f = g + h;   add x10, x11, x12
else             j Exit
	f = g - h;   Else:sub x10, x11, x12
				 Exit:

- blt reg1, reg2, Label: if (reg1 < reg2) goto Label;
- bltu reg1, reg2, Label: if (reg1 < reg2) goto Label; // Using reg1 and reg2 as unsigned
- No such commands: branch (<=) or branch (>)--do not exist b/c of redundancy

Loops
C code

int A[20];
int sum = 0;
for (int i = 0; i < 20; i++) {
	sum += A[i];
}

RISC-V Equivalent
add x9, x8, x0 // x9 = &A[0]
add x10, x0, x0 // sum = 0
add x11, x0, x0 // i = 0
addi x13, x0, 20 // x13 = 20 (provides exist condition)
	Loop:
		bge x11, x13, Done // if i > 20 --> go to done
		lw x12 0(x9) // x12 is loaded with A[i]
		add x10, x10, x12 // sum += A[i]
		addi x9, x9, 4 # x9  // &A[i+1] -- Increment to the next element of the array
		addi x11, x11, 1 // i += 1 -- Used to determine stopping condition (NOT the array index)
		j Loop // Go back to the beginning of Loop
	Done: // End of loop





