Lecture 9: Logical Instructions

Bitwise Operations
- C bitwise oeprators: &, |, ^, <<, >>
- RISC-V Equivalents: and, or, xor, sll, srl
	- Each have their own parallels w/ immediate
	and x5, x6, x7 // x5 = x6 & x7
	andi x5, x6, 3 // x5 = x6 & 3
		- andi w/ 0x00000FF gets LSB, andi w/ 0xFF000000 gets MSB (masks)
- RISC-V does NOT have logical not (instead: xor w/ 11111111 for each byte)
slli x11, x12, 2 // x11 = x12 << 2
- Shifted right arithmetic (sra, srai) moves n bits to right & inserts MSB sign bit into empty positions
	x10 = 0xFFFFFFE7 = -25
	srai x10, x10,4 --> 0xFFFFFFFE = -2 (does not directly divide by 2^n)
	- Fails for odd negative numbers & C rounds toward 0 (not true here)

Machine Program/Psuedoinstructions
- Assembler converts source files into object files
- Linker combines the .o files into an executable (such as a.out)
	- Executable lives in memory (which contains all of the following: program, bytes, data)
- Memory contains program, bytes, & data: each RISC-V Instruction contained w/in program as 32 bits
- PC (program counter): register in processor that holds byte address of next instruction to be completed
	- Instruction retrieved from memory --> Control unit completes instruction using datapath/memory --> update PC
	  (default of adding 4 bytes to move to next instruction but branch/jump can change that)
- RISC-V has symbolic register names: a0-a7 = registers x10-x17 for function calls
- Pseudo instructions exist as a shorthand for common assembly commands
	- Ex: mv rdm rs = addi rd, rs, 0
	- Ex: li rd, 13 = addi rd, x0, 13
	- nop = addi x0, x0, 0

Function Calls

/* C multiplication function */
int mult(int a, b)
{
	int product = 0;
	while (a > 0) {
		product += b;
		a--;
	}
	return product;
}
/* Calling mult function written above */
void callMult()
{
	int i = 5, j = 4, k = 7;
	mult(i,j);
	mult(j,k);
	mult(j,j);
}

Calling a function
- Step 1: Make arguments accessible, Step 2: Transfer control to function, Step 3: acquire storage for function
  Step 4: execute function, Step 5: put return value in accessible location to calling code, restore registers used, release storage
  Step 6: Return control to origin
- RISC-V conventions: a0-a7 are 8 argument registers to pass parameters, 2 of which store return values (a0-a1)
	- ra: return adddress register to return to point of origin (x1)
	- s0-s1 (x8-x9) & s2-s11 (x18-x27) are the save registers

/* Sample C Function */
int callSum()
{
	sum(x, y); // Pass by value
}
int sum(int x, int y) {
	return x + y;
}

/* RISC-V under the hood */

// Each of the following are addresses where the program instructions  are stored 
1000 mv a0, s0 // Initialize: x = a
1004 mv a1, s1  // Initialize y to b
1008 addi ra, zero, 1016 // ra = 1016--> this stores the return address
1012 j sum // Jump to the instruction beginning with sum
1016
...
2000 sum: add a0, a0, a1 // x = x + y--adding the value of y to x, saved to x
2004 jr ra // Jump register: go to the register stored in ra, which is 1016
           // Use jr to avoid having to manually code out return address every time

jal & jr
- jal (Jump and Link): Jump to and save address
	Before: 1008 addi ra, zero, 1016 // ra = 1016
	        1012 j sum // Jump to sum instruction
	After:  1008 jal sum // Equivalent to: ra = 1012, goto sum
	- jal serves as a common case utilized b/c function calls often used & reduces program size
		- Also eliminates having to know where the code is in memory, b/c it is the next address
- "Link" component refers to create an address (aka Link) pointing to call site so that a function
  knows where to return a value to
- jal functionLabel: goes to the address & saves address of instruction in register ra
- jr (Jump register): unconditional jump to address specified in register







