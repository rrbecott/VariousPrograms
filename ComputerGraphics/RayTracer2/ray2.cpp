/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 09/20/2021                         */
/* PROGRAM ASSIGNMENT #2                        */
/* FILE NAME: ray2.cpp                          */
/* PROGRAM PURPOSE:                             */
/*    Uses raytracing to display a sphere and   */
/*    saves it to disk                          */
/* -------------------------------------------- */

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

struct Ray
{
	float pos[3];
	float dir[3];
};

struct Sphere
{
	float coords[3];
	float radius;
};

float dot_prod(float vec1[3], float vec2[3]){
	float sum = 0;
	sum = sum + (vec1[0] * vec2[0]);
	sum = sum + (vec1[1] * vec2[1]);
	sum = sum + (vec1[2] * vec2[2]);
	return sum;
}

float sphere_intersect(Sphere sphere, Ray ray){
	float t1, t2, rsq, temp1[3], temp2, temp3, temp4, temp5, temp6, nArr[3];
	nArr[0] = ray.dir[0] * -1.;   //negates array
	nArr[1] = ray.dir[1] * -1.;
	nArr[2] = ray.dir[2] * -1.;
	rsq = pow(sphere.radius, 2);   //R^2
	temp1[0] = ray.pos[0] - sphere.coords[0];   //(e-c)
	temp1[1] = ray.pos[1] - sphere.coords[1];
	temp1[2] = ray.pos[2] - sphere.coords[2];
	temp2 = dot_prod(nArr, temp1);  //-d o (e-c)
	temp3 = pow(dot_prod(ray.dir, temp1),2);   //(d o (e-c))^2
	temp4 = dot_prod(ray.dir, ray.dir);   //d o d
	temp5 = dot_prod(temp1, temp1) - rsq;   // ((e-c) o (e-c) - R^2)
	temp6 = temp3 - (temp4 * temp5);
	if (temp6 < 0){   //does not hit
		t1 = -1;
		return t1;
	}
	temp6 = sqrt(temp6);
	t1 = (temp2 + temp6) / temp4;
	t2 = (temp2 - temp6) / temp4;
	if (t1 < t2){   //return smallest
		return t1;
	} else {
		return t2;
	}
}

Ray getRay(float index[3]){
	Ray newRay;
	newRay.pos[0] = 0;
	newRay.pos[1] = 0;
	newRay.pos[2] = 0;
	float length = sqrt(pow(index[0], 2) + pow(index[1], 2) + pow(index[2], 2));   //length of vector
	newRay.dir[0] = index[0] / length;   //normalize vector
	newRay.dir[1] = index[1] / length;
	newRay.dir[2] = index[2] / length;

	return newRay;
}

int main()
{
	float width = 512;   //width of entire image
        float height = 512;   //height of entire image
	float planeWidth = 2;   //width of image plane
	float planeHeight = 2;   //height of image plane
	float index[3];   //index of ray to shoot through pixel
	float calcT;   //calculated t value
	int imgInd = 0;   //index for image array
	Ray calcRay;   //normalized ray

	Sphere sph;   //sphere in image
	sph.coords[0] = 2;
	sph.coords[1] = 2;
	sph.coords[2] = -16;
	sph.radius = 5.3547;

        unsigned char *image = (unsigned char *) malloc(width*height*3);   //array for rgb values

	for (int y = height - 1; y >= 0; y--){
		for (int x = 0; x < width; x++){
			index[0] = (x * (planeWidth / width) + (planeWidth / width) / 2) - (planeWidth / 2);   //index for each pixel
			index[1] = (y * (planeHeight / height) + (planeHeight / height) / 2) - (planeHeight / 2);
			index[2] = -2;
			calcRay = getRay(index);
			if (x == 0 && y == (height - 1)){
				printf("Bottom left pixel\nRayPosition %f %f %f\nRayDirection %f %f %f\n\n", calcRay.pos[0], calcRay.pos[1], calcRay.pos[2], calcRay.dir[0], calcRay.dir[1], calcRay.dir[2]);
			}
			if (x == (width - 1) && y == 0){
				printf("Top right pixel\nRayPosition %f %f %f\nRayDirection %f %f %f\n\n", calcRay.pos[0], calcRay.pos[1], calcRay.pos[2], calcRay.dir[0], calcRay.dir[1], calcRay.dir[2]);
			}
			if (x == (width / 2) && y == ((height / 2) + 1)){
				printf("Middle pixel\nRayPosition %f %f %f\nRayDirection %f %f %f\n\n", calcRay.pos[0], calcRay.pos[1], calcRay.pos[2], calcRay.dir[0], calcRay.dir[1], calcRay.dir[2]);
			}
			calcT = sphere_intersect(sph, calcRay);
			if (calcT < 0){   //color red
				image[imgInd] = 128;
				imgInd++;
				image[imgInd] = 0;
                                imgInd++;
				image[imgInd] = 0;
                                imgInd++;
			} else {   //color white
				image[imgInd] = 255;
                                imgInd++;
                                image[imgInd] = 255;
                                imgInd++;
                                image[imgInd] = 255;
                                imgInd++;
			}
		}
	}

	stbi_write_png("sphere.png", width, height, 3, image,width*3);
	free(image);
}
