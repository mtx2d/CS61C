# RISC-V Practice

# Fall 2019 Final #4
mystery:
	la t6, loop # t6 STORES the address of loop
loop: addi x0, x0, 0 # Instruction to be executed at loop
	lw   t5, 0(t6) # t5 register loads address of instruction (addi)
	# addi: imm[31:20], rs1[15:19], funct3[14:12], rd[11:7], opdcode[0:6]
	addi t5, t5, 0x80 # Increment t5 by 1000 0000, which increments rd by 1
	sw   t5, 0(t6) # Store modified instruction back in memory (addi now will refer to a new register)
	addi a0, a0, -1 # Decrement a0
	bnez a0, loop # Keep looping if a0 != 0
ret

4a. What does mystery(x) do when x < 10?
Reset first x registers from x0

4b: What are the values of a0 when mystery(13) at every iteration?
- 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, -1, -2, -3
- Justification: Upon reaching the 10th iteration, hits a0 register, so a0 is initially reset to 0
- But then a0 is decremented (a0 = -1) before bnez checks its value

4c: How many times is bnez seen for mystery(33) before reaching ret?
2^32 + 10

4d: Justify answer to 4c

- It takes 10 iterations to get to a0. Then, slowly increment -1, -2, -3... until looping back to 0
	- -1 = 2^32 -1, -2 = 2^32 - 2... Which is why it takes 2^32 iterations


# Fall 2019 Final #2c

# What is 0xFF00003 as a RISC-V instruction?
- 1111 1111 0000 [0000 0][000] [0000 0][000 0011] --> Load Instruction
- Format: 20-31: offset, 15-19: rs1, 12-14: funct3, 7-11: rd, 0-6: opcode = LOAD
- Offset: 1111 1111 0000, rs1: 0(x0), rd = 0 (x0), funct3 = 0 (lb)
	--> Offset translation: -1 * (0000 0000 1111 + 1) = -(0000 0001 0000) = -16
	- Final result: lb x0, -16(x0)

# Fall 2019 Midterm #4

# Write an algorithm to reverse a linked list with RISCV

# a0: pointer to "current" element in linked list
# a1: pointer to "previous" element in linked list
reverse:
 	# Node *second = node->next
	lw t0, 4(a0) # t0 holds pointer to "next" element of a0

	# node->next = prev
	sw a1, 4(a0) # Store contents of a1 (which contains pointer to prev) back in a0 memory ("next")

	# if (second == NULL) {return node;}
	beq x0, t0, returnnode

	mv a1, a0 # Set the 2nd argument: *node
	mv a0, t0 # Set the 1st argument: original node->next (points to next element in LL)

	# PROLOGUE--need a prologue/epilogue every time calling a function
	addi sp, sp, -4
	sw ra, 0(sp)

	# return reverse(second, node);
	jal ra reverse

	# EPILOGUE
	lw ra, 0(sp)
	addi sp, sp, 4

returnnode:
	jr ra # Customary way to return from a function

reverse:
	lw t0, 4(sp) # Node *second = node->next
	sw t0  # node->next = prev
	beq x0, t0, returnnode # if (second == NULL) {return node;}
	_________
	_________
	addi sp, sp, -4
	_________
	jal ra reverse # return reverse(second, node);
	_________
	_________
returnnode:
	ret

# Summer 2019 Midterm 1 #5

# Take in 2 char* arguments and copy up to first n characters into destination
# Return a pointer to the destination string
FORMAT: char* strncpy(char* destination, char* source, unsigned int n)

a0: contains pointer to destination string
a1: contains pointer to source string
a2: contains pointer to n
t0: counter for loop
t1: address of nth element
t2: nth element of source string

strncpy:
	add t0, x0, x0 # Current Length
	Loop:
		beq t0, a2, End # If reached nth character --> Go to End
		addi t1, a1, t0 # t1 = source string address (load location)
		lb t2, 0(t1) # Load element in memory t1 into register t2
		addi t1, a0, t0 # t1 = destination string address (store location)
		sb t2, 0(t1) # Store the contents in destination string
		addi t0, t0, 1 # t0 += 1 --> Increment the pointer
		bne t1, x0, Loop # If reached null char --> End
End:
	jr ra

# Fall 2018 Quest #4
4a. Code that compiles x15 = 20 - x5?
sub x5, 20, x15 --> Not allowed b/c 2nd argument must be a register
sub x15, 20, x5 --> Not allowed b/c 2nd argument must be a register
addi x15, x5, -20; sub x15, x0, x15 --> x15 = x5 - 20, x15 = 20 - x5 (VALID)
addi x15, x0, 20; sub x15, x15, x5 --> x15 = 20, x15 = 20 - x5 (VALID)

4b. x5 contains &A[0] where A[99] starts at address 0x0010000
Value of x10 after lw x10, 8(x5)?
- 2nd element of A is loaded into x10


# Summer 2018 Final #3

# Branch if overflow encountered w/ add or addi 

# Unsigned addition overflow
Q1:
	add t0, t1, t2
	bltu t0, t1, overflow # If t0 < t1 --> Bits were eliminated
	j end
# Signed addition overflow with pos immediate
Q2:
	addi t0, t1, posImm
	bltu t0, t1, overflow # If t0 < t1 (unsigned comp) --> Bits eliminated
	j end
# General Signed Addition
Q3:
	add t0, t1, t2
	slt t3, t2, x0 # t3 = t2 is negative
	slt t4, t0, t1 # t4 = t0 < t1 (signed addition result is smaller)
	bne t3, t4, overflow
	j end
overflow: ...
end: ...

# SUmmer 2018 Midterm 


