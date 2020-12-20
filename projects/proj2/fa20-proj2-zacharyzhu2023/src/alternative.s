load_m0:
    

    # Malloc data for m0_height
    li a0, 4
    jal ra, malloc
    mv s2, a0 # s2 = pointer to m0_height

    # Malloc data for width
    li a0, 4
    jal ra, malloc
    mv s3, a0 # s3 is a pointer to m0_width

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)



    # Set the arguments
    lw s1, 4(s0) # s1: contains pointer to filename for m0
    mv a1, s2 # a1 = pointer to m0_height
    mv a2, s3 # a2 = pointer to m0_width
    mv a0, s1 # a0: contains pointer to filename for m0

    # Call the function
    jal ra, read_matrix

    # Save relevant data
    mv s1, a0 # s1 = pointer to m0
    lw s2, 0(s2) # s2 = # rows (HEIGHT) of m0
    lw s3, 0(s3) # s3 = # cols (WIDTH) of m0

    ebreak

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12