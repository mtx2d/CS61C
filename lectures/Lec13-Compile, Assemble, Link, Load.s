Lecture 13: Compile, Assemble, Link, Load

Interpretation vs. Translation
- Interpreter: program that executes other programs (executes program in source language)
	- Useful to interpret machine language for simulators (VENUS) & backwards compatibility
- Translator convert program from source language into another language
	- Interpreters easier to write, closer to high level languages (better debugging),
	  slower/require more code, & can be run on any machine
	- Translated code usually more efficient & hide source program from users b/c the code
	  in the original language is not accessible
- Easier to program = less efficient to interpret w/ the converse being true
	- Easier to program: Python, Java, C++; Harder: C, Assembly, Machine Code
	- Interpret used when efficiency not needed, translate to low-level language to inc performance

Compiler/Assembler
- Compiler Input: high level language code (foo.c) --> Output: assembly language code (foo.s)
	- Output allowed to contain pseudo-instructions (assembler understands, but not machine)
- Assembler input: assembly language code (foo.s) --> Output: Object code/information tables (foo.o)
	- Assembler in charge of reading/using directives, replacing pseudoinstructions, producing
	  machine language, & creating the object file
- Assembler directives: directions received by assembler (do not create machine instructions, however)
	.text --> Items that belong to text segment (aka machine code)
	.data --> Items put in data segment (source file data)
	.globl sym --> Declares symbols as global (referencable in other files)
	.string str --> Stores strings in memory w/ null termination
	.word w1 w2... --> Store 32-bits in memory as words
- Pseudoinstructions: assembler replaces assembly code w/ the real instructions
	mb t0, t1 --> addi t0, t1, 0
	neg t0, t1 --> sub t0, zero, t1
	li t0, imm --> addi t0, zero, imm (Immediate is sign extended)
	not t0, t1 --> xori t0, t1, -1 (Immediate is sign extended)
	beqz t0, loop --> beq t0, zero, loop (Branch immediates counted in half words)
	la t0 str --> lui t0, str[31:12] # Static addressing--provides an absolute address
				  addi t0, t0, str[11:0] (Immediate is sign extended)
				  auipc t0, str[31:12] # PC-Relative addressing (relative to program counter)
				  addi t0, t0, str[11:0] # Keep in mind sign extension

Producing Machine Language
- For arithmetic/logical/shift instructions, machine language is simple
- Branches/Jumps use PC-relative addressing, which is accomplished once pseudoinstructions replaced
- Issue with forward referencing: possible to branch to future instructions
	- Take 2 passes in program to solve this problem: first pass to remember label positions
	  while the second pass generates code from these saved label positions
- j offset --> jal zero, offset: count number of instructions halfwords bw target & jump
- References to static data: la becomes lui &  addi
- Symbol Table: list of items in file that could be used by other files
	- Includes labels (for function calls) and Data (all components in .data section)
- Relocation Table: Items whose address file needs: absolute label jumps & data in static section
	- The contents here is everything that is "to be filled in later" once have the actual executable
		- Filling in is the job of the linker
	- Includes absolute jumps to jal, jalr & la instruction
- Object file format: includes header, text segment, data segment, relocation info, symbol table, debugging info
	- Header: size/position of other pieces of object file, text segment: machine code, data segment: binary rep
	  of static data in source file, relocation info: lines of code to be fixed, symbol table: list of file labels
	  & static data to be referenced, debugging info: standard format is ELF

Linker
- Input: object code files/info tables (foo.o, libc.o) --> Output: executable code (a.out)
- Combines multiple object files into a single executable (process is called linking)
	- Allows for separate compilation of files (changing 1 file does not require recompiling everything)
	- Each .o file contains text, data, info --> Linker will rearrange ordering to combine all text segments, etc.
- Steps: Smush text segments from each .o file together, then data segments, then resolve references
  by going through relocation table & filling in absolute addresses
- 4 forms of addresses: PC Relative, absolute function, external function, static data reference
	- PC relative does not require relocation (PIC: position independent code) but the other 3 do
- J-format requires relocation editing while I/S Format require relocating b/c of global pointer needs to point to the new 
  static area (contains bytes relative to static area)
- Assume first word of text segment at 0x1000; knows length/order of text/data segments and calculates
  absolute address of each label (both external and internal)
- Resolve references: search for reference in symbol tables, then library files to create the
  executable file w/ text, data, header
- Statically Linked Approach: whole executable is self-contained (no changes can be made)
- Dynamically Linked Libraries: Machine code is when the dynamic linking occurs (actually loading the library
  imports); requires less storage space & sending program updates faster but runtime overhead and executable can change

Loader
- Input: executable code (a.out), output: program actually being run
- Loader loads executable stored in disk into memory (loader = operating system)
- Tasks of the loader: read header to figure out text/data segment sizes, create address space for text/data/stack, 
  copy instructions/data from executable into address space, copy arguments into stack, initialize machine registers,
  start program w/ arguments (setting the PC)

Example of Compile --> Assemble --> Link --> Load

# C PROGRAM (Hello.c)

#include <stdio.h>
int main() {
	printf("Hello %s\n", "world");
	return 0;
}

# Assember Language Compilation (Hello.s)

.text # Directive to indicate entering text portion of the code
	.align 2 # Code is going to be aligned by 2^2 = 4bytes
	.global main # main is declared a global symbol
main: # Label to indicate start of main
	addi sp, sp, -16 # Decrement stack pointer to set up storage
	sw ra, 12(sp) # Save the return address to the stack
	lui a0, %hi(string1) # With line below, calculates address of string1
	addi a0, a0, %lo(string1)
	lui a1, %hi(string2) # With line below, calculates addresss of string2
	addi a1, a1, %lo(string2)
	call printf # Call function printf using 2 strings as arguments
	lw ra, 12(sp) # Restore the return address
	addi sp, sp, 16 # Increment stack pointer to "free" the storage
	li a0, 0 # Load return value of 0
	ret # Return (0 if there isn't an error)
	.section .rodata # Directive to indicate entering read-only data section
	.balign 4 # Directive to align data section to 4 bytes
string1: # Label for string1
	.string "Hello, %s!\n" # Directive to create a null terminated string
string2: # Label for string2
	.string "world" # Directive to create another null terminated string

# Result of Linker (Hello.s --> Hello.o)

00000000: ff010113 (addi sp, sp, -16)
00000004: 00112623 (sw ra, 12(sp))
00000008: [00000]537 (lui a0, 0x0) [00000]-->00021 (lui a0, 0x21)
0000000c: [000]50513 (addi a0, a0, 0) [000]-->a10 (addi a0, a0, -1520)
00000010: [00000]5b7(lui a1, 0x0) [00000]-->00021 (lui a1, 0x21)
00000014: [000]58593 (addi a1, a1, 0) [000]-->a1c (addi a1, a1, -1508)
00000018: [00000097] (auipc ra, 0x0)
0000001c: [000]080e7 (jalr ra) --> 28800(jal ra, 10450)
00000020: 00c12083 (lw ra, 12(sp))
00000024: 01010113 (addi sp, sp, 16)
00000028: 00000513 (addi a0, a0, 0)
0000002c: 00008067 (jalr ra)

- Note: [000],[00000] act as address placeholders post-assemble pre-link
	- Linking actually fills in these addresses (along with converting them into bytes)
- Let us try to create an instruction to obtain target address 0x0020A10
	- lui 0x00020, addi 0xA10 fails b/c of sign extension: 00020 000 + FFFFF A10 = 0001F A10
- Solution: lui 0x00021, addi 0xA10 where 0xA10 becomes 0xFFFFFA10 b/c of sign extension, 
  which we can calulate out to be -1520 --> Hence, we have the instruction addi a0, a0, -1520






