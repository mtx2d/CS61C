#include <stdio.h>


/*
cc command
Example syntax: cc main.c getline.c strindex.c
- cc command compiles the 3 files (main.o, getline.o, strindex.o) & loads 
  an executable (a.out)
*/

/*
External Variables
- External variables defined outside any function
	- Have the property of being globally accessible
*/

int x; // External variable defined (space is allocated) & declared (value set)
double xArr[10]; // Size must be specified when not using extern notation

extern int y; // y is declared as an int, yArr is declared as a double array
extern double yArr[]; // Variables not created, storage not reserved

/*
Static Variables
- Internal static variables: provide private but peramnent storage within a function
	- Stay in existence even when functino finishes executing
- External static declaration: scope of an object is limited to the source file
*/

static int z; // This integer is only accessible within this source file now

/*
Register Variables
- register declaration: tells compiler variable will be used often
*/

int x1;
int y1;
void func(double x1)
{
	double y1;
	/*
	Within this function, any reference to x1 is the parameter
	Any reference to y1 will refer to the double declared
	But outside this function the int x1, y1 will be used
	*/
}

/*
File Inclusion
- Syntax: #include "filename" OR #include <filename>
- #include "filename": search for filename where source program found
	- If not found, follow implementation-defined rule for finding a file
- #include <filename>: directly use implementation-defined rule for finding file
*/

/* Using Macros */
#define MAX_LENGTH 100 // Any reference to MAX_LENGTH replaced w/ 100
#define forever for(;;) // forever results in an infinite loop
#define max(A, B) ((A>B) ? (A):(B)) // Define a macro with arguments
#undef getMax // Undefining a name--usually to ensure routine is a function

/* 
Conditional Inclusion
- #if evaluates constant integer expression
	- If it's nonzero --> execute lines until #endif/#elif/#else reached
- #ifdef: test whether a name is defined
- #ifndef NAME: if NAME is not defined, define. #endif
*/

int main()
{
	printf("Macro: %i\n", MAX_LENGTH);
	printf("Function macro: %i\n", max(5+4, 3*2));
}