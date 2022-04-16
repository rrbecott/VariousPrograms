/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/02/2020                         */
/* PROGRAM ASSIGNMENT #3                        */
/* FILE NAME: thread.cpp                        */
/* PROGRAM PURPOSE:                             */
/*    Implements functions for the thread class */
/*    Adds elements to array B for prefix sum   */
/* -------------------------------------------- */

#include <iostream>
#include <cmath>
#include <string.h>
#include <stdio.h>
#include "thread.h"
using namespace std;

thread::thread(int k, int n)  //thread constructor
		:k(k), n(n)
{

}

void thread::ThreadFunc()
{
	Thread::ThreadFunc();
	char buf[100];
	sprintf(buf, "     Thread %d created\n", n);
        write(1, buf, strlen(buf));
	int i = 0;
	if (n - pow(2, (k-1)) < 0){  //copies element if n < 2^(k-1)
		B[k][n] = B[k-1][n];
		sprintf(buf, "     Thread %d copies x[%d]\n", n, n);
        	write(1, buf, strlen(buf));
	} else {  //adds element n and element n-2^(k-1)
		B[k][n] = B[k-1][n] + B[k-1][(int)(n - pow(2, (k-1)))];
		sprintf(buf, "     Thread %d computes x[%d] + x[%d-2^(%d-1)]\n", n, n, n, k);
        	write(1, buf, strlen(buf));
	}
	sprintf(buf, "     Thread %d exits\n", n);
        write(1, buf, strlen(buf));
	Exit();
}
