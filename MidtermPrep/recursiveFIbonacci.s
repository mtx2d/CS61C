li a0, 5
fib:
    li t0, 2
    bge a0, t0, greater # If n >= 2: call recursively
    li a0, 1 # Return 1 if n == 0 or n == 1
    jr ra # If n == 0 or n == 1--> return 1
greater:
    # PROLOGUE
    addi sp, sp, -12
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw ra, 8(sp)

    mv s0, x0 # s0 = 0
    mv s1, a0 # s1 = n
    addi a0, s1, -1 # a0 = n - 1
    jal ra, fib # Recursive call: fib(n-1)
    add s0, s0, a0 # s0 += fib(n-1)
    addi a0, s1, -2 # a0 = n - 2
    jal ra, fib # Recursive call: fib(n-2)
    add s0, s0, a0 # s0 += fib(n-2)
    mv a0, s0 # a0 = s0 = fib(n-1) + fib(n-2)

    # EPILOGUE
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw ra, 8(sp)
    addi sp, sp, 12
    jr ra # Return