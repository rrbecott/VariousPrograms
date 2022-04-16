/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 09/28/2021                         */
/* PROGRAM ASSIGNMENT #3                        */
/* FILE NAME: ray3.cpp                          */
/* PROGRAM PURPOSE:                             */
/*    Uses raytracing to display a scene and    */
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

struct Material
{
        float color[3];
        int reflective;
};

struct Sphere
{
	float coords[3];
	float radius;
	Material material;
};

struct Triangle
{
	float v[3][3];
	Material material;
};

struct RayHit
{
	float t;
	Material material;
	float intersect[3];
	float surfNorm[3];
};

float dot_prod(float vec1[3], float vec2[3]){
	return (vec1[0] * vec2[0]) + (vec1[1] * vec2[1]) + (vec1[2] * vec2[2]);
}

float diff_shade(float vecNorm[3], float lightVec[3]){
	float perc = dot_prod(vecNorm, lightVec);
	if (perc < .2){   //clamp
		perc = .2;
	}
	if (perc > 1){
		perc = 1;
	}
	return perc;
}

RayHit triangle_intersect(Triangle triangle, Ray ray){
	RayHit rayhit;
	rayhit.material = triangle.material;
	float A, B, C, D, E, F, G, H, I, J, K, L, M, beta, gamma, temp[3], temp2[3];
	A = triangle.v[0][0] - triangle.v[1][0];   //ray triangle intersection formula
	B = triangle.v[0][1] - triangle.v[1][1];
	C = triangle.v[0][2] - triangle.v[1][2];
	D = triangle.v[0][0] - triangle.v[2][0];
	E = triangle.v[0][1] - triangle.v[2][1];
	F = triangle.v[0][2] - triangle.v[2][2];
	G = ray.dir[0];
	H = ray.dir[1];
	I = ray.dir[2];
	J = triangle.v[0][0] - ray.pos[0];
	K = triangle.v[0][1] - ray.pos[1];
	L = triangle.v[0][2] - ray.pos[2];
	M = A*(E*I - H*F) + B*(G*F - D*I) + C*(D*H - E*G);
	rayhit.t = (-(F*(A*K - J*B) + E*(J*C - A*L) + D*(B*L - K*C))) / M;

	temp[0] = triangle.v[2][0] - triangle.v[0][0];   //(b-a) X (c-a)
	temp[1] = triangle.v[2][1] - triangle.v[0][1];
	temp[2] = triangle.v[2][2] - triangle.v[0][2];
	temp2[0] = triangle.v[1][0] - triangle.v[0][0];
        temp2[1] = triangle.v[1][1] - triangle.v[0][1];
        temp2[2] = triangle.v[1][2] - triangle.v[0][2];
	rayhit.surfNorm[0] = temp2[1]*temp[2] - temp2[2]*temp[1];   //calculate surface norm
	rayhit.surfNorm[1] = temp2[2]*temp[0] - temp2[0]*temp[2];
	rayhit.surfNorm[2] = temp2[0]*temp[1] - temp2[1]*temp[0];
	float length = sqrt(pow(rayhit.surfNorm[0], 2) + pow(rayhit.surfNorm[1], 2) + pow(rayhit.surfNorm[2], 2));
        rayhit.surfNorm[0] = rayhit.surfNorm[0] / length;   //normalize surface norm
        rayhit.surfNorm[1] = rayhit.surfNorm[1] / length;
        rayhit.surfNorm[2] = rayhit.surfNorm[2] / length;

	if (rayhit.t < 1){   //no hit
		return rayhit;
	}
	gamma = (I*(A*K - J*B) + H*(J*C - A*L) + G*(B*L - K*C)) / M;
	if (gamma < 0 || gamma > 1){   //no hit
		rayhit.t = -1;
		return rayhit;
	}
	beta = (J*(E*I - H*F) + K*(G*F - D*I) + L*(D*H - E*G)) / M;
	if (beta < 0 || beta > (1 - gamma)){   //no hit
		rayhit.t = -1;
		return rayhit;
	}
	rayhit.intersect[0] = ray.pos[0] + rayhit.t * ray.dir[0];
        rayhit.intersect[1] = ray.pos[1] + rayhit.t * ray.dir[1];
        rayhit.intersect[2] = ray.pos[2] + rayhit.t * ray.dir[2];

	return rayhit;
}

RayHit sphere_intersect(Sphere sphere, Ray ray){
	RayHit rayhit;
	rayhit.material = sphere.material;
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
		rayhit.t = -1;
		return rayhit;
	}
	temp6 = sqrt(temp6);
	t1 = (temp2 + temp6) / temp4;
	t2 = (temp2 - temp6) / temp4;
	if (t1 < t2){   //return smallest t value
		rayhit.t = t1;
	} else {
		rayhit.t = t2;
	}
	rayhit.intersect[0] = ray.pos[0] + rayhit.t * ray.dir[0];   //calculate intersect point
	rayhit.intersect[1] = ray.pos[1] + rayhit.t * ray.dir[1];
	rayhit.intersect[2] = ray.pos[2] + rayhit.t * ray.dir[2];
	rayhit.surfNorm[0] = rayhit.intersect[0] - sphere.coords[0];   //calculate surface norm
        rayhit.surfNorm[1] = rayhit.intersect[1] - sphere.coords[1];
        rayhit.surfNorm[2] = rayhit.intersect[2] - sphere.coords[2];
        float length = sqrt(pow(rayhit.surfNorm[0], 2) + pow(rayhit.surfNorm[1], 2) + pow(rayhit.surfNorm[2], 2));
        rayhit.surfNorm[0] = rayhit.surfNorm[0] / length;   //normalize surface norm
	rayhit.surfNorm[1] = rayhit.surfNorm[1] / length;
        rayhit.surfNorm[2] = rayhit.surfNorm[2] / length;
	return rayhit;
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
	RayHit calcRH;   //final rayhit
	RayHit tempRH;   //temp rayhit
	float lightRay[3];   //ray towards light
	float length;   //norm length
	float percDiff;   //percent diffusion
	Ray calcLight;   //light ray
        unsigned char *image = (unsigned char *) malloc(width*height*3);   //array for rgb values

	Material refl = { .color = {0,0,0}, .reflective = 1 };
	Material blue = { .color = {0,0,255}, .reflective = 0 };
	Material red =  { .color = {255,0,0}, .reflective = 0 };
	Material white = { .color = {255,255,255}, .reflective = 0 };
	Material black = { .color = {0,0,0}, .reflective = 0 };

	float light[3] = {3,5,-15};

	Sphere spheres[100];
	spheres[0] = (Sphere) { .coords = { 0,0,-16 },   .radius = 2, .material = refl };
	spheres[1] = (Sphere) { .coords = { 3,-1,-14 },  .radius = 1, .material = refl };
	spheres[2] = (Sphere) { .coords = { -3,-1,-14 }, .radius = 1, .material = red };
	int numSpheres = 3;

	Triangle triangles[100];
	// back wall
	triangles[0] = (Triangle) { .v = { { -8,-2,-20 }, {8,-2,-20}, {8,10,-20} }, .material = blue };
	triangles[1] = (Triangle) { .v = { { -8,-2,-20 }, {8,10,-20}, {-8,10,-20} }, .material = blue };
	// floor
	triangles[2] = (Triangle) { .v = { { -8,-2,-20 }, {8,-2,-10}, {8,-2,-20}}, .material = white };
	triangles[3] = (Triangle) { .v = { { -8,-2,-20 }, {-8,-2,-10}, {8,-2,-10}}, .material = white };
	// right red triangle
	triangles[4] = (Triangle) { .v = { { 8,-2,-20 }, {8,-2,-10}, {8,10,-20}}, .material = red };
	int numTriangles=5;

	for (int y = height - 1; y >= 0; y--){
		for (int x = 0; x < width; x++){
			index[0] = (x * (planeWidth / width) + (planeWidth / width) / 2) - (planeWidth / 2);   //index for each pixel
			index[1] = (y * (planeHeight / height) + (planeHeight / height) / 2) - (planeHeight / 2);
			index[2] = -2;
			calcRay = getRay(index);
			calcRH.t = -1;
			calcRH.material = black;
			for (int i = 0; i < numSpheres; i++){   //iterate through spheres
				tempRH = sphere_intersect(spheres[i], calcRay);
				if (tempRH.t > 0){
					if (calcRH.t < 0 || tempRH.t < calcRH.t){   //find closest hit
						calcRH = tempRH;
					}
				}
			}
			for (int i = 0; i < numTriangles; i++){   //iterate through triangles
                                tempRH = triangle_intersect(triangles[i], calcRay);
                                if (tempRH.t > 0){
                                        if (calcRH.t < 0 || tempRH.t < calcRH.t){   //find closest hit
                                                calcRH = tempRH;
                                        }
                                }
                        }
			int j = 0;
			while (calcRH.material.reflective == 1){   //if the material is reflective
				calcRH.t = -1;
				float ddotn = dot_prod(calcRay.dir, calcRH.surfNorm);
				calcRay.dir[0] = calcRay.dir[0] + -2*ddotn*calcRH.surfNorm[0];
				calcRay.dir[1] = calcRay.dir[1] + -2*ddotn*calcRH.surfNorm[1];
				calcRay.dir[2] = calcRay.dir[2] + -2*ddotn*calcRH.surfNorm[2];
				calcRay.pos[0] = calcRH.intersect[0] + .0001 * calcRay.dir[0];
				calcRay.pos[1] = calcRH.intersect[1] + .0001 * calcRay.dir[1];
				calcRay.pos[2] = calcRH.intersect[2] + .0001 * calcRay.dir[2];
				for (int i = 0; i < numSpheres; i++){   //iterate through spheres
                                	tempRH = sphere_intersect(spheres[i], calcRay);
                                	if (tempRH.t > 0){
                                        	if (calcRH.t < 0 || tempRH.t < calcRH.t){   //find closest hit
                                                	calcRH = tempRH;
                                        	}
                                	}
                        	}
                        	for (int i = 0; i < numTriangles; i++){   //iterate through triangles
                                	tempRH = triangle_intersect(triangles[i], calcRay);
                                	if (tempRH.t > 0){
                                        	if (calcRH.t < 0 || tempRH.t < calcRH.t){   //find closest hit
                                                	calcRH = tempRH;
                                        	}
                                	}
                        	}
				j++;
				if (j == 10 || calcRH.t < 0){
					calcRH.material = black;
					break;
				}
			}

			if (calcRH.material.reflective == 0){   //if the material is diffuse
                                lightRay[0] = light[0] - calcRH.intersect[0];   //calculate and normalize light ray
                                lightRay[1] = light[1] - calcRH.intersect[1];
                                lightRay[2] = light[2] - calcRH.intersect[2];
                                length = sqrt(pow(lightRay[0], 2) + pow(lightRay[1], 2) + pow(lightRay[2], 2));
                                lightRay[0] = lightRay[0] / length;   //normalize light ray
                                lightRay[1] = lightRay[1] / length;
                                lightRay[2] = lightRay[2] / length;
                                calcLight.dir[0] = lightRay[0];   //put light ray into ray struct
                                calcLight.dir[1] = lightRay[1];
                                calcLight.dir[2] = lightRay[2];
                                calcLight.pos[0] = calcRH.intersect[0] + .0001 * calcLight.dir[0];
                                calcLight.pos[1] = calcRH.intersect[1] + .0001 * calcLight.dir[1];
                                calcLight.pos[2] = calcRH.intersect[2] + .0001 * calcLight.dir[2];
                                for (int i = 0; i < numSpheres; i++){   //iterate through spheres
                                        tempRH = sphere_intersect(spheres[i], calcLight);   //calculate shadow with light ray
                                        if (tempRH.t > 0){   //is in shadow
                                                percDiff = .2;
                                        }
                                }
                                for (int i = 0; i < numTriangles; i++){   //iterate through spheres
                                        tempRH = triangle_intersect(triangles[i], calcLight);   //calculate shadow with light ray
                                        if (tempRH.t > 0){   //is in shadow
                                                percDiff = .2;
                                        }
                                }
				if (percDiff == -1){   //was not in shadow
                        		percDiff = diff_shade(calcRH.surfNorm, lightRay);
                        	}
			}
			image[imgInd] = calcRH.material.color[0] * percDiff;
			imgInd++;
			image[imgInd] = calcRH.material.color[1] * percDiff;
                        imgInd++;
			image[imgInd] = calcRH.material.color[2] * percDiff;
			imgInd++;
			percDiff = -1;
		}
	}

	stbi_write_png("reference.png", width, height, 3, image,width*3);
	free(image);
}
