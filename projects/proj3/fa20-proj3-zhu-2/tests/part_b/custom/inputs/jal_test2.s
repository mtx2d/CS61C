addi s0, x0, 1
addi t0, x0, 5
label:
	addi s0, s0, 1
	beq s0, t0, label_new
	jal ra, label
addi s1, x0, 1
label_new:
	jal ra, label_next
	addi t0, t0, 1
	addi t0, t0, 1
	addi t0, t0, 1
label_next:
