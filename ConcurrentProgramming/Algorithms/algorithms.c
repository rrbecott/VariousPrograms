/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 09/25/2020                         */
/* PROGRAM ASSIGNMENT #1                        */
/* FILE NAME: prog1.c                           */
/* PROGRAM PURPOSE:                             */
/*      Concurrently runs five processes to     */
/*      complete four different math problems,  */
/*	fibonacci sequence, Buffon's Needle,    */
/*	integration, and Bernoulli's            */
/*	approximation of e.                     */
/* -------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <math.h>

long fibonacci(long n);
double buffon(long r);
float integration(long s);
void bernoulli(unsigned long m);

/* ----------------------------------------------------------- */
/* FUNCTION main                                               */
/*     Concurrently runs multiple processes to run different   */
/*     methods and output status updates of each process       */
/* PARAMETER USAGE :                                           */
/*     int argc = number of inputs on command line             */
/*     char** argv = array of inputs on command line           */
/* FUNCTION CALLED :                                           */
/*     fibonacci(), buffon(), integration(), bernoulli(),      */
/*     strlen(), atol(), fork(), wait(), exit(), sprintf(),    */
/*     write()                                                 */
/* ----------------------------------------------------------- */
int main(int argc, char** argv){

	int status;
	char buf[100];

	sprintf(buf, "Main Process Started\n");
	write(1, buf, strlen(buf));

	unsigned long m = atol(argv[1]);    /*reads command line input*/
	long n = atol(argv[2]);
	long r = atol(argv[3]);
	long s = atol(argv[4]);

	sprintf(buf, "Fibonacci Number %ld\n", n);
	write(1, buf, strlen(buf));
	sprintf(buf, "Buffon's Needle Iterations = %ld\n", r);
	write(1, buf, strlen(buf));
	sprintf(buf, "Integration Iterations = %ld\n", s);
	write(1, buf, strlen(buf));
	sprintf(buf, "Approx. e Iterations = %lu\n", m);
	write(1, buf, strlen(buf));

	pid_t pid;
	pid = fork();     /*forks for the fibonacci process*/
	if (pid == 0){
		sprintf(buf, "      Fibonacci Process Started\n");
		write(1, buf, strlen(buf));
		long fibOut = fibonacci(n);  
		sprintf(buf, "      Input Number %ld\n", n);
		write(1, buf, strlen(buf));
		sprintf(buf, "      Fibonacci Number f(%ld) is %ld\n", n, fibOut);
		write(1, buf, strlen(buf));
		sprintf(buf, "      Fibonacci Process Exits\n");
		write(1, buf, strlen(buf));
		exit(1);
	} else {
		sprintf(buf, "Fibonacci Process Created\n");
		write(1, buf, strlen(buf));
	}

	pid = fork();     /*forks for buffon's needle process*/
	if (pid == 0){
		sprintf(buf, "          Buffon's Needle Process Started\n");
		write(1, buf, strlen(buf));
		double buffonOut = buffon(r);
		sprintf(buf, "          Input Number %ld\n", r);
		write(1, buf, strlen(buf));
		sprintf(buf, "          Estimated Probability is %.5f\n", buffonOut);
		write(1, buf, strlen(buf));
		sprintf(buf, "          Buffon's Needle Process Exits\n");
		write(1, buf, strlen(buf));
		exit(1);
	} else {
		sprintf(buf, "Buffon's Needle Process Created\n");
		write(1, buf, strlen(buf));
	}

	pid = fork();      /*forks for the integration process*/
	if (pid == 0){
		sprintf(buf, "            Integration Process Started\n");
		write(1, buf, strlen(buf));
		sprintf(buf, "            Input Number %ld\n", s);
		write(1, buf, strlen(buf));
		float integrationOut = integration(s);
		sprintf(buf, "            Estimated Area is %.7f\n", integrationOut);
		write(1, buf, strlen(buf));
		sprintf(buf, "            Integration Process Exits\n");
		write(1, buf, strlen(buf));
		exit(1);
	} else {
		sprintf(buf, "Integration Process Created\n");
		write(1, buf, strlen(buf));
	}

	pid = fork();     /*forks for the approximation of e process*/
	if (pid == 0){
		sprintf(buf, "    Approximation of e Process Started\n");
		write(1, buf, strlen(buf));
		sprintf(buf, "    Maximum of the Exponent %ld\n", m);
		write(1, buf, strlen(buf));
		bernoulli(m);
		exit(1);
	} else {
		sprintf(buf, "Approximation of e Process Created\n");
		write(1, buf, strlen(buf));
	}

	sprintf(buf, "Main Process Waits\n");
	write(1, buf, strlen(buf));

	int i;
	for (i = 0; i < 4; i++){    /*loops 4 times to wait for child processes*/
		wait(&status);
	}
	
	sprintf(buf, "Main Process Exits\n");
	write(1, buf, strlen(buf));
	return 1;
}

/* ----------------------------------------------------------- */
/* FUNCTION fibonacci:                                         */
/*     Returns fibonacci sum for the number input              */
/* PARAMETER USAGE:                                            */
/*     n = how many numbers of the fibonacci sequence          */
/*	   the function will add up                            */
/* FUNCTION CALLED:                                            */
/*     fibonacci()                                             */
/* ----------------------------------------------------------- */
long fibonacci(long n){
	if (n == 0)
		return 0;
	else if (n == 1)
		return 1;
	else
		return (fibonacci(n-1) + fibonacci(n-2));  /*recursively calls fibonacci*/
}

/* ----------------------------------------------------------- */
/* FUNCTION buffon:                                            */
/*     Calculates the probability that a needle being          */
/*     dropped will intersect lines                            */
/* PARAMETER USAGE:                                            */
/*     r = number of iterations of dropping a needle           */
/* FUNCTION CALLED:                                            */
/*     rand(), sin(), acos()                                   */
/* ----------------------------------------------------------- */
double buffon(long r){
	int i;
	long t = 0;
	for (i = 0; i < r; i++){  /*loops r times to check if a line was crossed*/
		float d = (float) rand()/RAND_MAX;
		float temp = (float) rand()/RAND_MAX;
		float a = 2.0 * acos(-1.0) * temp;
		if ((d + sin(a)) > 1.0 || (d + sin(a)) < 0.0){
			t++;
		}
	}
	return (double) t/r;
}

/* ----------------------------------------------------------- */
/* FUNCTION integration:                                       */
/*     Estimates the area below the sine curve                 */
/* PARAMETER USAGE :                                           */
/*     long s = number of points put on the graph              */
/* FUNCTION CALLED :                                           */
/*    rand(), sin(), acos()                                    */
/* ----------------------------------------------------------- */
float integration(long s){
	char buf[100];
	int i;
	long t = 0;
	for (i = 0; i < s; i++){  /*loops s times to check if b is under the sine curve*/
		float a = ((float) rand()/RAND_MAX) * acos(-1.0);
		float b = (float) rand()/RAND_MAX;
		if (b <= sin(a)){
			t++;
		}
	}
	sprintf(buf, "            Total Hit %ld\n", t);
	write(1, buf, strlen(buf));
	return ((float) t/s) * acos(-1.0);
}

/* ----------------------------------------------------------- */
/* FUNCTION bernoulli:                                         */
/*    Approximates the value of e                              */
/* PARAMETER USAGE :                                           */
/*    unsigned long m = maximum exponent of bernoulli's        */
/*                      formula to check                       */
/* FUNCTION CALLED :                                           */
/*    exp(), pow()                                             */
/* ----------------------------------------------------------- */
void bernoulli(unsigned long m){
	char buf[100];
	double e;
	double error = exp(1.0);
	unsigned long i;
	for (i = 1; i < 11; i++){  /*loops 10 times and approximates e*/
		e = pow((1+(1/(double) i)), (double) i);
		sprintf(buf, "    %18lu    %.15f    %.15f\n", i, e, (error-e));
		write(1, buf, strlen(buf));
	}
	for (i = 16; i < m; i = (i*2)){  /*loops up to m and approximates e*/
		e = pow((1+(1/(double) i)), (double) i);
		sprintf(buf, "    %18lu    %.15f    %.15f\n", i, e, (error-e));
		write(1, buf, strlen(buf));
	}
}
