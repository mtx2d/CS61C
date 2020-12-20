sum:
	addi t0, x0, 5
	addi t1, x0, 5
	jalr x0, ra, 12

# new_test:
# 	addi a0, a0, 10
# 	addi a0, a0, 10
# 	jalr ra, a0, -4

addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi ra, x0, 68
jalr x0, ra, -12
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5
addi s0, s0, 5