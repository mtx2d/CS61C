Disc 4-RISCV Intro & Control

2. What does the following RISCV code do?
int *arr = {1,2,3,4,5,6,0};
s0 holds address of index 0 in arr

a. lw t0 12(s0) # Load arr[3] into Register t0-->t0 now contains arr[3]: 4
b. sw t0 16(s0) # Store t0 into s0 offset by 16--> arr[4] = 4
c. 
	slli t1, t0, 2 # Shift logical left: t1 = t0 * 4
	add t2, s0, t1 # t2 = s0 + t1 --> t2 points to address of arr[t0]
	lw t3, 0(t2) # Load word from t2 into t3 --> t3 = arr[t0]
	addi t3, t3, 1 # t3 += 1 --> t3 = (original arr[t0]) + 1
	sw t3, 0(t2) # Store new t3 back into t2 (the one incremented by one) --> arr[t0] = origVal + 1
d. 
	lw t0, 0(s0) # Load from from s0 into t0 --> t0 = arr[0]
	xori t0, t0, 0xFFF # Flip the bits of t0--Result of not being able to specify an immediate of > 3 bits
	addi t0, t0, 1 # Add 1 to t0 --> t0 = -arr[0] (2's complement)

3. Translate b/w C and RISCV

a.
C Version
	s0 -> a, s1 -> b
	s2 -> c, s3 -> z
	int a = 4, b = 5, c = 6, z;
	z = a + b + c + 10;
RISCV Version
	addi s0, x0, 4 # Initialize a variable w/ addi
	addi s1, x0, 5
	addi s2, x0, 6
	add s3, s0, x0 # Initialize s3 to the value stored in s0
	add s3, s3, s1 # Sequentially adding the remaining variables
	add s3, s3, s2
	addi s3, s3, 10 # Adding a constant value of 10

b.
C Version
	s0 -> int *p = intArr;
	s1 -> a;
	#int *p; --Initialize a pointer to an array (implicitly done)
	*p = 0; # The value the pointer points to is now 0
	int a = 2; # Initialize another variable a (a = 2)
	p[1] = p[a] = a; # elem1 & elemA of array both set to a
RISCV Version
	sw x0, 0(s0) # Stores the value of 0 into s0 w/ no offset --> p[0] = 0
	addi s1, x0, 2 #s1 = 2 --> a = 2
	add t0, s1, x0 #t0 = s1 = 2 --> t0 = 2
	slli t0, t0, 2 #t0 *= 4 --> t0 = 8 (creating offset)
	add t0, t0, s0 #t0 += s0 --> Now points to s0 w/ an offset of t0 (t0 points to address of p[a])
	sw s1, 0(t0) # Store from s1 into t0 (no offset) --> p[a] = a
	sw s1, 4(s0) # Store from s1 into s0 (offset 4) --> p[1] = a

c.
C Version
	s0 -> a, s1 -> b
	int a = 5, b = 10;
	if(a + a == b) {
		a = 0;
	} else {
		b = a - 1;
	}
RISCV Version
	addi s0, x0, 5 # s0 = 5
	addi s1, x0, 10 # s1 = 10
	add t0, s0, s0 #t0 = 2 * s0 --> Using a temporary register
	bne t0, s1, else # If t0 != b --> go to else
		add s0, x0, x0 # If t0 == b --> s0 = 0 (a = 0)
		j done # Branch to done
	else:
		addi s1, s0, -1 # Else --> s1 = s0 - 1 (b = a - 1)
	done:

d.
RISCV Version
	addi s0, x0, 0 # Initialize s0 to 0
	addi s1, x0, 1 # Initialize s1 to 1
	addi t0, x0, 30 # Initialize t0 to 30
	loop:
		beq s0, t0, exit # Exit when s0 == t0
		add s1, s1, s1 # s1 *= 2
		addi s0, s0, 1 # s0 += 1
		jal x0, loop # Go back to beginning of loop
	exit: # Terminated the loop

C Version
	int i = 0;
	int product = 1;
	int limit = 30;
	while (i != limit) {
		product = product * 2;
		i++;
	}


e.
C Version
	s0 -> n, s1 -> sum
	assume n > 0 to start

	for(int sum = 0; n > 0; n--) {
		sum += n;
	}
RISCV Version
	# Assume n is implicitly stored in s0
	add s1, x0, x0
	add t0, s0, x0
	loop:
		beq t0 x0 done
			add s1, s1, s0
			addi t0, t0, -1
			jal x0, loop
	done:


4. RISCV with Arrays & Lists

int arr[6] = {3, 1, 4, 1, 5, 9};
# Assume arr starts at memory address 0xBFFFFF00
# Assume s0 contains address 0xBFFFFF00
struct ll* lst; # Assume first element at: 0xABCD0000
# Assume NULL pointer is memory address 0x00000000
# Assume s1 contains the address 0xABCD0000
struct ll {
	int val;
	struct ll* next;
}

a. 
	lw t0, 0(s0) # Load s0 w/ offset 0 into t0 --> t0 = arr[0]
	lw t1, 8(s0) # Load s0 w/ offset 8 into t1 --> t1 = arr[2]
	add t2, t0, t1 # t2 = t0 + t1 --> t2 = arr[0] + arr[2]
	sw t2, 4(s0) # Store t2 back into s0 (offset 4) --> arr[1] = arr[0] + arr[2]

b.
	loop: beq s1, x0, end # IF s1 == 0 --> exit loop (if it's a null pointer)
		lw t0, 0(s1) # Load s1 w/ offest 0 into t0 --> t0 = current node of ll
		addi t0, t0, 1 # t0 += 1 --> t0 += 1
		sw t0, 0(s1) # Shove result back into s1 w/ offset 0 --> ll(current node value) += 1 (increment each node val by 1)
		lw s1, 4(s1) # Load next address of s1 back into s1 --> s1 points to ll->next
		jal x0, loop # Go back to looping
	end:


c.
	add t0, x0, x0 # t0 = 0
	loop: 
		slti t1, t0, 6 # IF t0 < 6 --> t1 = 1, ELSE: t1 = 0
		beq t1, x0, end # IF t1 == 0 --> END
		slli t2, t0, 2 # t2 = 4 * t0 --> Setting offset for t2
		add t3, s0, t2 # t3 = s0 + t2 --> t3 is a pointer to offset address (arr[t0])
		lw t4, 0(t3) # Load t3 (offset 0) into t4 --> t4 = arr[t0]
		sub t4, x0, t4 # t4 = -t4 --> Negate the result
		sw t4, 0(t3) # Store t4 back into t3 (offset 0) --> arr[t0] = -arr[t0]
		addi t0, t0, 1 # t0 += 1 --> Move counter to next element
		jal x0, loop # Go back to loop
	end:
	# End result: negate the 6 elements of the array

6. Writing RISCV Functions: implement the following function

sumSquare(n)
	IF n > 0 --> return n^2 + (n-1)^2... + 1^2
	IF n <= 0 --> return 0

square(n)
	return n * n;



square:
	mul a0, a0, a0
sumSquare:
	# PROLOGUE
	addi sp, sp, -4
	sw s0, 0(sp)

	add s0, x0, x0
	loop:
		blti a0, 1, done
		addi sp, sp, -8
		sw a0, 0(sp)
		sw ra, 4(sp)
		jal square
		add s0, s0, a0
		lw a0, 0(sp)
		lw ra, 4(sp)
		addi sp, sp, 8
		addi a0, a0, -1
		jal x0, loop
	# EPILOGUE
	lw s0, 0(sp)
	addi sp, sp, 4
done:









