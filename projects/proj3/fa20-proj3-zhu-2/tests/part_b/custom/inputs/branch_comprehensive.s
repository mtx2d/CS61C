addi s0, x0, 1
addi s1, x0, 2
bne s1, s2, label1
addi s0, s0, 1
label1:
	addi s0, x0, 1
	addi s0, s0, 1
	addi t0, x0, 3
beq s0, t0, label2
addi t0, x0, 4
label2:
	addi t0, x0, 4
	addi t1, x0, 4
bne t0, t1, label3
addi t0, x0, 5
label3:
	addi t0, x0, 1
	addi t1, x0, 4
blt t0, t1, label4
addi t0, x0, 2
label4:
	addi t0, x0, 3
addi t0, x0, 7
addi t1, x0, 3
blt t0, t1, label5
addi t0, x0, 20
label5:
	addi t1, x0, 9
addi t0, x0, -1
addi t1, x0, -1
bltu t0, t1, label6
addi t0, x0, 1
label6:
	addi t0, x0, 40
addi t0, x0, -2
addi t1, x0, -1
bge t0, t1, label7
addi t0, x0, 4
label7:
	addi t0, x0, 45

