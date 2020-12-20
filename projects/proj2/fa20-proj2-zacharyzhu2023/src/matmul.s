.globl matmul

.text
# =======================================================
# FUNCTION: Matrix Multiplication of 2 integer matrices
# 	d = matmul(m0, m1)
# Arguments:
# 	a0 (int*)  is the pointer to the start of m0 
#	a1 (int)   is the # of rows (height) of m0
#	a2 (int)   is the # of columns (width) of m0
#	a3 (int*)  is the pointer to the start of m1
# 	a4 (int)   is the # of rows (height) of m1
#	a5 (int)   is the # of columns (width) of m1
#	a6 (int*)  is the pointer to the the start of d
# Returns:
#	None (void), sets d = matmul(m0, m1)
# Exceptions:
#   Make sure to check in top to bottom order!
#   - If the dimensions of m0 do not make sense,
#     this function terminates the program with exit code 72.
#   - If the dimensions of m1 do not make sense,
#     this function terminates the program with exit code 73.
#   - If the dimensions of m0 and m1 don't match,
#     this function terminates the program with exit code 74.
# =======================================================
matmul:
    # Error checks

    # Check if any height/width < 1
    addi t0, x0, 1
	blt a1, t0, error_m0_dims
	blt a2, t0, error_m0_dims
	blt a4, t0, error_m1_dims
	blt a5, t0, error_m1_dims


	# Check if dimensions don't align
	bne a2, a4, error_mismatched_dims

    # Prologue
    addi sp, sp, -48
    sw s0, 0(sp) 
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw s9, 36(sp)
    sw s10, 40(sp)
    sw ra, 44(sp)
    
	mv s0, a6 # s0 = the address of current element of d (that we are manipulating)
	mv s1, x0 # s1 = counter for the outer loop
	mv s2, x0 # s2 = counter for inner loop

	mv s3, a0 # s3 = pointer to m0
	mv s4, a1 # s4 = m0 # rows (HEIGHT)
	mv s5, a2 # s5 = m0 # cols (WIDTH)
	mv s6, a3 # s6 = pointer to m1
	mv s7, a4 # s7 = m1 # rows (HEIGHT)
	mv s8, a5 # s8 = m1 # cols (WIDTH)

outer_loop_start:
	beq s1, s4, outer_loop_end # If outer counter == matrix0 height --> Done looping (go to outer_loop_end)

inner_loop_start:
	beq s2, s8, inner_loop_end # If inner counter == matrix1 width --> Done looping (go to inner_loop_end)

	# Let s9 hold address of desired starting element of matrix0
	mul s9, s1, s5
	slli s9, s9, 2
	add s9, s9, s3
	# Let s10 point to matrix1's desired starting index
	slli s10, s2, 2
    add s10, s10, s6


	# Set up the arguments for dot.s
	mv a0, s9 # Pointer to start of v0
	mv a1, s10 # Pointer to start of v1
	mv a2, s5 # a2 is vector length = width of m0
	li a3, 1 # a3 is stride of v0 = 1
	mv a4, s8 # a4 is stride of v1 = width of m1

	# Calling dot.s
	jal ra, dot # After this call, a0 should contain the dot product

	sw a0, 0(s0) # Load result of a0 (calling dot) back into the destination matrix d @ s0
	addi s0, s0, 4 # Increment s0 by 4 so that it's pointing to the next space

	addi s2, s2, 1 # Increment inner loop counter
	jal inner_loop_start


inner_loop_end:
	mv s2, x0 # Reset inner loop counter to 0
	addi s1, s1, 1 # Increment outer loop counter by 1
	jal outer_loop_start # Go back to the start of outer_loop_start

outer_loop_end:

	#ebreak
    # Epilogue
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw s4, 16(sp)
    lw s5, 20(sp)
    lw s6, 24(sp)
    lw s7, 28(sp)
    lw s8, 32(sp)
    lw s9, 36(sp)
    lw s10, 40(sp)
    lw ra, 44(sp)
    addi sp, sp, 48
    ret

error_m0_dims:
	li a1, 72
	jal exit2
error_m1_dims:
	li a1, 73
	jal exit2
error_mismatched_dims:
	li a1, 74
	jal exit2




