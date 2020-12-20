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
    li t0, 3
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