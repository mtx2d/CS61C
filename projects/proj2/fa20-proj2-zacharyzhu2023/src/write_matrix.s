.globl write_matrix

.text
# ==============================================================================
# FUNCTION: Writes a matrix of integers into a binary file
# FILE FORMAT:
#   The first 8 bytes of the file will be two 4 byte ints representing the
#   numbers of rows and columns respectively. Every 4 bytes thereafter is an
#   element of the matrix in row-major order.
# Arguments:
#   a0 (char*) is the pointer to string representing the filename
#   a1 (int*)  is the pointer to the start of the matrix in memory
#   a2 (int)   is the number of rows in the matrix
#   a3 (int)   is the number of columns in the matrix
# Returns:
#   None
# Exceptions:
# - If you receive an fopen error or eof,
#   this function terminates the program with error code 93.
# - If you receive an fwrite error or eof,
#   this function terminates the program with error code 94.
# - If you receive an fclose error or eof,
#   this function terminates the program with error code 95.
# ==============================================================================
write_matrix:

    # Prologue
    addi sp, sp, -32
	sw s0, 0(sp)
	sw s1, 4(sp)
	sw s2, 8(sp)
	sw s3, 12(sp)
	sw s4, 16(sp)
	sw s5, 20(sp)
	sw s6, 24(sp)
	sw ra, 28(sp)

	mv s0, a0 # s0 = pointer to filename string
	mv s1, a1 # s1 = pointer to start of matrix in memro
	mv s2, a2 # s2 = # rows in matrix (HEIGHT)
	mv s3, a3 # s3 = # cols in matrix (WIDTH)

	# Let s4 contain whether an error occurred or not
	# Corresponding--0: no error, 1: open, 2: write, 3: close
	add s4, x0, x0

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
	mv a1, s0 # Set a1 = pointer to filename
	addi a2, x0, 1 # Set a2 = 1 for write mode

	# Call the function
	jal ra, fopen

	# Save the file descriptor: Let s5 contain the file descriptor
	mv s5, a0 


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


    ebreak
	addi t0, x0, -1 # t0 = -1
	addi s4, x0, 1
	beq s5, t0, done # If file descriptor (s0) == -1 --> go directly to done
	mv s4, x0

# Write # rows (HEIGHT)

write_height:

	# PROLOGUE
	addi sp, sp, -28
	sw a0, 0(sp)
	sw a1, 4(sp)
	sw a2, 8(sp)
	sw a3, 12(sp)
	sw t0, 16(sp)
	sw t1, 20(sp)
	sw ra, 24(sp)



# int fwrite(int a1, void *a2, size_t a3, size_t a4)
# Writes a3 * a4 bytes from the buffer in a2 to the file descriptor a1.
# args:
#   a1 = file descriptor
#   a2 = Buffer to read from
#   a3 = Number of items to read from the buffer.
#   a4 = Size of each item in the buffer.
# return:
#   a0 = Number of elements writen. If this is less than a3,
#    it is either an error or EOF. You will also need to still flush the fd.

	# Set arguments
	mv a1, s5 # a1 contains file descriptor (s5)
	addi t0, sp, 8 # t0 contains the address of pointer to a2's integer
	mv a2, t0 # a2 contains buffer to read from (sp contains pointer to the int)
	addi a3, x0, 1 # a3: Read 1 item (a single integer)
	addi a4, x0, 4 # a4: size = 4 (bytes)

	# Call the function
	jal ra, fwrite
	# Save # elements written
	mv s6, a0 # Let s6 check the # of elements

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Check for error
	ebreak
	addi t0, x0, 1
	addi s4, x0, 2
	bne s6, t0, error_fwrite # If # elements written != 1 --> error_fwrite
	mv s4, x0

# Write # cols (WIDTH)

write_width:
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
	mv a1, s5 # a1 contains file descriptor (s5)
	addi t0, sp, 12 # t0 contains the address of pointer to a2's integer
	mv a2, t0 # a2 contains buffer to read from (sp contains pointer to the int for # cols)
	addi a3, x0, 1 # a3: Read 1 item (a single integer)
	addi a4, x0, 4 # a4: size = 4 (bytes)

	# Call the function
	jal ra, fwrite
	# Save # bytes written
	mv s6, a0 # Let s6 check the # of elements

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Check for error
	ebreak
	addi t0, x0, 1
	addi s4, x0, 2
	bne s6, t0, error_fwrite # If # elements != 1 --> error_fwrite
	mv s4, x0

# Write matrix data
write_data:

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
	add t0, a1, x0 # t0 contains address of matrix w/ the data to be written to the file
	mv a1, s5 # a1 contains file descriptor (s5)
	mv a2, t0 # a2 contains buffer to read from (the address of the matrix)
	mul a3, s2, s3 # a3: # of items to read = width * height
	addi a4, x0, 4 # a4: size of items = 4 (bytes)

	# Call the function
	jal ra, fwrite
	# Save # bytes written
	mv s6, a0 # Let s6 check the # of bytes written

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Check for error
	ebreak
	mul t0, s2, s3 # t0 = a2 * a3 = width * height
	addi s4, x0, 2 # Set error code to 2
	bne s6, t0, error_fwrite # If # elements written != expected # elements --> error_fwrite
	mv s4, x0 # No error code reached--> s4 = 0

# Close the file
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

	# Set the arguments
	mv a1, s5

	# Call the function
	jal ra, fclose

	mv s6, a0 # s6 = whether or not an error was encountered in close file

	# EPILOGUE
	lw a0, 0(sp)
	lw a1, 4(sp)
	lw a2, 8(sp)
	lw a3, 12(sp)
	lw t0, 16(sp)
	lw t1, 20(sp)
	lw ra, 24(sp)
	addi sp, sp, 28

	# Check for error
	ebreak
	addi s4, x0, 3
	bne s6, x0, done
	mv s4, x0

done:

	mv t0, s4 # Move error code into t0 from s4

	# Epilogue
	lw s0, 0(sp)
	lw s1, 4(sp)
	lw s2, 8(sp)
	lw s3, 12(sp)
	lw s4, 16(sp)
	lw s5, 20(sp)
	lw s6, 24(sp)
	lw ra, 28(sp)
	addi sp, sp, 32

	# Branching to error cases--Checking for failures
	addi t1, x0, 1 # t1 used to check error code
	beq t0, t1, error_fopen # t0 = 1 --> error_fopen
	addi t1, t1, 1
	beq t0, t1, error_fwrite # t0 = 2 --> error_fwrite
	addi t1, t1, 1
	beq t0, t1, error_fclose # t0 = 3 --> error_fclose

	ret

# Set up the error cases
error_fopen:
	li a1, 93
	jal exit2
error_fwrite:
	li a1, 94
	jal exit2
error_fclose:
	li a1, 95
	jal exit2

