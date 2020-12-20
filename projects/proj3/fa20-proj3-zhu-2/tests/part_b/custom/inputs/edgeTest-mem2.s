addi t0, x0, 50
lui sp, 0x10080
sb t0, 0(sp)
sb t0, 1(sp)
sb t0, 2(sp)
sh t0, -2(sp)
sh t0, -4(sp)
sh t0, 4(sp)


lb s0, 1(t1)
lh t1, -2(sp)