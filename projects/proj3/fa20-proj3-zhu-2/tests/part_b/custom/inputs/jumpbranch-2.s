addi t0, x0, 10
addi t0, t0, 10
addi t0, t0, 10
addi t0, t0, 10
addi t0, t0, 10
addi s0, x0, 24
addi t1, x0, 0
jalr ra, s0, 8
jalr ra, s0, 16
addi t0, t0, 4
label1: beq t1, x0, label2
addi t0, t0, 4
label2: beq t1, x0, label3
addi t0, t0, 4
label3: jalr ra, s0, 36
addi t0, t0, 4
addi t0, t0, 4