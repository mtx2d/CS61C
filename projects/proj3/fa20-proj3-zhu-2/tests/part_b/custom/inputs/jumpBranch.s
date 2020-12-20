addi t0, x0, 5
addi t1, x0, 1
label1:
	add t2, t2, t1
	addi t1, t1, 1
	bne t0, t1, label1
addi t0, x0, 10
addi t1, x0, 0
label2:
	beq t0, t1, label3
	addi t1, t1, 2
	jal ra, label2
label3:
	

