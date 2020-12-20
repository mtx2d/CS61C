.globl reverse

.data
test_arr: .word 1 2 3 4

before_string: .asciiz "Input should be '1 2 3 4', and it is: "
after_string: .asciiz "After calling, it should be '4 3 2 1', and it is: "
space: .asciiz " "

.text
main:
    la a0, before_string
    jal print_str
    # Convert arr to linked list
    la a0, test_arr
    li a1, 4
    jal arr_to_ll
    mv s0, a0
    jal print_ll
    jal print_newline
    la a0, after_string
    jal print_str
    # Call reverse
    mv a0, s0
    li a1, 0
    jal reverse
    mv s0, a0
    # Print result
    jal print_ll
    jal print_newline
    # Free memory
    mv a0, s0
    jal free_ll
    # Exit
    li a0, 10
    ecall

# Struct layout:
#
# typedef struct node_struct {
#     int32_t value;
#     struct node_struct *next;
# } Node;
#
# Arguments:
# - a0: node, the head of the linked list
# - a1, prev, the originally previous node (NULL on the first call to this function)
# Returns: the new head of the linked list
#
reverse:
    # YOUR CODE HERE
    addi sp, sp, -4
    sw ra, 0(sp)
    #
    lw t0, 4(a0) # second = node->next
    sw a1, 4(a0) # node->next = prev
    beq t0, x0, end
    mv a1, a0    # node argument
    mv a0, t0    # "second" argument
    jal reverse
end:
    lw ra, 0(sp)
    addi sp, sp, 4
    ret


# === Utility functions ===

# Prints the int in a0.
print_int:
    mv a1, a0
    li a0, 1
    ecall
    ret

# Prints the string in a0
print_str:
    mv a1, a0
    li a0, 4
    ecall
    ret

# Prints a newline.
print_newline:
    li a1, '\n'
    li a0, 11
    ecall
    ret

# === YOU CAN IGNORE ALL THE CODE BELOW THIS LINE ===
# Though we can't reveal exactly what the exam looks like, it's unlikely
# that you'll need to do these sorts of complicated data structure
# manipulations on the real exam (though you should be able to understand
# how they are written). These functions are included in order to make
# it possible to test this old exam problem.

# Converts the int array in a0 with a1 elements into a linked list
arr_to_ll:
    addi sp, sp, -16
    sw ra, 0(sp)
    sw s0, 4(sp)
    sw s1, 8(sp)
    sw s2, 12(sp)
    mv s0, a0
    mv s1, a1
    beqz s1, arr_to_ll_base_case
    # malloc sizeof(Node)
    li a0, 8
    jal malloc
    # Copy value from array and set next to NULL
    mv s2, a0
    lw t0, 0(s0)
    sw t0, 0(s2)
    sw zero, 4(s2)
    # Perform recursive call to get node for next element
    addi a0, s0, 4
    addi a1, s1, -1
    jal arr_to_ll
    # Set next field
    sw a0, 4(s2)
    mv a0, s2
    j arr_to_ll_end
arr_to_ll_base_case:
    li a0, 0
arr_to_ll_end:
    lw s2, 12(sp)
    lw s1, 8(sp)
    lw s0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 16
    ret

# Recursively frees the linked list in a0.
free_ll:
    addi sp, sp, -8
    sw ra, 0(sp)
    sw s0, 4(sp)
    # Base case: null pointer
    beqz a0, free_ll_end
    # Free child
    mv s0, a0
    lw a0, 4(s0)
    jal free
    # Free self
    mv a0, s0
    jal free
free_ll_end:
    lw s0, 4(sp)
    lw ra, 0(sp)
    addi sp, sp, 8
    ret

# Returns a pointer with a0 bytes allocated
malloc:
    mv a1, a0
    li a0, 0x3CC
    addi a6, x0, 1
    ecall
    ret

# Frees the malloc'd pointer in a0
free:
    mv a1 a0
    li a0 0x3CC
    addi a6 x0 4
    ecall
    ret

.text
# Prints the linked list in a0.
print_ll:
    addi sp, sp, -8
    sw s0, 0(sp)
    sw ra, 4(sp)
    mv s0, a0
print_ll_loop:
    beq s0, zero, print_ll_end
    lw a0, 0(s0)
    jal print_int
    lw s0, 4(s0)
    la a0, space
    jal print_str
    j print_ll_loop
print_ll_end:
    lw s0, 0(sp)
    lw ra, 4(sp)
    addi sp, sp, 8
    ret