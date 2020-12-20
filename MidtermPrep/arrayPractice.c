#include <stdio.h>
#include <stdlib.h>


// Alter the array by adding 1
void addOneArray(int* arr, int length)
{
	for (int i = 0; i < length; i++)
	{
		*(arr + i) += 1;
	}
}

// Allocate space for and return an array that returns palindrome strings
void printStringArray(char** arr, char stringLength, char arrLen)
{
	if (arr == NULL || stringLength == 0 || arrLen == 0)
	{
		printf("Invalid array or dimensions\n");
		//return NULL;
	}
	for (int i = 0; i < arrLen; i++)
	{
		for (int j = 0; j < stringLength; j++)
		{
			printf("%c", arr[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	for (int i = 0; i < arrLen; i++)
	{
		for (int j = 0; j < stringLength; j++)
		{
			printf("%c", *(*(arr + i) + j));
		}
		printf("\n");
	}
	printf("\n");


	// //int numPalindromes = 0;
	// for (char i = 0; i < arrLen; i++) {
	// 	// Check if palindrome
	// 	for (char j = 0; j < stringLength; j++)
	// 	{
	// 		char c = **(arr + i * stringLength + j);
	// 		printf("%c", c);
	// 	}
	// 	printf("\n");
	// }
}

int palindromeStrings(char** stringArr, int arrLen, int strLen)
{
	int numPalindromes = 0;
	for (int i = 0; i < arrLen; i++)
	{
		int pal = 1;
		for (int j = 0; j < strLen; j++)
		{
			char c = *(*(stringArr + i)+j);
			char _c = *(*(stringArr + i)+strLen-j-1);
			if (c != _c)
			{
				pal = 0;
			}
		}
		if (pal)
		{
			numPalindromes += 1;
		}
	}
	printf("Num Palindromes: %i\n", numPalindromes);
	return numPalindromes;
}

int main()
{
	/**
	int arr1[] = {1, 2, 3, 4, 5};
	addOneArray(arr1, 5);
	for (int i = 0; i < 5; i++)
	{
		printf("%i ", arr1[i]);
	}
	printf("\n");
	**/

	/**
	int *arr2;
	arr2 = malloc(5 * sizeof(int));
	arr2[0] = 1; arr2[1] = 2; arr2[2] = 4; arr2[3] = 8; arr2[4] = 16;
	addOneArray(arr2, 5);
	for (int i = 0; i < 5; i++)
	{
		printf("%i ", arr2[i]);
	}

	free(arr2);
	**/
	char** stringArr = malloc(3 * sizeof(char*));
	stringArr[0] = malloc(3 * sizeof(char));
	stringArr[1] = malloc(3 * sizeof(char));
	stringArr[2] = malloc(3 * sizeof(char));
	stringArr[0][0] = 'A';
	*(*(stringArr)+1) = 'B';
	stringArr[0][2] = 'A';
	stringArr[1][0] = 'D';
	stringArr[1][1] = 'E';
	stringArr[1][2] = 'F';
	stringArr[2][0] = 'G';
	stringArr[2][1] = 'H';
	stringArr[2][2] = 'G';
	
	printStringArray(stringArr, 3, 3);
	palindromeStrings(stringArr, 3, 3);


	free(*stringArr);
	free(*(stringArr + 1));
	free(*(stringArr + 2));
	free(stringArr);
	// char** stringArray;
	
	// palindromeStrings(stringArray, 4, 3);


	printf("Done!\n");
	return 0;
}