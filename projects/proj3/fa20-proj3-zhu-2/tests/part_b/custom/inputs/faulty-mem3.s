lui t0, 0xDEADC
addi t0, t0, -222
addi s0, x0, 0
lui s0, 10000
sw t0, -4(sp)
sw t0, -5(sp)
sw t0, -6(sp)
sw t0, -7(sp)
sh t0, -12(sp)
sh t0, -13(sp)
sh t0, -14(sp)
sh t0, -15(sp)
sb t0, -20(sp)
sb t0, -21(sp)
sb t0, -22(sp)
sb t0, -23(sp)

lw t1, -4(sp)
lw t1, -5(sp)
lw t1, -6(sp)
lw t1, -7(sp)
lh t1, -12(sp)
lh t1, -13(sp)
lh t1, -14(sp)
lh t1, -15(sp)
lb t1, -20(sp)
lb t1, -21(sp)
lb t1, -22(sp)
lb t1, -23(sp)