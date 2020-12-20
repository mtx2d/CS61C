Lecture 23: Pipelining III

Load Data Hazards

lw s2, 20(s1)
and s4, s2, s5 # Unable to forward the results of lw into this instruction as s2 only properly updated after memory stage
or s8, s2, s6
add s9, s4, s2
slt s1, s6, s7
- Potential Solution 1: Implement 1 cycle pipeline stall
	- Convert instruction after lw to nop if one of the operands is the destination register of lw instruction
	- Hardware Implementation: Cancel writing back to the destination register (and still executed)
	- Downsides: results in more code space and performance loss
- Solution 2: reorder instructions so that instruction not dependent on result of lw occurs directly after lw
	- Thus, the instruction that originally depended on load word is "stalled" and can now use forwarding if needed
	- Results in no performance loss

Example of Solution 2: a[3] = a[0] + a[1]; a[4] = a[0] + a[2];

Original         Alternative

lw t1, 0(t0)     lw t1, 0(t0)   
lw t2, 4(t0)     lw t2, 0(t0)
# ORIGINAL: Line below depends on t2 (load data hazard!)
add t3, t1, t2   lw t4, 8(t0) # No longer depends on t2
sw t3, 12(t0)    add t3, t1, t2 # Can now safely access t2
lw t4, 8(t0)	 sw t3, 12(t0)
# ORIGINAL: Line below depends on t4 (load data hazard!)
add t5, t1, t4   add t5, t1, t4 # Can also now safely access t4
sw t5, 16(t0)	 sw t5, 16(t0)

Control Hazards
- Insight: if branch not taken, can continue executing instructions w/o issue
- Otherwise, need to "flush" incorrect instructions from pipelining (converting them to NOPs)
	- Results in 2 cycles spent on NOPs... but realize that branches used w/ loops (branch mostly untaken)
	- To improve, make predictions about when branch is taken. That way, do not have to flush as often

beq t0, t1, LABEL # Assume the branch is taken
sub t2, s0, t0 # Convert instruction to nop
or t6, s0, t3 # Convert instruction to nop
LABEL: xor t5, t1, s0

Superscalar Processors
- 3 strategies to improve processor performance: increase clock rate, pipelining, multi-issue superscalar processor
	1. Clock rate: limited by tech & power dissipation (inc clock rate --> requires more power)
	2. Pipelining: saw how shorter clock cycle can be used but also increases possibility for hazards
	3. Multiple Issue Superscalar: replicate pipeline stages using multiple pipelines
		- Allows for more instructions to be initiated every clock cycle --> CPI < 1
		- Out of Order Execution: reorder instructions in order to reduce hazards
- Setup of superscalar processor: IF/Decode Unit --> Pipelines for: lw/sw, fp, int operations... --> Commit unit
	- Ideally can get CPI < 1, but "ideal" performance differs from reality b/c of necessity of stalls/hazards

ISA Design
- RISC-V design enables pipelining b/c: 32 bit instructions, fewer/regular instruction formats, load/store addressing, memory alignment
	- Can fetch/decode instructions in a single cycle
	- Decode/Read registers occur in one fell swoop
	- Step 3 calculates address, Step 4 (directly after) accesses memory
	- Memory access takes but 1 cycle
	




