Lecture 3--Intro to C

History of Computer Organization
- ENIAC (1946): first electronic general purpose computer
- EDSAC (1949): first general stored program computer (programs held as numbers in memory)
- C is an example of a high level language program (not manipulating bits directly)
  - C provides flexibility to use base features in computer architecture
  - Also dangerous b/c C has UNSAFE behavior w/ pointers, arrays, memory management
    - Might run fine locally but have inconsistent behavior for others

Compilation vs. Interpretation
- C compiler: takes C --> converts into architecture specific machine code
  - For Java, byte code is architecture independent
  - C differs from other languages in when program is converted to machine code
- C process: .c files compiled into .o files; then, .o files are linked into
  executables. Assembling occurs by default
    - CAL: compiling, assembling, linking
- Advantages of compilation: good run time (optimizes for given architecture) &
  decent compilation time (Makefiles allow only modified files to be recompiled)
- Major disadvantage: all compiled files are architecture specific (depend on CPU
  and operating system)
- C preprocessor (CPP): source files must go through macro processor
  - Replaces comments w/ space, adds include files, defines constants, etc.
  - Can run -save-temps in gcc to see what happens during preprocessing

Java vs. C
- Java characteristics: Objected-oriented, methods, class libraries for data
  structures, automatic memory management
    - High memory overhead for class libraries, relatively slow, arrays initialize
      to zero
- C characteristics: no built-in object abstraction, function-oriented, libraries are
  lower level, manual memory management (malloc, free), uses pointers, has preprocessor
  - Low memory overhead, relatively fast, arrays initialize to garbage by default


ANSI-C Updates
- Update to C is called C99
- Declare variables within for loops (for int i = 0; ...)
- // Comments are allowed
- Variable-length non-global arrays (used to have to be a constant)
- <inttypes.h>: explicit integer types
  - Tells us how large each integer is
- <stdbool.h> gives boolean logic definitions
- C11 Updates (update to C99): Multi-threading, unicode strings/constants, no more gets(),
  type-generic, macros, supports complex values, static assertions


C basic syntax
- Syntax for main function in a C program: int main(int argc, char *argv[])
  - argc: # of strings in command line
  - argv: pointer to array containing strings as arguments
  - Return type is int b/c return value = 0 says program ran correctly, but
    anything else indicates an error
- FALSE values in C: 0, NULL (no built-in Boolean values)
  - All other values are true
- Typed variables: int, unsigned int, float, double, char, long, long long
  - int (at least 16 bits), long >= 32 bits, long long >= 64 bits
- Constant: value assigned during declaration (cannot change during execution)
  - Example: const float goldenRatio = 1.618
- Enum: group of integer constants
  - Example: enum color {RED, GREEN, BLUE};
- Variables/Functions must be declared before use
- Typedef: allows new types to be defined
  - Example: typedef uint8_t BYTE; BYTE b1, b2;
- Struct: group of variables
typedef struct {
  int lengthInSeconds;
  int yearRecorded;
} SONG;
SONG s1;
s1.lengthInSeconds = 500; // Uses dot notation to set/access
s1.yearRecorded = 1990;


