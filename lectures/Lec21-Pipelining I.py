Lecture 21: Pipelining

Performance Measures
- Pipelining central to performance measurement & improvement (first need to define metrics for computer "performance")
- Computer performance measured by: program execution time (ex: time to update display), throughput 
(ex: number of server requests handled/hour), and energy per task (ex: tasks done per battery charge)
- Iron Law of Professor Performance: Time/Program = Instructions/Program * Cycles/Instruction * Time/Cycle
	1. Instructions/Program: determined by task, algorithm used (O(N) vs O(N^2)), compiler, Instruction Set Architecture (ISA)
	2. Average Clock Cycles per Instruction (CPI): ISA, processor implementation, equal to one in "our" 
	   RISC-V design (CPI = 1), complex instructions might require larger values (CPI > 1, such as strcpy),
	   while superscalar processors can yield better results: CPI < 1
	3. Time/Cycle (Frequency): Processor microprocessor, technology, power constraints (lower voltage = lower transistor speed)

Energy Efficiency
- Energy efficiency determined by CMOS (includes capacitor, voltage source, and ground)
- Energy/Program = Instructions/Program * Energy/Instruction; proportional to Capacitance * Voltage^2
	- Capacitance dictated by tech & processor features. Tradeoff b/w reducing capacitance & voltage to save on energy
	  which will often decrease performance from a time perspective
- Moore Law + reduced supply voltage = improved energy efficiency from historical perspective
	- Recently, inability to further reduce supply voltage w/o inc leakage power b/c cannot turn transistors off
- Performance (Tasks/Second) = Power (Joules/Second) * Energy Efficiency (Tasks/Joule)
	- Performance dictated both by power consumption and the energy efficiency given power as a central constraint
- Energy constrained systems (phones): better energy efficiency desirable to extend battery life
- Power constrained systems (datacenter): better energy efficiency --> same performance uses less power (save $$)

Introduction to Pipelining
- Idea: certain tasks can be streamlined to decrease the total amount of time required to run through them
- Example: 4 ppl need to use washer, dryer, folding station, and walk back (each task takes 1 hour)
	- Instead of waiting for 1 person to accomplish all 4 tasks before someone else starts, instead use washer 
	  after first person finishes, then dryer, folding station, etc.
	- Increase efficiency by more efficiently allocating resources that are not being used by someone
- Pipelining does not help w/ latency of individual tasks, but helps w/ overall throughput
	- Potential Speedup = number of pipe stages
	- Pipeline rate limited by slowest stage of the pipeline