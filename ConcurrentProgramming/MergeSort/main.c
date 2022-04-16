/* -------------------------------------------- */ 
/* NAME: Ryan Becotte         User ID: rrbecott */ 
/* DUE DATE: 10/19/2020                         */ 
/* PROGRAM ASSIGNMENT #2                        */ 
/* FILE NAME: main.c                            */ 
/* PROGRAM PURPOSE:                             */
/*    Completes merge sort using multiple       */
/*    processes and shared memory               */
/* -------------------------------------------- */
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/types.h>
#include <unistd.h> 
#include <string.h>
#include <limits.h>
#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void main(void){

	int n = 0;   /*size of input array*/
	char buf[100];
	int status;

	sprintf(buf, "\nMerge Sort with Multiple Processes:\n\n");
        write(1, buf, strlen(buf));

	fscanf(stdin, "%d", &n);   /*gets size of input array*/

	key_t key = ftok("./", 'R');   /*key for shared array*/
	sprintf(buf, "*** MAIN: shared memory key = %d\n", key);
        write(1, buf, strlen(buf));
        int ShmID = shmget(key, n*sizeof(int), IPC_CREAT | 0666);   /*ShmID for shared array*/
	
        int *shared = (int *) shmat(ShmID, NULL, 0);   /*creates shared array*/
	sprintf(buf, "*** MAIN: shared memory created\n");
        write(1, buf, strlen(buf));

	int temporary = 0;
	int i = 0;
	while (fscanf(stdin, "%d", &temporary) == 1){   /*gets all input elements and puts it into shared array*/
		shared[i] = temporary;
		i++;
	}
	sprintf(buf, "*** MAIN: shared memory attached and is ready to use\n\n");
        write(1, buf, strlen(buf));

	key_t key2 = ("./", 'B');   /*key for temporary shared array*/
        int ShmID2 = shmget(key2, n*sizeof(int), IPC_CREAT | 0666);   /*ShmID for temporary shared array*/
        int *temp = (int *) shmat(ShmID2, NULL, 0);   /*creates temporary shared array*/
        sprintf(buf, "*** MERGE: shared memory key = %d\n", key2);
        write(1, buf, strlen(buf));
        sprintf(buf, "*** MERGE: shared memory created\n");
        write(1, buf, strlen(buf));
        sprintf(buf, "*** MERGE: shared memory is attached and is ready to use for temp array purpose\n");
        write(1, buf, strlen(buf));

	sprintf(buf, "Input array for mergesort has %d elements:\n", n);
        write(1, buf, strlen(buf));
	for (i = 0; i < n; i++){   /*prints the shared array*/
		sprintf(buf, "   %d", shared[i]);
       		write(1, buf, strlen(buf));
	}

	sprintf(buf, "\n\n*** MAIN: about to spawn the merge sort process\n");
        write(1, buf, strlen(buf));

	char *args[6];   /*argument array to pass in to merge.c*/
	args[0] = "./merge";   /*program name*/
	args[1] = "0";   /*left index*/
	char rstring[100];
	sprintf(rstring, "%d", (n-1));
	args[2] = rstring;   /*right index*/
	char sharedkey[100];
	sprintf(sharedkey, "%d", key);
	args[3] = sharedkey;   /*shared array key*/
	char pidstring[100];
	sprintf(pidstring, "%d", getpid());
	args[4] = pidstring;   /*first parent id*/
	char tempkey[100];
	sprintf(tempkey, "%d", key2);
	args[5] = tempkey;   /*temporary array key*/
	args[6] = '\0';   /*null terminator*/

	pid_t pid;
	if ((pid = fork()) == 0){
		if (execvp(args[0], args) < 0){   /*child execvp merge.c with args array*/
			printf("execvp() failed\n");
			exit(1);
		} else {
			wait(&status);
		}
	} else {
		wait(&status);
	}

	for (i = 0; i < n; i++){   /*copy temporary shared array to shared array*/
		shared[i] = temp[i];
	}
	
	sprintf(buf, "*** MAIN: merged array:\n");
        write(1, buf, strlen(buf));
	sprintf(buf, "      ");
        write(1, buf, strlen(buf));
        for (i = 0; i < n; i++){   /*prints out shared array*/
                sprintf(buf, "%d   ", shared[i]);
                write(1, buf, strlen(buf));
        }

	shmdt(shared);   /*detach shared memory from shared array*/
	shmdt(temp);   /*detach shared memory from shared temporary array*/
	sprintf(buf, "\n\n*** MAIN: shared memory successfully detached\n");
        write(1, buf, strlen(buf));
	shmctl(ShmID, IPC_RMID, NULL);   /*deallocates shared memory from shared array*/
	shmctl(ShmID2, IPC_RMID, NULL);   /*deallocates shared memory from shared temporary array*/
	sprintf(buf, "*** MAIN: shared memory successfully removed\n");
        write(1, buf, strlen(buf));
	sprintf(buf, "*** MAIN: exits\n");
        write(1, buf, strlen(buf));
	exit(1);
}
