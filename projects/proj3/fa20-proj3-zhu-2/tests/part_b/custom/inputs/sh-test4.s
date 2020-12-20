addi s0, x0, 100
sh s0, 100(t0)
addi s0, s0, 2
sh s0, 102(t0)
lh t1, 100(t0)
lh t2, 102(t0)
