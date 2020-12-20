lui t0, 0xDEADC
addi t0, t0, -222
addi s0, x0, 0
lui s0, 10000
sw t0, 100(sp)
sw t0, 101(sp)
sw t0, 102(sp)
sw t0, 103(sp)
sh t0, 110(sp)
sh t0, 111(sp)
sh t0, 112(sp)
sh t0, 113(sp)
sb t0, 120(sp)
sb t0, 121(sp)
sb t0, 122(sp)
sb t0, 123(sp)

lw t1, 100(sp)
lw t1, 101(sp)
lw t1, 102(sp)
lw t1, 103(sp)
lh t1, 110(sp)
lh t1, 111(sp)
lh t1, 112(sp)
lh t1, 113(sp)
lb t1, 120(sp)
lb t1, 121(sp)
lb t1, 122(sp)
lb t1, 123(sp)