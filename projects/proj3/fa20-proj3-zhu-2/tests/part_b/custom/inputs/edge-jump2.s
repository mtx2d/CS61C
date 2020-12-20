addi a0, x0, 3
label1: 
addi a0, a0, -1
beq a0, x0, label2
jal ra, label1
label2:
	addi a0, x0, 55
	addi ra, x0, 55
    beq t0, x0, sub
	beq t0, x0, label3
sub:	jalr ra, a0, -24
label3:
	addi a0, x0, 0