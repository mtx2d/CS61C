addi t0, x0, 0x333
addi t1, x0, 0x100


sb t0, 100(s0)
addi t1, t1, 1
sb t0, 101(s0)
addi t1, t1, 1
sb t0, 102(s0)
addi t1, t1, 1
sb t0, 103(s0)

lh a0, 100(s0)
lh s1, 101(s0)
lh ra, 102(s0)