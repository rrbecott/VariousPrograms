// -----------------------------------------------------------
// NAME : Ryan Becotte                       User ID: rrbecott
// DUE DATE : 12/04/2020
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
//    Creates cannibal, missionary, and boat threads and 
//    starts them
// -----------------------------------------------------------

#include <ThreadClass.h>
#include "thread.h"
#include "boat-monitor.h"
#include <stdio.h>
#include <string.h>
using namespace std;

int b;
int x;

int main(int argc, char *argv[])
{
	int c = atoi(argv[1]);   //number of cannibals
	int m = atoi(argv[2]);   //number of missionaries
	b = atoi(argv[3]);   //number of trips to be made
	char buf[100];
	thread *cannibals[c];   //array of cannibal threads
        thread *missionaries[m];   //array of missionary threads
        thread *boat;   //boat thread
        int i = 0;
	x = 1;

        boat = new thread(0, 0);
        boat->Begin();
        sprintf(buf, "***** BOAT thread starts\n");
        write(1, buf, strlen(buf));

        for (i = 1; i <= c; i++){   //cannibal threads
		sprintf(buf, "%*sCannibal %d starts\n", i, "", i);
                write(1, buf, strlen(buf));
                cannibals[i] = new thread(i, 1);
                cannibals[i]->Begin();
        }
        for (i = 1; i <= m; i++){   //missionary threads
		sprintf(buf, "%*sMissionary %d starts\n", i, "", i);
                write(1, buf, strlen(buf));
                missionaries[i] = new thread(i, 2);
                missionaries[i]->Begin();
        }
        boat->Join();   //joins main thread on boat thread
        Exit();
        return 0;
}
