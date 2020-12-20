# Adding to an array

# a0: pointer to an array
# a1: number of elements in the array
# a2: number to add to the array

addArray:
	mv t0, x0 # t0 = 0

loop:
	beq t0, a1, done # If t0 == num elements, go to done
	mv t1, t0 # t1 = t0
	slli t1, 2 # t1 = t1 * 4 (calculate the actual offset)
	add t1, a0, t1 # t1 = a0 + t1 (points to current element)
	lw t2, 0(t1) # t2 contains current element at specified index
	add t2, t2, a2 # t2 += a2--KEY: Can transform element of array in any which a way here
	sw t2, 0(t1) # t1 contains updated value in specified memory
	addi t0, t0, 1
	
done:
	jr ra