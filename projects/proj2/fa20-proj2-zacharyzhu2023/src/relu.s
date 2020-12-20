.globl relu

.text
# ==============================================================================
# FUNCTION: Performs an inplace element-wise ReLU on an array of ints
# Arguments:
# 	a0 (int*) is the pointer to the array
#	a1 (int)  is the # of elements in the array
# Returns:
#	None
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 78.
# ==============================================================================
relu:
	# PROLOGUE
	addi sp, sp, -4
	sw ra, 0(sp)

	addi t0, x0, 1
	blt a1, t0, epilogue
	add t0, x0, x0

loop_start:
	beq t0, a1, epilogue
	slli t1, t0, 2
	add t1, t1, a0
	lw t2, 0(t1)
	bge t2, x0, no_zero
		add t2, x0, x0
	no_zero:
		sw t2, 0(t1)
    	addi t0, t0, 1
    	jal x0, loop_start

epilogue:
	lw ra, 0(sp)
	addi sp, sp, 4

	li a2, 1
	bge a1, a2, loop_end

error_code:
	li a1, 78
	jal exit2

loop_end: 
	ret
