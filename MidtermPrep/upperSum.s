.data

InputString:   .asciiz "Cal"

BeforeString:   .asciiz "Input should be 'Cal', and it is: "

AfterString:   .asciiz "After calling, it should be 'CAL'. It is: "

ReturnValueString:   .asciiz "Return Value should be 272. It is: "

.text

main:
#Feel free to edit this for your own tests, but this part will not be graded
#Print String before running code
la a0 BeforeString
jal print_str
la a0 InputString
jal print_str
jal print_newline
#Run UpperSum
la a0 InputString
jal UpperSum
mv s0 a0
#Print String after running code
la a0 AfterString
jal print_str
la a0 InputString
jal print_str
jal print_newline
#Print return value
la a0 ReturnValueString
jal print_str
mv a0 s0
jal print_int
jal print_newline
#Exits the program
li a0 10
ecall



UpperSum:
#Reads through the string, and converts the entire string to UPPERCASE
#Returns the sum of the ASCII values of each character in the ORIGINAL string
#YOUR CODE HERE



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