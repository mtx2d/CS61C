addi s0, x0, -1
addi s1, x0, 1
bge s0, s1, label
addi s1, x0, 5
bge s1, s0, label
addi s0, s0, 1
label:
	addi t0, x0, 15
	addi t1, x0, 10
	bge t1, t0, label
	addi t0, t0, 30
	bge t0, t1, label1
	addi t0, t0, 1
label1:

