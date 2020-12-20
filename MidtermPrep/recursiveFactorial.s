li a0, 5
jal ra, factorial # Call factorial function
j finished # Indicate finished program
factorial:
    bnez a0, not_zero # If n > 0: call recursively
    li a0, 1 # Return 1 if n == 0
    jr ra # If n == 0 --> return 1
not_zero:
    # PROLOGUE
    addi sp, sp, -8
    sw ra,4(sp)
    sw s0,0(sp)
    mv s0, a0 # s0 = n
    addi a0, a0, -1 # a0 = n - 1
    jal ra, factorial # Recursive call
    mul a0, a0, s0 # a0 = n * fact(n-1)

    # EPILOGUE
    lw s0,0(sp)
    lw ra,4(sp)
    addi sp,sp,8
    jr ra # Return

finished:
    mv a1, a0 # a1 contains the return of factorial
    mv a0, x0

# CC Checker: java -jar venus.jar -cc recursiveFactorial.s