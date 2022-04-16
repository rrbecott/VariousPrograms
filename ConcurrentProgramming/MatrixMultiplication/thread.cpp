// -----------------------------------------------------------
// NAME : Ryan Becotte                      User ID: rrbecott
// DUE DATE : 12/11/2020
// PROGRAM ASSIGNMENT #6
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//    Defines behavior for row, column, and processor threads
// -----------------------------------------------------------

#include <string.h>
#include <stdio.h>
#include <ThreadClass.h>
#include "thread.h"
using namespace std;

extern int** C;

rthread::rthread(int id, int* A, int size, pthread *proc) : id(id), A(A), size(size), proc(proc)
{
	UserDefinedThreadID = id;
}

void rthread::ThreadFunc()
{
	Thread::ThreadFunc();
	char buf[100];
	int i;
	for (i = 0; i < size; i++){
		proc->channel1->Send(&A[i], sizeof(int));
		sprintf(buf, "Row thread r[%d] sent %d to P[%d,1]\n", (id / 10), A[i], (id / 10));
		write(1, buf, strlen(buf));
	}
	int end = -1;
	proc->channel1->Send(&end, sizeof(int));
	sprintf(buf, "Row thread r[%d] sent EOD to P[%d,1] and terminated\n", (id / 10), (id / 10));
        write(1, buf, strlen(buf));
	Exit();
}

cthread::cthread(int id, int* B, int size, pthread *proc) : id(id), B(B), size(size), proc(proc)
{
	UserDefinedThreadID = id;
}

void cthread::ThreadFunc()
{
        Thread::ThreadFunc();
	int i;
	char buf[100];
	for (i = 0; i < size; i++){
		proc->channel2->Send(&B[i], sizeof(int));
		sprintf(buf, "   Column thread c[%d] sent %d to P[1,%d]\n", id, B[i], id);
                write(1, buf, strlen(buf));
	}
	int end = -1;
	proc->channel2->Send(&end, sizeof(int));
	sprintf(buf, "   Column thread c[%d] sent EOD to P[1,%d] and terminated\n", id, id);
        write(1, buf, strlen(buf));
	Exit();
}

pthread::pthread(int id, pthread *neighbor1, pthread *neighbor2) : id(id), neighbor1(neighbor1), neighbor2(neighbor2)
{
	UserDefinedThreadID = id;
	sum = 0;
	channel1 = new SynOneToOneChannel("Channel1", id - 1, id);
	channel2 = new SynOneToOneChannel("Channel2", id - 10, id);
}

void pthread::ThreadFunc()
{
        Thread::ThreadFunc();
	char buf[100];
	while(1){
		channel1->Receive(&num1, sizeof(int));
		channel2->Receive(&num2, sizeof(int));
		if (num1 == -1 && num2 == -1){
                        C[(id / 10) - 1][(id % 10) - 1] = sum;
			sprintf(buf, "      Thread P[%d,%d] received EOD, saved result %d and terminated\n", (id / 10), (id % 10), sum);
                        write(1, buf, strlen(buf));
			break;
                }
		sprintf(buf, "      Thread P[%d,%d] received %d from above and %d from left\n", (id / 10), (id % 10), num2, num1);
                write(1, buf, strlen(buf));
		sum += (num1 * num2);
		if (neighbor1 != NULL){
			neighbor1->channel1->Send(&num1, sizeof(int));
		}
		if (neighbor2 != NULL){
			neighbor2->channel2->Send(&num2, sizeof(int));
		}				
		sprintf(buf, "      Thread P[%d,%d] sent %d to below and %d to right\n", (id / 10), (id % 10), num2, num1);
                write(1, buf, strlen(buf));
	}
	if (neighbor1 != NULL){
        	neighbor1->channel1->Send(&num1, sizeof(int));
        }
        if (neighbor2 != NULL){
                neighbor2->channel2->Send(&num2, sizeof(int));
        }
	Exit();
}
