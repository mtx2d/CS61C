#include <stdio.h>


char test[2];
char* test2;

void populate()
{
	printf("First: %c %c\n", test[0], test[1]);
	test[0] = 'a';
	test[1] = 'b';
	printf("Second: %c %c\n", test[0], test[1]);
}

void populate2()
{
	char temp[3] = "CD";
	test2 = temp;
	printf("New First: %c %c\n", test2[0], test2[1]);
	// printf("Second: %c %c\n", test[0], test[1]);
}
int main()
{
	populate();
	printf("Third: %c %c\n", test[0], test[1]);
	populate2();
	printf("New second: %c %c\n", test2[0], test2[1]);
}