.globl argmax

.text
# =================================================================
# FUNCTION: Given a int vector, return the index of the largest
#	element. If there are multiple, return the one
#	with the smallest index.
# Arguments:
# 	a0 (int*) is the pointer to the start of the vector
#	a1 (int)  is the # of elements in the vector
# Returns:
#	a0 (int)  is the first index of the largest element
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 77.
# =================================================================
argmax:

    # Prologue
    addi sp, sp, -16
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw ra, 12(sp)
    
    # Check if length of vector is < 1
    addi t0, x0, 1
	blt a1, t0, loop_end

	add t0, x0, x0 # Counter (in reference to array length)
    lw s0, 0(a0) # s0 used to store the max element
    add s1, x0, x0 # s1 used to store the index of max elem

loop_start:
	beq t0, a1, set_arg
	slli t1, t0, 2 # t1 = 4 * t0
	add t1, t1, a0 # t1 now holds the address of new vector element
	lw t2, 0(t1) # t2 = elem in the array
	bge s0, t2, next_elem 
		mv s0, t2 # If t2 > s0: s0 stores t2
		mv s1, t0 # If t2 > s0: s1 now stores t0
	next_elem:
    	addi t0, t0, 1
    	jal x0, loop_start

set_arg:
	mv a0, s1 # Set a0 to the result of the argmax

loop_end:
	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw ra, 12(sp)
	addi sp, sp, 16
	bge x0, a1, error_code # If 0 >= array length --> branch to error_code
	ret
error_code:
	li a1, 77
	jal exit2
