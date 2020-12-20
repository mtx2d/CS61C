addi t0, x0, 5
addi t1, x0, 5
addi s0, s0, 1
addi s0, s0, 10
beq t0, t1, label
addi s0, s0, 10
label: jal ra, label2
addi s0, s0, 1
addi s0, s0, 10
label2: bne s0, x0, label3
addi s0, x0, 5
label3: jal ra, label4

label4:
	addi s0, s0, 1
	addi s0, s0, 1

addi t0, x0, 5
addi t1, x0, 5

label6: jal ra, label5
addi t0, t0, 1
addi s0, s0, 1
label5: bne t0, t1, label6
	addi s0, s0, 5




