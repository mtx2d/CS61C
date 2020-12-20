.globl factorial

.data
n: .word 5

.text
main:
    la t0, n
    lw a0, 0(t0)
    jal ra, factorial

    addi a1, a0, 0
    addi a0, x0, 1
    ecall # Print Result

    addi a1, x0, '\n'
    addi a0, x0, 11
    ecall # Print newline

    addi a0, x0, 10
    ecall # Exit

factorial:
    addi t1, x0, 1
loop:    
    beq a0, x0, endLoop
    mul t1, t1, a0
    addi a0, a0, -1
    j loop
endLoop:
    mv a0, t1
    ret
    