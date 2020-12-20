# Fall 2018 Midterm 2 #4

# Write a RISCV function to return 0 if input = inf, else nonzero value

a. isNotInfinity:
# Infinity: bits 23-30 = exponent, bits 0-22 = 0
	lui a1, 0x7F800 # Make the upper bit 1
	xori a0, a0, a1 # XORI with the value for pos INF
	ret

done: li a0, 1 # When done, return 1
	  ret
fun:beq a0, x0, done # IF a0 = x0 --> go to done
	addi sp, sp, -12 # Allocate space on stack
	addi a0, a0, -1 # Decrement a0
	sw ra, 8(sp) # Save prev ra value
	sw a0, 4(sp) # Save prev a0 value
	sw s0, 0(sp) # Save prev s0 value
	jal fun # Call fun w/ a0 decremented
	mv s0, a0 # s0 = a0
	lw a0, 4(sp) # Restore a0 vlaue
	jal fun # Call fun w/ new a0 value
	add a0, a0, s0 # a0 += s0
	lw s0, 0(sp) # Restore s0 value
	lw ra, 8(sp) # Restore ra value
	addi sp, sp, 12 # Return stack pointer
ret

b. What is the HEX encoding of the following instruction: beq a0, x0, done?
- B format: 31: imm[12], 25-30: imm[10:5], 20-24: rs2, 15-19: rs1, 12-14: funct3, 8-11:imm[4:1], 7:imm[11], 0-6:opcode

31: imm[12] = 1
25-30: imm[10:5] = 1111 11
20-24: rs2 = x0 = 0 = 00000
15-19: rs1 = a0 = 10 = 01010
12-14: funct3 = 000
8-11:imm[4:1] = 1100
7:imm[11] = 1
0-6:opcode = 1100011

Translation: 1 1111 11 00000 01010 000 1100 1 1100011
1111 1110 0000 0101 0000 1100 1110 0011
0xFE050CE3

c. What is the C version of fun?

 Iteration 0: a0 = 0 --> return 1
 Iteration 1: a0 = 1 --> a0 -= 1 --> a0 = 0 --> call fun --> a0 = 1 --> s0 = 1, a0 = 0 --> call fun again --> a0 = 1 --> a0 += s0 --> a0 = 2
 Iteration 2: a0 = 2 --> a0 -= 1 --> a0 = 1 --> call fun --> a0 = 2 --> s0 = 2, a0 = 1 --> call fun again --> a0 = 2 --> a0 += s0 --> a0 = 4
int fun(int a0)
{
	return (a0 ? 0) 1 : fun(a0 - 1) + fun(a0-1);
}

d. C version of fun with no recursion?
int fun (int a0)
{
	return 1 << a0;
}

e. Maximum value reached w/ a0?
31 (afterward each shift does not do anything)



# Spring 2020 Midterm 1 #6

# 16 bit floating point: 1 sign, 5 exponent, 10 mantissa, Bias = -15
# Assume x is a bit that's corrupted: 0b0xx1 --> 0[00]1, 0[01]1, ...

a. 0b0x1x0x1x0x1x0x1x is received. What is hex encoding for largest number sender can send?
- Sign = 0, Exp = x1x0x, Mantissa: 1x0x1x0x1x
- Maximized: 0 11101 1101110111 --> 0111 0111 0111 0111 --> 0x7777
b. Receive: 0b1110xxxxxxxxxxxx. What is decimal value for smallest number to send?
- Sign: 1, Exp: 110xx, Mantissa: xxxxxxxxxx
	- Exp = 11011, Mantissa = 1111111111
- (-1) * 2^(11011 - 15) * 1.1111111111 = -1 * 2^12 * 1.1111111111 = 1111111111100 =  -4*(2048-1) = - 8192 + 4 = -8192
c. Sign/Exponent is correct but mantissa is corrupted. Not NaN nor infinity. Smallest possible positive number?
- Sign = 0, Exp = 0, Mantissa = 0000000001
- 2^(-14) * 0.0000000001 = 2^-14 * 2^-10 = 2^(-24)


# Spring 2020 Midterm 1 #7

# RISCV function to return the binary representation of a number
findBinary:
	# PROLOGUE--a0 is the arg & return reg
	addi sp, sp, -8
	sw ra, 4(sp)
	sw s0, 0(sp)
	beq a0, x0, end # Base case: return 0
	andi s0, a0, 1 # s0 = 1 if odd, 0 if even
	srli a0, a0, 1 # srli: divide a0 by 2
	jal ra, findBinary # Recursive call
	addi t0, x0, 10 # Load 10 into t0
	mul a0, t0, a0 # a0 = 10 * a0

postamble:
	# EPILOGUE-restore ra, s0, SP
	lw ra, 4(sp) 
	lw s0, 0(sp)
	addi sp, sp, 8
end:
	jr ra


# Spring 2019 Midterm 1: # 3--Search and replace every instance of data inside tree

Struct ASTSimple {
	Struct ASTSimple **children;
	int size;
	int data;
}

void SearchAST (struct ASTSimple *ast, int data, int (*f)(int)) {
	int i = 0;
	/* If the AST is NULL, no match */
	if (ast == NULL) {
		return;
	}
	/* If the head node contains our data, we found a match */
	if (ast->data == data){
		ast->data = (*f)(data);
	}
	/* Search for the data within the children nodes */
	for (; i < ast->size; i++){
		searchAST(ast->children[i], data, f);
	}
}

# Translate to RISCV
a0: pointer to ASTSimple
a1: pointer to data
a2: pointer to function
SearchAST:
	# Prologue
	addi sp, sp, -20
	sw ra, 0(sp)
	sw s0, 4(sp)
	sw s1, 8(sp)
	sw s2, 12(sp)
	sw s3, 16(sp)
	# Preserve and set arguments
	add s3, x0, x0
	mv s0, a0
	mv s1, a1
	mv s2, a2
	# Start computing
	IfOne:
		bne s0, x0, IfTwo # If s0 == 0 --> we're done
		j Done
	IfTwo: # Have we found what we’re looking for?
		lw t0, 8(s0) # Load data from current ast pointer
		bne t0, s1, Loop # If data != intData --> check children
		mv a0, t0	 # Prepare arguments for function call
		jalr ra, 0(s2) # If data == intData --> perform function
		sw a0, 8(s0) # Store transformed data back into memory
	Loop: # Check our children
		lw t0, 4(s0) # Load the size of ast pointer 
		bge s3, t0, Done # If s3 >= t0 --> Return
		lw t0, 0(s0) # Else... load ast->child
		slli t1, s3, 2 # t1 = s3 * 4
		add t2, t0, t1 # t2 = address of current child
		# Prepare for recursive call
		lw a0, 0(t2) # a0 = ast->child[i]
		mv a1, s1 # Ensure that a1 arg is set
		mv a2, s2 # Ensure that a2 arg is set
		jal ra, SearchAST # Make recursive call to function
		addi s3, s3, 1 # Increment s3
		j Loop
Done: # Epilogue
	lw ra, 0(sp)
	lw s0, 4(sp)
	lw s1, 8(sp)
	lw s2, 12(sp)
	lw s3, 16(sp)
	addi sp, sp, 20
	jr ra





