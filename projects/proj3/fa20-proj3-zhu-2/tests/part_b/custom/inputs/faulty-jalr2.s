addi t0, x0, 10
addi t0, t0, 10
addi t0, t0, 10
addi t0, t0, 10
addi t0, t0, 10
addi s0, x0, 24
jalr ra, s0, 8
addi t0, t0, 4
label1: jal ra, label2
addi t1, x0, 4
label2: jalr ra, s0, 20
addi t0, x0, 0
addi t0, x0, 0
addi t0, x0, 0
addi t0, x0, 0
addi t0, x0, 0
addi t0, x0, 0

beq t0, x0, label3
	addi t0, t0, 1
label3: bne t0, x0, label4
	addi t0, x0, 5
label4:

addi t0, x0, 0
label5: