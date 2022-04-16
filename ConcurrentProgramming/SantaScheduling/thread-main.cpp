/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/20/2020                         */
/* PROGRAM ASSIGNMENT #4                        */
/* FILE NAME: thread-main.cpp                   */
/* PROGRAM PURPOSE:                             */
/*    Creates and runs santa, elf, and reindeer */
/*    threads                                   */
/* -------------------------------------------- */

#include "thread.h"
#include "thread-support.h"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

int e;   //number of elves input
int r;   //number of reindeer input
int t;   //number of trips input

int main(int argc, char *argv[])
{
	char buf[100];
	e = atoi(argv[1]);
	if (e <= 0){   //default for e
		e = 7;
	}
	r = atoi(argv[2]);
	if (r <= 0){   //default for r
		r = 9;
	}
	t = atoi(argv[3]);
	if (t <= 0){   //default for t
		t = 5;
	}
	thread *elves[e];   //array of elf threads
	thread *reindeer[r];   //array of reindeer threads
	thread *santa;   //santa thread
	int i = 0;

	santa = new thread(0, 0);
	santa->Begin();
	sprintf(buf, "Santa thread starts\n");
        write(1, buf, strlen(buf));

	for (i = 1; i <= r; i++){
		reindeer[i] = new thread(i, 1);
		reindeer[i]->Begin();
		sprintf(buf, "    Reindeer %d starts\n", i);
        	write(1, buf, strlen(buf));
	}
	for (i = 1; i <= e; i++){
		elves[i] = new thread(i, 2);
		elves[i]->Begin();
		sprintf(buf, "         Elf %d starts\n", i);
        	write(1, buf, strlen(buf));
	}
	santa->Join();   //joins main thread on santa thread
	Exit();
	return 0;
}
