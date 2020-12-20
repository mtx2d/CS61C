.globl read_matrix

.text
# ==============================================================================
# FUNCTION: Allocates memory and reads in a binary file as a matrix of integers
# FILE FORMAT:
#   The first 8 bytes are two 4 byte ints representing the # of rows and columns
#   in the matrix. Every 4 bytes afterwards is an element of the matrix in
#   row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is a pointer to an integer, we will set it to the number of rows
#   a2 (int*)  is a pointer to an integer, we will set it to the number of columns
# Returns:
#   a0 (int*)  is the pointer to the matrix in memory
# Exceptions:
# - If malloc returns an error,
#   this function terminates the program with error code 88.
# - If you receive an fopen error or eof, 
#   this function terminates the program with error code 90.
# - If you receive an fread error or eof,
#   this function terminates the program with error code 91.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 92.
# ==============================================================================
read_matrix:

    # Prologue
    addi sp, sp, -52
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	sw s5, 20(sp)
	sw s6, 24(sp)
	sw s7, 28(sp)
	sw s8, 32(sp)
	sw s9, 36(sp)
	sw s10, 40(sp)
	sw s11, 44(sp)
	sw ra, 48(sp)

	mv s6, a0 # pointer to filename
	mv s7, a1 # pointer to # rows (height)
	mv s8, a2 # pointer to # cols (width)

	# Let s5 contain whether an error occurred or not
	# Corresponding--0: no error, 1: open, 2: read, 3: malloc, 4: close
	add s5, x0, x0

# OPEN FILE

open:
	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)

	# Set the arguments
	mv a1, s6 # Set a1 = pointer to filename
	add a2, x0, x0 # Set a2 = 0 for read mode

	# Call the function
	jal ra, fopen
	# Save the file descriptor: Let s0 contain the file descriptor
	mv s0, a0


	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28
	

	# Check for failure
    
    #ebreak
	addi t0, x0, -1 # t0 = -1
	addi s5, x0, 1
	beq s0, t0, done # If file descriptor (s0) == -1 --> go directly to done
	mv s5, x0


read_height:
	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)

	# Set arguments
	mv a1, s0 # a1 contains file descriptor
	mv a2, s7 # a2 contains buffer to be written to
	addi a3, x0, 4 # a3: Read 4 bytes (a single integer)

	# Call the function
	jal ra, fread

	# Save bytes read--Let s9 = # bytes read by read_height
	mv s9, a0

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Encountered failure--branch to failure case
	addi t0, x0, 4
	addi s5, x0, 2
	bne s9, t0, done # If s9 != t0 --> go directly to done

	# No failure
	#ebreak
	lw s1, 0(s7) # Let s1 contain # rows
	mv s5, x0
	

# Read the width
read_width:
	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)

	# Set arguments
	mv a1, s0 # a1 contains file descriptor
	mv a2, s8 # a2 contains buffer to be written to
	addi a3, x0, 4 # a3: Read 4 bytes (a single integer)

	# Call the function
	jal ra, fread

	# Save # bytes actually read by read_width
	mv s10, a0 # s10 = # bytes actually read

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28


	# Encountered failure--branch to failure case
	addi t0, x0, 4
	addi s5, x0, 2
	bne s10, t0, done # If s10 != t0 --> Go directly to done

	# No failure
	lw s2, 0(s8) # Let s2 contain # cols
	mv s5, x0

malloc_data:

	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)
	
	# Set the arguments
	# Let s3 hold the dimensions of the array to be alloced
	mul s3, s1, s2 # s3 = s1 * s2 --> Give us width * height
	slli a0, s3, 2 # a0 = 4 * width * height (actual # bytes of the matrix)
	# Call malloc
	#ebreak
	jal ra, malloc

	# Save pointer to heap memory returned by malloc
	# Let s4 hold the pointer to the malloc'ed data
	mv s4, a0

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Test for failure
    #ebreak 
    addi s5, x0, 3
	beq s4, x0, done # If s4 (pointer) == 0 --> go directly to done
	mv s5, x0

read_data:
	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)

	# Set arguments
	mv a1, s0 # a1 = file descriptor
	mv a2, s4 # a2 = pointer to malloc'ed data
	slli a3, s3, 2 # a3 = 4 * s3 (# bytes to be read by the matrix)

	# Call the function
	jal ra, fread

	# Save # bytes actually read by read_data
	mv s11, a0

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28


	# Encountered failure--branch to failure case
	slli t0, s3, 2 # t0 = 4 * s3 = # bytes expected to have been read

	# Failure case
	addi s5, x0, 2
	bne s11, t0, done # If bytes actually read (s11) == expected bytes read --> close_file
	mv s5, x0


close_file:
	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)

	# Set arguments
	mv a1, s0 # a1 = file descriptor

	# Call function
	jal ra, fclose

	mv s6, a0 # s6 = whether close_file was a success or not
	
	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Check for failure
	addi s5, x0, 4
	bne s6, x0, done
	mv s5, x0

done:

	mv t0, s5 # Move error code into t0 from s5
	bne t0, x0, epilogue # If t0 != 0 --> Go to epilogue
	mv a0, s4 # Else, save pointer to heap (s4) in a0 to be returned

epilogue:

	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	lw s5, 20(sp)
	lw s6, 24(sp)
	lw s7, 28(sp)
	lw s8, 32(sp)
	lw s9, 36(sp)
	lw s10, 40(sp)
	lw s11, 44(sp)
	lw ra, 48(sp)
	addi sp, sp, 52



	# Checking for failures
	addi t1, x0, 1 # t1 used to check error code
	beq t0, t1, error_fopen # t0 = 1 --> error_fopen
	addi t1, t1, 1
	beq t0, t1, error_fread # t0 = 2 --> error_fread
	addi t1, t1, 1
	beq t0, t1, error_malloc # t0 = 3 --> error_malloc
	addi t1, t1, 1
	beq t0, t1, error_fclose # t0 = 4 --> error_fclose

	# No failures: t0 = 0 --> ret
	#ebreak
	ret

# Set up the error cases
error_malloc:
	li a1, 88
	jal exit2
error_fopen:
	li a1, 90
	jal exit2
error_fread:
	li a1, 91
	jal exit2
error_fclose:
	li a1, 92
	jal exit2

