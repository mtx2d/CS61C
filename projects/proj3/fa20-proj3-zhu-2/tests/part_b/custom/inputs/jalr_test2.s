addi s0, x0, 1
addi t0, x0, 5
label:
	jalr ra, t0, 8
addi s1, x0, 1
label_new:
	addi s0, s0, 5
