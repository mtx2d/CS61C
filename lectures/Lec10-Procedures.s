Lecture 10-Procedures

Steps to Call a Function
1. Put argument in registers where function can access them
2. Transfer control to function (jal)
3. Get storage for function
4. Execute function
5. Put return value in place where calling code can access.
   Restore registers used & release local storage
6. Return control to point of origin (ret)

Example C/RISCV Function: Leaf

int leaf(int g, h, i, j) # Arguments--Can use registers: a0, a1, a2, a3,
{
	int f; # Store the result: f in s0
	# Will need another save register to save math results
	f = (g + h) - (i + j);
	return f;
}

# RISC-V Translation
Leaf:
	addi sp, sp, -8 # Decrement stack pointer: make room for 2 items
	sw s0, 0(sp) # Store previous value of s1 in the stack
	sw s1, 4(sp) # Store previous value of s0 in the stack
	add s0, a0, a1 # s0 = a0 + a1 --> f = g + h
	add s1, a2, a3 # s1 = a2 + a3 --> temp = i + j
	sub a0, s0, s1 # a0 is return register: a0 = a0 - s1 --> return f - temp
	lw s0, 0(sp) # Restore previous value of s0 from the stack
	lw s1, 4(sp) # Restore the previous value of s1 from the stack
	addi sp, sp, 8 # Increment stack pointer: "free" the data
	jr ra # Go back to the caller (done with routine)

Register Conventions
- If a function is called from within a function, need to save return address
	- Caller needs to save the address of next instruction in case callee messed with ra register
	- Similarly, if a caller plans on calling a function but uses a temporary register or 
	  argument after the callee, they need to store those results in the stack as well
- CALLER: calling function; CALLEE: function that gets called
	- The burden: caller needs to know that certain registers remain undisturbed
- Register conventions: rules for registers governing what registers can/cannot be changed in a function call
	- Preserved across functino call: sp, gp, tp, & save registers: s0-s11
	- NOT preserved: argument/return registers (a0-a7, ra), temporary registers (t0-t6)

Example C/RISCV Nested Function: sumSquare & mult

# C Code--both functions take in 2 arguments & spit 1 out
int sumSquare(int x, int y) {
	return mult(x, x) + y;
}

# RISC-V Translation--abstract away whatever mult does; focus on sumSquare
sumSquare:
	addi sp, sp, -8 # Decrement pointer: Make space for 2 objects
	sw ra, 0(sp) # Need to save return address b/c makes a function call FROM w/in a function
	sw a1, 4(sp) # Also need to save y value in a1 in case mult clobbers over it
	mv a1, a0 # a1 = a0 --> Preparing for a call to mult
	jal mult # Call mult with the argument registers (result stored in a0)
	lw a1, 0(sp) # Restore y into a1 from the stack
	add a0, a0, a1 # a0 = mult() + y
	lw ra, 4(sp) # Restore the return address from stack
	addi sp, sp, 8 # Increment the stack
	jr ra # Go back to the next instruction

Memory Allocation
- Stack: used to save previous values before function call, restore, & delete
	- LIFO queue: push (put data into stack) & pop (remove data from the stack)
	- Contains a stack pointer (sp, x2)--which grows downward by convention
	- Stack frames contain return address, parameters (args), & space for local variables
	- Frames const. contiguous blocks of mem w/ sp pointing to bottom frame
- C has 2 storage classes: automatic (local to function & DNE after function exits) & static (always present)
- Stack used for local variables that cannot be stored in registers; procedure frame: portion of the stack
  w/ the save registers/local variables
 - C program contains 3 areas of memory: static (only die when execution ends), heap (dynamically declared variables),
   & stack (used by procedure during execution--where save registers stored)
- RV32 stack starts at 0xBFFFFFF0 & grows down; text: 0x00010000, static: 0x1000000 (exists above text),
  heap (above static data) grows upward to high addresses towards stack





