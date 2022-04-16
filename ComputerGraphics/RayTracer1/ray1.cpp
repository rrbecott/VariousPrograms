/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 09/11/2021                         */
/* PROGRAM ASSIGNMENT #1                        */
/* FILE NAME: ray1.cpp                          */
/* PROGRAM PURPOSE:                             */
/*    Writes a checkerboard image and gradient  */
/*    image to the disk                         */
/* -------------------------------------------- */

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdlib.h>

int main()
{
	int width = 512;   //width of entire image
	int height = 512;   //height of entire image
	unsigned char *image = (unsigned char *) malloc(width*height*3);   //array for rgb values

	//Checkerboard Pattern

	int tileRows = 8;   //number of squares in each checkerboard row
	int tileDim = 64;   //dimensions of checkerboard tiles
	int index = 0;   //index for image array

	for (int k = 0; k < tileRows/2; k++){   //repeat for every 2 rows
		for (int i = 0; i < tileRows/2; i++){   //repeat for every 2 squares, start with red
			for (int m = 0; m < tileDim; m++){   //repeat for the height of each square
				for (int j = 0; j < tileDim; j++){   //colors one row of one red square
					image[index] = 255;   //set red
					index++;
					image[index] = 0;
                        		index++;
					image[index] = 0;
                        		index++;
				}
				for (int j = 0; j < tileDim; j++){   //colors one row of one blue square
                        		image[index] = 0;   //set blue
                        		index++;
                        		image[index] = 0;
                        		index++;
                        		image[index] = 255;
                        		index++;
                		}
			}
		}
		for (int i = 0; i < tileRows/2; i++){   //repeat for every 2 squares, start with blue
                        for (int m = 0; m < tileDim; m++){   //repeat for the height of each square
				for (int j = 0; j < tileDim; j++){   //colors one row of one blue square
                                	image[index] = 0;   //set blue
                                	index++;
                                	image[index] = 0;
                                	index++;
                                	image[index] = 255;
                                	index++;
                        	}
                        	for (int j = 0; j < tileDim; j++){   //colors one row of one red square
                                	image[index] = 255;   //set red
                                	index++;
                                	image[index] = 0;
                                	index++;
                                	image[index] = 0;
                                	index++;
                        	}
			}
                }
	}

	stbi_write_png("checkerboard.png", width, height, 3, image,width*3);

	int red = 0;
	int green = 0;
	int blue = 128;
	index = 0;

	for (int i = 0; i < height; i++){   //repeat for the length of the height
		for (int j = 0; j < width; j++){   //repeat for the length of the width
			image[index] = red;
			index++;
			image[index] = green;
			index++;
			image[index] = blue;
			index++;
			if (j % 2 == 1){   //increment red every 2 pixels
				red++;
			}
		}
		if (i % 2 == 1){   //increment green every 2 pixels
			green++;
		}
	}

	stbi_write_png("gradient.png", width, height, 3, image,width*3);
	free(image);
}
