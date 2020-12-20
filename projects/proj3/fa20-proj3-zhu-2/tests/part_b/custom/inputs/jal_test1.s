addi s0, x0, 1
jal ra, label
addi s1, x0, 1
label:

addi t0, x0, 1
addi t0, t0, 1
addi t0, t0, 1
addi t1, x0, 20
label1:
	beq t0, t1, label2
	addi t0, t0, 1
	jal ra, label1
label2:
