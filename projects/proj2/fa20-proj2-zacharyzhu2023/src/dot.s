.globl dot
# 30 36 42 66 81 96 102 126 150
.text
# =======================================================
# FUNCTION: Dot product of 2 int vectors
# Arguments:
#   a0 (int*) is the pointer to the start of v0
#   a1 (int*) is the pointer to the start of v1
#   a2 (int)  is the length of the vectors
#   a3 (int)  is the stride of v0
#   a4 (int)  is the stride of v1
# Returns:
#   a0 (int)  is the dot product of v0 and v1
# Exceptions:
# - If the length of the vector is less than 1,
#   this function terminates the program with error code 75.
# - If the stride of either vector is less than 1,
#   this function terminates the program with error code 76.
# =======================================================
dot:
    bge x0, a2, error_length # If array length <= 0 --> Branch to error_length
    bge x0, a3, error_stride # If stride1 <= 0 --> Branch to error_stride
    bge x0, a4, error_stride # If stride2 <= 0 --> Branch to error_stride
    # Prologue
    addi sp, sp, -40
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)
    sw s3, 12(sp)
    sw s4, 16(sp)
    sw s5, 20(sp)
    sw s6, 24(sp)
    sw s7, 28(sp)
    sw s8, 32(sp)
    sw ra, 36(sp)


    mv s0, a0  # Let s0 hold the address of v0[elem]
    mv s1, a1  # Let s1 hold the new address of v1[elem]
    mv s2, x0  # Let s2 hold the final dot product
    mv s3, x0 # s3 = Counter for v0/v1 (in reference to array length)

loop_start:
	beq s3, a2, set_arg # Let s3 hold the value of v0[counter0]
    lw s4, 0(s0) # Let s4 hold the value of v0[counter0]
    lw s5, 0(s1) # Let s5 hold the value of v1[counter1]
    # Let s6 represent the product of s4 and s5
    mul s6, s4, s5

    add s2, s2, s6 # Total += new product
    addi s3, s3, 1 # Increment the counter

    # Set the incremented stride lengths
    mul s7, s3, a3
    mul s8, s3, a4

    # Get the offset
    slli s7, s7, 2
    slli s8, s8, 2

    # Get the new addresses
    add s0, a0, s7
    add s1, a1, s8

    j loop_start

set_arg:
	mv a0, s2

loop_end:
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
    lw ra, 36(sp)
    addi sp, sp 40
    ret
error_length:
	li a1, 75
	jal exit2
error_stride:
	li a1, 76
	jal exit2


