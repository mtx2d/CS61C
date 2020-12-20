addi t0, x0, 50
lui sp, 0x10000
sb t0, 0(sp)
sb t0, 1(sp)
sb t0, 2(sp)
sh t0, -2(sp)
sh t0, -4(sp)
sh t0, 4(sp)

lw s0, 4(sp)
lh a0, -2(sp)


lui t1, 0x10000
addi t1, t1, -4
sw t0, 0(t1)
sb t0, 4(t1)
sh t0, 2(t1)

lw s0, 0(t1)
lh a0, 2(t1)




