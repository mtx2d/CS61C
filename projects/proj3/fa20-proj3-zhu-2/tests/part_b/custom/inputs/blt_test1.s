addi s0, x0, -1
addi s1, x0, 1
blt s1, s0, label
addi s1, x0, -5
blt s1, s0, label
addi s0, s0, 1
label:
	addi t0, x0, 15
	addi t1, x0, 10
	blt t0, t1, label
	addi t0, t0, -10
	blt t0, t1, label1
	addi t0, t0, 1
label1:

