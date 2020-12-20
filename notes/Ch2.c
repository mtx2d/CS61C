#include <stdio.h>
#include <math.h>
// Function to convert an char to lower case
int lower(int c)
{
	/* Uses the fact that a char is an integer */
	if (c >= 'A' && c <= 'Z')
	{
		return c + 'A' - 'a';
	}
	return c;
}

/*
Arithmetic implicit conversion rules (in order of precedence)
- Convert to long double
- Convert to double
- Convert to float
- Convert char/short to int
- Convert to long
*/

/*
Conditional Expressions
- Example: expr1? expr2 : expr3
	- Evaluate expr1. If true, expr2 is evaluated. Else, expr3 is evaluated
- More succinct form than an if expression
*/
int main()
{
	printf("Test\n");
	int n = 5;
	double rootN = sqrt((double)n); // n is cast as a double
	printf("The root is: %f \n", rootN);
	int a = 2, b = 7;
	int z = (a > b) ? a : b; // z is set to the max of a and b
	a = 15;
	printf("Bitwise right: %i\n", a >> 1);
	printf("Bitwise left: %i\n", a << 1);
	printf("Bitwise AND: %i\n", a & 19);
	printf("Bitwise OR: %i\n", a | 110);
}