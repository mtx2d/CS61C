.data

InputArray:   .word 1 7 6 0

Space: .asciiz " "

BeforeString:   .asciiz "Input should be '1 7 6', and it is: "

AfterString:   .asciiz "After calling, it should be '1 13 8'. It is: "

.text

main:
#Feel free to edit this for your own tests, but this part will not be graded
#Print Array before running code
la a0 BeforeString
jal print_str
la a0 InputArray
jal print_array
jal print_newline
#Run callfib
la a0 InputArray
jal callfib
mv s0 a0
#Print String after running code
la a0 AfterString
jal print_str
la a0 InputArray
jal print_array
jal print_newline
#Exits the program
li a0 10
ecall



callfib:
# Runs the function fib on all elements of the input array a0. You may assume that all values of the 
# array are nonzero, and that the array terminates on the first
# entry which is zero. Stores the results in the original array
# YOUR CODE HERE


loop:
    lw s0, 0(a0)
    beq s0, x0, done # If s0 == 0, go to done
    
    # Prologue
    addi sp, sp -8
    sw ra, 0(sp)
    sw a0, 4(sp)
    
    mv a0, s0 # Set argument for fib
    jal ra, fib
    #ebreak
    mv s0, a0
    
    # Epilogue
    lw ra, 0(sp)
    lw a0, 4(sp)
    addi sp, sp, 8
    
    sw s0, 0(a0) # Save result back in address
    addi a0, a0, 4 # Point to next element of a0
    j loop

done:
    li a0 0
    jr ra

fib:
# Iteratively determines the a0th fibonacci number.
#YOUR CODE HERE
    li t0, 1 # Prev Fib
    li t1, 1 # Curr Fib
    li t2, 1 # Index

fibLoop:
    beq a0, t2, fibDone # If t2 == a0 --> return t0
    mv t3, t1 # t3 = curr fib
    add t1, t0, t1 # t1 = curr + prev
    mv t0, t3 # t0 = t3
    addi t2, t2, 1
    j fibLoop
fibDone:
    mv a0, t0
    jr ra

###########################################
#Utility Functions
###########################################
print_int:
    mv a1, a0
    li a0, 1
    ecall
    jr    ra

print_str:
    mv a1, a0
    li a0, 4
    ecall
    jr    ra

print_newline:
    li a1, '\n'
    li a0, 11
    ecall
    jr    ra
    
print_array:
    addi sp sp -8
    sw s0 0(sp)
    sw ra 4(sp)
    mv s0 a0
    printarrayLoop:
    lw a0 0(s0)
    beq a0 x0 printarrayEnd
    jal print_int
    la a0 Space
    jal print_str
    addi s0 s0 4
    j printarrayLoop
    printarrayEnd:
    lw s0 0(sp)
    lw ra 4(sp)
    addi sp sp 8
    jr ra