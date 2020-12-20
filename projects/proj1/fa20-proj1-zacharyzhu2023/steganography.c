/************************************************************************
**
** NAME:        steganography.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**				Justin Yokota - Starter Code
**				YOUR NAME HERE
**
** DATE:        2020-08-23
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "imageloader.h"

//Determines what color the cell at the given row/col should be. This should not affect Image, and should allocate space for a new Color.
Color *evaluateOnePixel(Image *image, int row, int col)
{
	//YOUR CODE HERE
	int width = image->rows;
	int height = image->cols;
	Color** colorPtr = image->image;
	Color* newColor = (struct Color*) malloc(sizeof(struct Color));
	Color* oldColor = *(colorPtr + (width * row + col));
	if (oldColor->B % 2 == 0)
	{
		newColor->R = 0;
		newColor->G = 0;
		newColor->B = 0;
	}
	else
	{
		newColor->R = 255;
		newColor->G = 255;
		newColor->B = 255;
	}
	return newColor;
}

//Given an image, creates a new image extracting the LSB of the B channel.
Image *steganography(Image *image)
{
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
		colorArray[i] = evaluateOnePixel(image, i/width, i % width);
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
Loads a .ppm from a file, and prints to stdout (e.g. with printf) a new image, 
where each pixel is black if the LSB of the B channel is 0, 
and white if the LSB of the B channel is 1.

argc stores the number of arguments.
argv stores a list of arguments. Here is the expected input:
argv[0] will store the name of the program (this happens automatically).
argv[1] should contain a filename, containing a .ppm.
If the input is not correct, a malloc fails, or any other error occurs, you should exit with code -1.
Otherwise, you should return from main with code 0.
Make sure to free all memory before returning!
*/
int main(int argc, char **argv)
{
	//YOUR CODE HERE
	if (argc != 2)
	{
		printf("Error: Wrong number of arguments\n");
		return -1;
	}
	else if (argv[0] == NULL)
	{
		printf("Error: Invalid program name\n");
		return -1;
	}
	else if (argv[1] == NULL)
	{
		printf("Error: Invalid file name\n");
		return -1;
	}
	char* fileName = argv[1];
	Image* originalImage = readData(fileName);
	Image* steganographyImage = steganography(originalImage);
	writeData(steganographyImage);
	freeImage(originalImage);
	freeImage(steganographyImage);
	return 0;
}
