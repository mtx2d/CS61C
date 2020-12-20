addi t0, x0, 50
lui sp, 0x80000
addi sp, sp, -8
sb t0, 0(sp)
sb t0, 1(sp)
sb t0, 2(sp)
sw t0, 4(sp)

lb s0, 1(sp)
lw t1, 4(sp)


