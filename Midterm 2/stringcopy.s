# Function to copy over string from source to dest register

# a0: pointer to destination reg
# a1: pointer to source reg
# a2: number of chars to copy

stringcopy:
	mv t0, x0 # t0 = counter
    mv t1, a1 # t1 = pointer to source reg
    
loop:
	beq t0, a2, done # If t0 == strlen --> go to done
    slli t1, t0, 2 # t1 = 4 * t0
    addi t1, a1, t1 # t1 = pointer to current element
    lb t1, 0(a1) # t1 = current elem of a1
    slli t2, t0, 2 # t2 = 4 * t0
    addi t2, a0, 2 # t2 = pointer to dest reg
    sw t1, 0(t2) # Store curr elem of a1 into curr elem of a0
    beq t1, x0, done # If t1 == null terminator --> go to done
    addi t0, t0, 1 # Increment t0
    j loop # Go back to beginning of loop
done:
	jr ra
	
	