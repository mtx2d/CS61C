/************************************************************************
**
** NAME:        gameoflife.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Justin Yokota - Starter Code
**				YOUR NAME HERE
**
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"



/* Utility function: power of 2 */
/* From a 24 bit array, return a Color pointer */
Color* colFromBitArray(int bitArray[24])
{
	Color* color = (struct Color*) malloc(sizeof(struct Color));
	uint8_t r = 0, g = 0, b = 0;
	for (int i = 0; i < 8; i++)
	{
		r += bitArray[i] * (1 << i);
		g += bitArray[8+i] * (1 << i);
		b += bitArray[16+i] * (1 << i);
	}
	color->R = r;
	color->G = g;
	color->B = b;
	return color;
}

/* Function to determine whether 1 of the 24 bits should be flipped */
int flipBit(int originalBit, uint32_t rule, int numNeighbors)
{
	if (originalBit == 0)
	{
		return ((1 << numNeighbors) & rule) != 0;
	}
	else
	{
		return ((1 << (numNeighbors + 9)) & rule) != 0;
	}
	
}

/* Given a row, col, width, height, and color array, find the neighbor */
Color* wrapAround(Color** colorArray, int row, int col, int width, int height, int neighbor)
{
	int newRow = row;
	int newCol = col;
	if (neighbor == 0)
	{
		newRow--;
		if (newRow < 0)
		{
			newRow = height - 1;
		}
		newCol--;
		if (newCol < 0)
		{
			newCol = width - 1;
		}
	}
	if (neighbor == 1)
	{
		newRow--;
		if (newRow < 0)
		{
			newRow = height - 1;
		}
	}
	else if (neighbor == 2)
	{
		newRow--;
		if (newRow < 0)
		{
			newRow = height - 1;
		}
		newCol++;
		if (newCol == width)
		{
			newCol = 0;
		}
	}
	else if (neighbor == 3)
	{
		newCol--;
		if (newCol < 0)
		{
			newCol = width - 1;
		}
	}
	else if (neighbor == 4)
	{
		newCol++;
		if (newCol == width)
		{
			newCol = 0;
		}
	}
	else if (neighbor == 5)
	{
		newRow++;
		if (newRow == height)
		{
			newRow = 0;
		}
		newCol--;
		if (newCol < 0)
		{
			newCol = width - 1;
		}
	}
	else if (neighbor == 6)
	{
		newRow++;
		if (newRow == height)
		{
			newRow = 0;
		}
	}
	else if (neighbor == 7)
	{
		newRow++;
		if (newRow == height)
		{
			newRow = 0;
		}
		newCol++;
		if (newCol == width)
		{
			newCol = 0;
		}
	}
	return *(colorArray + newRow * width + newCol);
}
//Determines what color the cell at the given row/col should be. This function allocates space for a new Color.
//Note that you will need to read the eight neighbors of the cell in question. The grid "wraps", so we treat the top row as adjacent to the bottom row
//and the left column as adjacent to the right column.
Color *evaluateOneCell(Image *image, int row, int col, uint32_t rule)
{
	//YOUR CODE HERE

	/* Step 0: Defining the necessary variables */
	int width = image->rows;
	int height = image->cols;
	Color **colorArray = image->image;
	int originalBits[24];
	int newBits[24];
	int numNeighbors[24];

	/* Step 1: Find the 8 color neighbors-Left to right, top to bottom */
	Color* color0 = wrapAround(colorArray, row, col, width, height, 0);
	Color* color1 = wrapAround(colorArray, row, col, width, height, 1);
	Color* color2 = wrapAround(colorArray, row, col, width, height, 2);
	Color* color3 = wrapAround(colorArray, row, col, width, height, 3);
	Color* color4 = wrapAround(colorArray, row, col, width, height, 4);
	Color* color5 = wrapAround(colorArray, row, col, width, height, 5);
	Color* color6 = wrapAround(colorArray, row, col, width, height, 6);
	Color* color7 = wrapAround(colorArray, row, col, width, height, 7);
	Color* currentColor = *(colorArray + row * width + col);

	for (int i = 0; i < 8; i++)
	{
		originalBits[i] = (currentColor->R & (1 << i)) !=0;
		originalBits[i+8] = (currentColor->G & (1 << i)) != 0;
		originalBits[i+16] = (currentColor->B & (1 << i)) != 0;
	}

	for (int i = 0; i < 24; i++)
	{
		numNeighbors[i] = 0;
	}

	/* Step 2: Obtain number of neighbors for each pixel */
	for (int i = 0; i < 8; i++)
	{
		int compare = (1 << i);
		numNeighbors[i] += (color0->R & compare) != 0;
		numNeighbors[i+8] += (color0->G & compare) != 0;
		numNeighbors[i+16] += (color0->B & compare) != 0;

		numNeighbors[i] += (color1->R & compare) != 0;
		numNeighbors[i+8] += (color1->G & compare) != 0;
		numNeighbors[i+16] += (color1->B & compare) != 0;

		numNeighbors[i] += (color2->R & compare) != 0;
		numNeighbors[i+8] += (color2->G & compare) != 0;
		numNeighbors[i+16] += (color2->B & compare) != 0;

		numNeighbors[i] += (color3->R & compare) != 0;
		numNeighbors[i+8] += (color3->G & compare) != 0;
		numNeighbors[i+16] += (color3->B & compare) != 0;

		numNeighbors[i] += (color4->R & compare) != 0;
		numNeighbors[i+8] += (color4->G & compare) != 0;
		numNeighbors[i+16] += (color4->B & compare) != 0;

		numNeighbors[i] += (color5->R & compare) != 0;
		numNeighbors[i+8] += (color5->G & compare) != 0;
		numNeighbors[i+16] += (color5->B & compare) != 0;

		numNeighbors[i] += (color6->R & compare) != 0;
		numNeighbors[i+8] += (color6->G & compare) != 0;
		numNeighbors[i+16] += (color6->B & compare) != 0;

		numNeighbors[i] += (color7->R & compare) != 0;
		numNeighbors[i+8] += (color7->G & compare) != 0;
		numNeighbors[i+16] += (color7->B & compare) != 0;

	}


	/* Step 3: Flip or don't flip the bits depending on the rule implementation */
	for (int i = 0; i < 24; i++)
	{
		newBits[i] = flipBit(originalBits[i], rule, numNeighbors[i]);
	}

	
	/* Step 4: Return a color pointer based on the flipped bits array */
	Color* newColor = (struct Color*) malloc(sizeof(struct Color));
	uint8_t r = 0, g = 0, b = 0;
	for (int i = 0; i < 8; i++)
	{
		r += newBits[i] * (1 << i);
		g += newBits[8+i] * (1 << i);
		b += newBits[16+i] * (1 << i);
	}
	newColor->R = r;
	newColor->G = g;
	newColor->B = b;

	return newColor;
}

//The main body of Life; given an image and a rule, computes one iteration of the Game of Life.
//You should be able to copy most of this from steganography.c
Image *life(Image *image, uint32_t rule)
{
	//YOUR CODE HERE
	int width = image->rows;
	int height = image->cols;

	/* Malloc space for color array */
	Color **colorArray;
	colorArray = (struct Color**) malloc(width * height * sizeof(struct Color *));
	if (colorArray == NULL)
	{
		printf("Out of space for initial color array \n");
	}
	/* Filling in individual color pointers */
	for (int i = 0; i < width * height; i++)
	{
		colorArray[i] = evaluateOneCell(image, i/width, i % width, rule);
		if (colorArray[i] == NULL)
		{
			printf("Out of space in colorArray[i]\n");
		}
	}
	/* Creating the final image */
	Image *imagePtr = (struct Image *) malloc(sizeof(struct Image));
    if (imagePtr == NULL)
    {
        printf("Out of space for image pointer\n");
    }
	imagePtr->image = colorArray;
    imagePtr->rows = width;
    imagePtr->cols = height;
	return imagePtr;
}

/*
Loads a .ppm from a file, computes the next iteration of the game of life, then prints to stdout the new image.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
argv[2] should contain a hexadecimal number (such as 0x1808). Note that this will be a string.
You may find the function strtol useful for this conversion.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!

You may find it useful to copy the code from steganography.c, to start.
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 3)
	{
		printf("usage: ./gameOfLife filename rule\nfilename is an ASCII PPM file (type P3) with maximum value 255.\nrule is a hex number beginning with 0x; Life is 0x1808.\n");
		exit(-1);
	}
	char* fileName = argv[1];
	Image* originalImage = readData(fileName);
	uint32_t rule = strtol(argv[2], NULL, 0);
	Image* lifeImage = life(originalImage, rule);
	writeData(lifeImage);
	freeImage(originalImage);
	freeImage(lifeImage);
	return 0;
}
