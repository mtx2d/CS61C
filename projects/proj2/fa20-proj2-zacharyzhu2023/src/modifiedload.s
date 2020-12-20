# Load pretrained m0
load_m0:
    
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    # Set the arguments
    lw s1, 4(s0) # s1: contains pointer to filename for m0

    # Malloc space for # rows for input
    addi a0, x0, 4
    jal ra, malloc
    beq a0, x0, error_malloc # Check for malloc failure
    mv a1, a0
    mv s2, a0 # s2 = pointer to m0_height

    # Malloc space for # cols for m0
    addi a0, x0, 4
    jal ra, malloc
    beq a0, x0, error_malloc # Check for malloc failure
    mv a2, a0
    mv s3, a0 # s3 = pointer to m0_height

    # Call the function
    jal ra, read_matrix


    # Save pointer to m0 matrix
    mv s1, a0 # s1 = pointer to m0

    # Free the pointers to m0_height and m0_width
    mv a0, s2
    jal ra, free
    mv a0, s3
    jal ra, free

    # Save dimensions of m0 matrix
    lw s2, 0(s1) # s2 = # rows (HEIGHT) of m0
    lw s3, 4(s1) # s3 = # cols (WIDTH) of m0


    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12




# Load pretrained m1
load_m1:
    
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    # Set the arguments
    lw s4, 8(s0) # s4: contains pointer to filename for m1

    # Malloc space for # rows for m1
    addi a0, x0, 4
    jal ra, malloc
    beq a0, x0, error_malloc # Check for malloc failure
    mv a1, a0
    mv s5, a0 # s5 = pointer to m1_height

    # Malloc space for # cols for input
    addi a0, x0, 4
    jal ra, malloc
    beq a0, x0, error_malloc # Check for malloc failure
    mv a2, a0
    mv s6, a0 # s6 = pointer to m1_height

    # Call the function
    jal ra, read_matrix


    # Save pointer to input matrix
    mv s4, a0 # s4 = pointer to input

    # Free the pointers to m1_height and m1_width
    mv a0, s5
    jal ra, free
    mv a0, s6
    jal ra, free

    # Save dimensions of m1 matrix
    lw s5, 0(s4) # s5 = # rows (HEIGHT) of m1
    lw s6, 4(s4) # s6 = # cols (WIDTH) of m1


    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12




# Load pretrained input
load_input:
    
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    # Set the arguments
    lw s7, 12(s0) # s7: contains pointer to filename for input

    # Malloc space for # rows for input
    addi a0, x0, 4
    jal ra, malloc
    beq a0, x0, error_malloc # Check for malloc failure
    mv a1, a0
    mv s8, a0 # s8 = pointer to input_height

    # Malloc space for # cols for input
    addi a0, x0, 4
    jal ra, malloc
    beq a0, x0, error_malloc # Check for malloc failure
    mv a2, a0
    mv s9, a0 # s9 = pointer to input_height

    # Call the function
    jal ra, read_matrix


    # Save pointer to input matrix
    mv s7, a0 # s7 = pointer to input

    # Free the pointers to input_height and input_width
    mv a0, s8
    jal ra, free
    mv a0, s9
    jal ra, free

    # Save dimensions of input matrix
    lw s8, 0(s7) # s8 = # rows (HEIGHT) of input
    lw s9, 4(s7) # s9 = # cols (WIDTH) of input


    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

