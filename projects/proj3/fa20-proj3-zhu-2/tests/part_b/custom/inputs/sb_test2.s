addi t0, x0, 0x333
addi t1, x0, 50
sb t0, 100(s0)
lb t1, 100(s0)
addi t0, t0, 1
sb t0, 101(s0)
lb t2, 101(s0)
addi t0, t0, 1
sb t0, 102(s0)
lb ra, 102(s0)
addi t0, t0, 1
sb t0, 103(s0)
lb a0, 103(s0)

	