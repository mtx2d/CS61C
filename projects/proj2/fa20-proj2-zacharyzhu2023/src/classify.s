.globl classify



# vdb unittests/assembly/TestMain_test0.s inputs/simple0/bin/m0.bin inputs/simple0/bin/m1.bin inputs/simple0/bin/inputs/input0.bin  outputs/test_basic_main/student_basic_output0.bin
# vdb unittests/assembly/TestMain_test1.s inputs/simple1/bin/m0.bin inputs/simple1/bin/m1.bin inputs/simple1/bin/inputs/input0.bin  outputs/test_basic_main/student_basic_output1.bin


# python3 -m unittest unittests.TestMain -v
# java -jar tools/venus.jar . -dm


.text
classify:
    # =====================================
    # COMMAND LINE ARGUMENTS
    # =====================================
    # Args:
    #   a0 (int)    argc
    #   a1 (char**) argv
    #   a2 (int)    print_classification, if this is zero, 
    #               you should print the classification. Otherwise,
    #               this function should not print ANYTHING.
    # Returns:
    #   a0 (int)    Classification
    # Exceptions:
    # - If there are an incorrect number of command line args,
    #   this function terminates the program with exit code 89.
    # - If malloc fails, this function terminates the program with exit code 88.
    #
    # Usage:
    #   main.s <M0_PATH> <M1_PATH> <INPUT_PATH> <OUTPUT_PATH>

    # COMMAND

    # Check for right # of arguments
    addi a0, a0, -5 # Add -5 to argc (should start as argc = 5)
    bne a0, x0, error_args # If a0 does not initially = 5 --> go directly error_args


    # PROLOGUE
    addi sp, sp, -52
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
    sw s11, 44(sp)
    sw ra, 48(sp)


    mv s0, a1 # Let s0 contain argv


    # =====================================
    # LOAD MATRICES
    # =====================================

    
    # ARGUMENTS FOR READ MATRIX
    #   a0 (char*) is the pointer to string representing the filename
    #   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
    #   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
   

# Load pretrained m0
#ebreak
load_m0:
    
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    # Set the arguments
    lw s1, 4(s0) # s1: contains pointer to filename for m0
    addi sp, sp, -8 # Decrement stack to make space for 2 integers (8 bytes)
    addi a1, sp, 0 # a1 = pointer to m0_height = sp
    addi a2, sp, 4 # a2 = pointer to m0_width = sp + 4
    mv a0, s1 # a0: contains pointer to filename for m0

    # Call the function
    jal ra, read_matrix


    #ebreak
    # Save relevant data
    mv s1, a0 # s1 = pointer to m0
    lw s2, 0(sp) # s2 = # rows (HEIGHT) of m0
    lw s3, 4(sp) # s3 = # cols (WIDTH) of m0

    addi sp, sp, 8 # Increment the stack b/c no longer need the space

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # -----------------------------------

# Load pretrained m1
#ebreak
load_m1:
    
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set the arguments
    lw s4, 8(s0) # s2: contains pointer to filename for m1
    addi sp, sp, -8 # Decrement stack to make space for 2 integers (8 bytes)
    addi a1, sp, 0 # a1 = pointer to m1_height = sp
    addi a2, sp, 4 # a2 = pointer to m1_width = sp + 4
    mv a0, s4 # a0: contains pointer to filename for m0

    # Call the function
    #ebreak
    jal ra, read_matrix

    # Save relevant data
    mv s4, a0 # s4 = pointer to m0
    lw s5, 0(sp) # s5 = # rows (HEIGHT) of m0
    lw s6, 4(sp) # s6 = # cols (WIDTH) of m0

    addi sp, sp, 8 # Increment the stack b/c no longer need the space

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12


    # -----------------------------------

    # Load pretrained input matrix
#ebreak
load_input:
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    # Set the arguments
    lw s7, 12(s0) # s1: contains pointer to filename for input
    addi sp, sp, -8 # Decrement stack to make space for 2 integers (8 bytes)
    addi a1, sp, 0 # a1 = pointer to input_height = sp
    addi a2, sp, 4 # a2 = pointer to input_width = sp + 4
    mv a0, s7 # a0: contains pointer to filename for input

    # Call the function
    jal ra, read_matrix

    # Save relevant data
    #ebreak
    mv s7, a0 # s1 = pointer to m0
    lw s8, 0(sp) # s8 = # rows (HEIGHT) of m0
    lw s9, 4(sp) # s9 = # cols (WIDTH) of m0

    addi sp, sp, 8 # Increment the stack b/c no longer need the space

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12



    # =====================================
    # RUN LAYERS
    # =====================================
    # 1. LINEAR LAYER:    m0 * input
    # 2. NONLINEAR LAYER: ReLU(m0 * input)
    # 3. LINEAR LAYER:    m1 * ReLU(m0 * input)

#jal print_num_alloc_blocks
# STEP 1: LINEAR LAYER
ebreak
linear_layer1:

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    

    mul a0, s2, s9 # Malloc data for m0 * input = m0_height * input_width
    slli a0, a0, 2
    jal ra, malloc

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # Check for malloc failure
    addi t0, x0, 1 # Set t0 to 1 to indicate malloc failure
    beq a0, x0, epilogue # If first malloc returned 0 --> go directly to epilogue (free nothing)
    mv t0, x0 # Reset t0 back to 0 (no error)

    # "Prologue"
    addi sp, sp, -36
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a0, 8(sp)
    sw a1, 12(sp)
    sw a2, 16(sp)
    sw a3, 20(sp)
    sw a4, 24(sp)
    sw a5, 28(sp)
    sw a6, 32(sp)

    # Set the arguments
    mv a6, a0 # a6 = pointer to newly malloc'ed data
    mv a0, s1 # a0 = pointer to m0
    mv a1, s2 # a1 = # rows of m0
    mv a2, s3 # a2 = # cols of m0
    mv a3, s7 # a3 = pointer to input
    mv a4, s8 # a4 = # rows of input
    mv a5, s9 # a5 = # cols of input

    #ebreak
    mv s10, a6 # s10 = pointer to m0 * input

    jal ra, matmul

    #mv s10, a6 # s10 = pointer to m0 * input

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a0, 8(sp)
    lw a1, 12(sp)
    lw a2, 16(sp)
    lw a3, 20(sp)
    lw a4, 24(sp)
    lw a5, 28(sp)
    lw a6, 32(sp)
    addi sp, sp, 36



# STEP 2: NONLINEAR LAYER

ebreak
non_linear_layer:

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)


    mv a0, s10 # a0 = pointer to matrix returned by m0 * input
    mul a1, s2, s9 # a1 = # elements in a0 (s10) = m0_height * input_width
    jal ra, relu # Call relu--results still pointed to by s10

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

# STEP 3: LINEAR LAYER II
ebreak
linear_layer2:

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    mul a0, s5, s9 # Malloc data for m0 * input = m1_height * (s10_width = input_width)
    slli a0, a0, 2
    jal ra, malloc

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    addi t0, x0, 1 # t0 = 1 to indicate failure
    # Check for malloc failure
    beq a0, x0, free_first_matrix # If malloc returned 0 --> go to free_first_matrix (need to free first allocation)
    mv t0, x0 # t0 had no error: restore back to 0


    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set arguments
    mv a6, a0 # a6 = pointer to newly malloc'ed data
    mv a0, s4 # a0 = pointer to m1
    mv a1, s5 # a1 = # rows of m1
    mv a2, s6 # a2 = # cols of m1
    mv a3, s10 # a3 = pointer to s10
    mv a4, s2 # a4 = # rows of s10 = # rows of m0
    mv a5, s9 # a5 = # cols of s10 = # cols of input

    mv s11, a6 # s11 = Contains pointer to malloc'ed data

    jal ra, matmul # Perform matrix multiplication of m1 * relu(m0, input)
    #mv s11, a6 # s11 = m1 * relu(m0, input)

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12


    # =====================================
    # WRITE OUTPUT
    # =====================================
    # Write output matrix
ebreak
write_output:
    
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    
    # Set the arguments
    lw a0, 16(s0) # a0 = pointer to string filename
    mv a1, s11 # a1 = pointer to start of matrix returned by linear layer II
    mv a2, s5 # a2 = # rows in s10 = # rows of m1
    mv a3, s9 # a3 = # cols in s11 = # cols of input
    jal ra, write_matrix # Call write_matrix function

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # =====================================
    # CALCULATE CLASSIFICATION/LABEL
    # =====================================
    # Call argmax
ebreak
call_argmax:

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    mv a0, s11 # Should it be offset by 2 elements (8 bytes?) to avoid first 2 args
    mul a1, s5, s9 # a1 = # elements in vector


    jal ra, argmax

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12 

    bne a2, x0, free_second_matrix # Skip printing if a2 (print_classification integer) != 0

ebreak
print_classification:    
    # Print the classification

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    mv a1, a0
    jal ra, print_int

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)

    # Print new line
    li a1, '\n'
    jal ra, print_char

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

# Free Data
# Free the second allocated matrix
free_second_matrix:
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set arguments
    mv a0, s11
    jal ra, free

    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

free_first_matrix:
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set arguments
    mv a0, s10
    jal ra, free
    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

free_m0:
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set arguments
    mv a0, s1
    jal ra, free
    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12
free_m1:
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set arguments
    mv a0, s4
    jal ra, free
    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12

free_input:
    # "Prologue"
    addi sp, sp, -12
    sw ra, 0(sp)
    sw t0, 4(sp)
    sw a2, 8(sp)
    # Set arguments
    mv a0, s7
    jal ra, free
    # "Epilogue"
    lw ra, 0(sp)
    lw t0, 4(sp)
    lw a2, 8(sp)
    addi sp, sp, 12


epilogue:
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
    lw s11, 44(sp)
    lw ra, 48(sp)
    addi sp, sp, 52
    bne t0, x0, error_malloc # If t0 != 0 --> Go to error_malloc
    ret # Return normally

# Error cases
error_args:
    li a1, 89
    jal exit2
error_malloc:
    li a1, 88
    jal exit2
