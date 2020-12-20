addi s0, x0, 1
addi s1, x0, 5
label:
	addi s0, s0, 1
	bltu s0, s1, label
	bgeu s0, s1, label2
addi t0, x0, 5
label2:
	addi t0, t0, 1


