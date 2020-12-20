.globl classify

#vdb unittests/assembly/TestMain_test0.s inputs/simple0/bin/m0.bin inputs/simple0/bin/m1.bin inputs/simple0/bin/inputs/input0.bin  outputs/test_basic_main/student_basic_output.bin
# 0x1000800C
.text
classify:

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

    # Prologue
    addi sp, sp, -4
    sw ra, 0(sp)
    addi a0, x0, 4
    #addi a1, x0, 4 # If I comment this line out everything goes to shit
    jal ra, malloc

    # Mini Epilogue
    lw ra, 0(sp)
    addi, sp, sp, 4


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
    ret # Return normally

error_args:
    # Do whatever