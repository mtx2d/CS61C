addi t0, x0, 0
addi t1, x0, 10
label:
	addi t1, t1, 1
	beq t0, t1, label
addi t1, t1, 5
addi t1, t1, 5
addi t1, t1, 5
addi t1, t1, 5
addi t1, t1, 0
addi t1, t1, 0
addi t1, t1, 0
addi t1, t1, 0
addi t2, x0, 95
label2: jalr ra, t2, 5
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1
addi t1, t1, 1