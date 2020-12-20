/************************************************************************
**
** NAME:        imageloader.c
**
** DESCRIPTION: CS61C Fall 2020 Project 1
**
** AUTHOR:      Dan Garcia  -  University of California at Berkeley
**              Copyright (C) Dan Garcia, 2020. All rights reserved.
**              Justin Yokota - Starter Code
**				Zachary Zhu
**
**
** DATE:        2020-08-15
**
**************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include "imageloader.h"

//Opens a .ppm P3 image file, and constructs an Image object. 
//You may find the function fscanf useful.
//Make sure that you close the file with fclose before returning.
Image *readData(char *filename) 
{
	FILE *fp;
    char format[3];
    int width, height, maxVal;
    fp = fopen(filename, "r");
    fscanf(fp, "%s %d %d %d", format, &width, &height, &maxVal);
    /*printf("Format: %s\n", format);
    printf("Num Cols: %i\n", width);
    printf("Num Rows: %i\n", height);
    printf("Max Val: %i\n", maxVal);*/
    
    /* Malloc space for color array */
	Color **colorArray;
	colorArray = (struct Color**) malloc(width * height * sizeof(struct Color *));
	if (colorArray == NULL)
	{
		printf("Out of space for initial color array \n");
	}
    
    /* Filling in the color array with color instances */
	for (int i = 0; i < width * height; i++)
	{
        colorArray[i] = (struct Color*) malloc(sizeof(struct Color));
        if (colorArray[i] == NULL)
        {
            printf("Out of space in colorArray[i]\n");
        }
        int r, g, b;
        fscanf(fp, "%d %d %d", &r, &g, &b);
        //printf("RGB: %i, %i, %i\n", r, g, b);
        colorArray[i]->R = r;
        colorArray[i]->G = g;
        colorArray[i]->B = b;
	}
    /* Creating an image pointer */
    Image *imagePtr = (struct Image *) malloc(sizeof(struct Image));
    if (imagePtr == NULL)
    {
        printf("Out of space for image pointer\n");
    }
	imagePtr->image = colorArray;
    imagePtr->rows = width;
    imagePtr->cols = height;
    fclose(fp);
    return imagePtr;
}

//Given an image, prints to stdout (e.g. with printf) a .ppm P3 file with the image's data.
void writeData(Image *image)
{
	//YOUR CODE HERE
	int width = image->rows;
	int height = image->cols;
	printf("P3\n");
	printf("%d %d\n", width, height);
	printf("255\n");
	Color** colorPtr = image->image;
    Color* tempColor;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width - 1; j++)
        {
            tempColor = *(colorPtr + (i * width + j));
            printf("%3u %3u %3u   ", tempColor->R, tempColor->G, tempColor->B);
        }
        tempColor = *(colorPtr + (i+1) * width - 1);
        printf("%3u %3u %3u\n", tempColor->R, tempColor->G, tempColor->B);
    }

}

//Frees an image
void freeImage(Image *image)
{
	//YOUR CODE HERE
	if (image == NULL)
	{
		printf("Error: null image pointer\n");
		return;
	}
	Color **colorPtr = image->image;
	if (colorPtr == NULL)
	{
		printf("Error: Null Color Pointer\n");
		return;
	}
    int width = image->rows;
    int height = image->cols;
    Color* tempColor;
    for (int i = 0; i < width * height; i++)
    {
        tempColor = *(colorPtr + i);
		if (tempColor == NULL)
		{
			printf("NULL Color error\n");
		}
		else
		{
			free(tempColor);
		}
    }
    free(colorPtr);
    free(image);
}