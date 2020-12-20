addi s0, x0, 1
addi s1, x0, 1
beq s0, x0, label
beq s0, s1, label
addi s0, s0, 1
label:

addi t0, x0, 5
addi t1, x0, 6
beq t0, t1, label
beq t0, t1, label1
addi t2, x0, 7
label1:
