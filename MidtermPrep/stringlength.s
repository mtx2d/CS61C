# Recursively finds the length of the string
# a0: pointer to the string
# At end of function, a0 should contain length of string


# Load [1, 2, 3, 4, 0] into memory
li a0 0x10000000
li a1 1
sb a1, 0(a0)
li a1 2
sb a1, 1(a0)
li a1 3
sb a1, 2(a0)
li a1 4
sb a1, 3(a0)
li a1 0
sb a1, 4(a0)
ebreak

jal ra, stringlength # Call the function
j finished # Finished calling function


stringlength:
	lb t0, 0(a0) # t0 = current char a0 points to
	beq t0, x0, basecase # If t0 == 0 --> go to basecase
	# PROLOGUE
	addi sp, sp, -4 # Decrement stack
	sw ra, 0(sp) # Save return reg
	addi a0, a0, 1 # Point to next element
	jal ra, stringlength # Make recursive call to stringlength
	addi a0, a0, 1 # s0 = length of string
	# EPILOGUE
	lw ra, 0(sp) # Restore return reg
	addi sp, sp, 4 # Increment the stack
	jr ra # Go back to prev instruction
basecase:
	li a0, 0 # Return 0
	jr ra

finished:
	mv a1, a0 # a1 contains the result
    mv a0, x0 # Finished

