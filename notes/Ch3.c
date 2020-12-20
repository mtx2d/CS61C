#include <stdio.h>

/*
Switch Statements
- switch: allows for multi-way decisions

Syntax:

switch (expr)
{
	case const-expr: statements
	case const-expr: statements
	default: statements
}

- If case matches expr: execute statements under that case
	- Keep executing the statements, regardless of case, until a break is reached
- If no case matches expr: execute default
- Good for "cumulative" commands
*/

/* Function to show grade using switch statement */
void gradeMessage(char grade)
{
	switch (grade)
	{
		case 'A':
			printf("Excellent!\n");
			break;
		case 'B':
		case 'C':
			printf("A passing grade...\n");
			break;
		case 'D':
			printf("You failed\n");
			break;
		case 'F':
			printf("You really failed...\n");
			break;
		default:
			printf("Invalid input\n");
			break;
	}
}

/*
do while loop
- Statement gets executed first, THEN expression is evaluated
	- Then, so long as the expresion is true, the statement will continue to be executed
Syntax

do {
	statement
} while (expression);

*/

/* Function that shows a do while loop */
void printFromZero (int x)
{
	int count = 0;
	do {
		printf("%i ", count++);
	} while (count <= x);
	printf("\n");
}

/*
Break/Continue
- Break: Used to exit a loop
- Continue: Forces next execution of a loop
*/

int main()
{
	char grade = 'B';
	printf("Grade message for grade %c: ", grade);
	gradeMessage(grade);
	printFromZero(10);

}

/*
Additional Notes
*/