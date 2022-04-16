/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/02/2020                         */
/* PROGRAM ASSIGNMENT #3                        */
/* FILE NAME: thread-main.cpp                   */
/* PROGRAM PURPOSE:                             */
/*    Computes the prefix sum of an array       */
/*    input and outputs the result              */
/* -------------------------------------------- */

#include "thread.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

int** B;  //global 2D array
int main(int argc, char *argv[])
{
	char buf[100];
	char buf2[100];

	sprintf(buf, "\nConcurrent Prefix Sum Computation\n\n");
        write(1, buf, strlen(buf));

	int n;  //size of input array
	fscanf(stdin, "%d", &n);
	
	sprintf(buf, "Number of input data = %d\n", n);
        write(1, buf, strlen(buf));	
	sprintf(buf, "Input array:\n");
        write(1, buf, strlen(buf));

	thread *newthread[n];  //array of threads
	int x[n];  //input array
	int temporary = 0;
        int i = 0;
        while (fscanf(stdin, "%d", &temporary) == 1){  //scans in input array to array x
                x[i] = temporary;
		sprintf(buf, "   %d", x[i]);
        	write(1, buf, strlen(buf));
                i++;
        }
	sprintf(buf, "\n\n");
        write(1, buf, strlen(buf));
	
	i = 1;
        int k = 0;
        while (i != n){
                i = i * 2;
                k++;
        }
	B = new int*[k+1];  //initializes global array B
	for (i = 0; i < k+1; i++){  
		B[i] = new int[n];
	}
	for (i = 0; i < n; i++){  //first row of B is input array x
                B[0][i] = x[i];
        }

	int j;
	int m;
	for (i = 1; i < k+1; i++){  //runs k times where n = 2^k
		sprintf(buf, "\nRun %d:\n", i);
                write(1, buf, strlen(buf));
		for (j = 0; j < n; j++){  //creates n threads to execute the threadfunc
			newthread[j] = new thread(i, j);
			newthread[j]->Begin();
		}
		for (m = 0; m < n; m++){  //joins the n threads created
			newthread[m]->Join();
		}
		sprintf(buf, "Result after run %d:\n", i);
		for (m = 0; m < n; m++){
			sprintf(buf2, "  %d", B[i][m]);
			strcat(buf, buf2);
		}
		write(1, buf, strlen(buf));
	}
	sprintf(buf, "\n\n\nFinal result after run %d:\n", k);
	for (i = 0; i < n; i++){
		sprintf(buf2, "  %d", B[k][i]);
		strcat(buf, buf2);
	}
	write(1, buf, strlen(buf));
	sprintf(buf, "\n\n");
        write(1, buf, strlen(buf));
	Exit();
}
