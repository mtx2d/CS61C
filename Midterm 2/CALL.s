Spring 2018 Midterm 2 Question 1
1. Computes offset for beq instruction
- Assembler makes a second pass in order to calculate offset needed for branches
2. Compute offset for jal instruction from different object file
- Actual offset across files only known once the files are smashed together (during linking)
	- Needs to be ready for the loader; hence, done by the Linker
3. Create symbol/relocation table
- Relocation/Symbol table created by the Assembler
	- Relocation Table: Lines of code linker is supposed to deal with (jumps to external labels, reference to static data, anything assembler did not find
	- Symbol Table: Record of data/files that can be used cross-files (all labels assembler has encountered)
4. Produces following instruction: add s0, s1, s2
- Converting instructions to machine code is job of the ASSEMBLER
5. Combines object files into single executable
- LINKING object files is a job of the Linker

Fall 2019 Final Question 1
1. Copy program from disk into physical memory
- Loader in charge of actually loading executable into memory
2. Remove pseudoinstructions
- Output of the compiler can contain pseudoinstructions, so it is the job of the assembler
  to get rid of them
  	- Present a machine-readable format
3. Determine increment size for pointer arithmeic
- Machine code does not distinguish b/w char, int, etc. so must be done by the compiler (which produces the machine code)
4. Incorporate statically linked libraries
- Linker in charge of adding the SLL
5. Incorporate dynamically linked libraries
- Dynamically linked libraries not available until runtime
	- Hence, must be done by the loader

Fall 2019 Midterm Question 2
1. Replaces immediate in jal label with final value (assume label lives in different file than jal)
- Final value not known until all the files get combined together (which only gets done by the LINKER)
2. Removes pseudoinstructions
- Removing pseudoinstructions is an explicit task of the assembler
	- Idea: the linker should be able to calculate offsets w/o having to worry about expanding pseudoinstructions
3. Outputs assembly language code
- Job of the LINKER is to produce the assembly code
4. Reads symbol table
- Symbol table produced by the assembler to be read by the LINKER
5. Copies arguments passed by program onto stack
- This is an explicit duty of the loader