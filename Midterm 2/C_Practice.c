#include <stdio.h>

// CSM Midterm Review #1

int main2()
{
	char f[] = "CS61C is cool";
	char *g = f;
	int *h = f;
	
	// Part A
	printf("%c\n", *g); // C --> pointer to 0th elem in f
	// Part B
	printf("%c\n", *(g+1)); // S --> Pointer to 1st elem in f
	// Part C
	printf("%c\n", g[3]); // 1 --> Pointer to 3rd elem in f
	// Part D
	printf("%c\n", f[13]); // '\0' --> Points to null terminator
	// Part E
	//printf("%c\n", f[14]); // Undefined/Segfault --> Tries indexing past last elem
	// Part F
	printf("%s\n", (char*)(h+1)); // C is cool --> h+1 points to C (cast to char)--prints until reaching null terminator
	return 0;
}



int main()
{
	main2();
}

