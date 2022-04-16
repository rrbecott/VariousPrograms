/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 10/19/2020                         */
/* PROGRAM ASSIGNMENT #2                        */
/* FILE NAME: merge.c                           */
/* PROGRAM PURPOSE:                             */
/*    Splits input array into sorted 2 element  */
/*    arrays and merges the sorted arrays       */
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

int main(int argc, char** argv){
	
	char buf[100];
	int left = atoi(argv[1]);
	int right = atoi(argv[2]);
	int n = right - left + 1;   /*size of array*/
	
	key_t key = atoi(argv[3]);   /*key for shared array*/
	int ShmID = shmget(key, n*sizeof(int), IPC_CREAT | 0666);   /*ShmID for shared array*/
	int *shared = (int *) shmat(ShmID, NULL, 0);   /*creates shared array*/

	key_t key2 = atoi(argv[5]);   /*key for shared temporary array*/
        int ShmID2 = shmget(key2, n*sizeof(int), IPC_CREAT | 0666);   /*ShmID for shared temporary array*/
        int *temp = (int *) shmat(ShmID2, NULL, 0);   /*creates shared temporary array*/

	if (getppid() == atoi(argv[4])){   /*if this process is the first child*/
		sprintf(buf, "   ### M-PROC(%d): entering with a[%d..%d]\n", getpid(), left, right);
	} else {	
		sprintf(buf, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d]\n", getpid(), getppid(), left, right);
	}
	write(1, buf, strlen(buf));

	int i = 0;
	sprintf(buf, "      ");
	write(1, buf, strlen(buf));
	for (i = left; i <= right; i++){   /*prints shared array section*/
		sprintf(buf, "%d  ", shared[i]);
        	write(1, buf, strlen(buf));
	}
	sprintf(buf, "\n");
        write(1, buf, strlen(buf));

	int middle;
	int temporary;
	if ((right - left) == 1){   /*if array size is 2*/
		if (shared[left] > shared[right]){   /*if elements are not in right order swap*/
			temporary = shared[left];
			shared[left] = shared[right];
			shared[right] = temporary;
		}
		sprintf(buf, "   ### M-PROC(%d) created by M-PROC(%d): entering with a[%d..%d] -- sorted\n", getpid(), getppid(), left, right);
		write(1, buf, strlen(buf));
		sprintf(buf, "      ");
        	write(1, buf, strlen(buf));
		for (i = left; i <= right; i++){   /*prints sorted shared array section*/
                	sprintf(buf, "%d  ", shared[i]);
               	 	write(1, buf, strlen(buf));
        	}
        	sprintf(buf, "\n");
        	write(1, buf, strlen(buf));
		return 1;
	}
	middle = (right + left) / 2;

	char *args[5];   /*arg array to execvp into merge.c with*/
        args[0] = "./merge";   /*program name*/
	char lstring[100];
	sprintf(lstring, "%d", left);
        args[1] = lstring;   /*left index*/
        char rstring[100];
        sprintf(rstring, "%d", middle);
        args[2] = rstring;   /*right index*/
        args[3] = argv[3];   /*shared array key*/
	args[4] = argv[4];   /*first parent id*/
	args[5] = argv[5];   /*shared temporary array key*/
        args[6] = '\0';   /*null terminator*/

	int status;

	pid_t pid;
        if ((pid = fork()) == 0){   /*execvp call for left side of array*/
                if (execvp(args[0], args) < 0){
                        printf("execvp() failed\n");
                        exit(1);
                } else {
                        wait(&status);
                }
        } else {
                wait(&status);
        }	

	char lstring2[100];
	sprintf(lstring2, "%d", (middle + 1));
	args[1] = lstring2;   /*left index*/
	char rstring2[100];
	sprintf(rstring2, "%d", right);
	args[2] = rstring2;   /*right index*/

	pid_t pid2;
        if ((pid2 = fork()) == 0){   /*execvp call for right side of array*/
                if (execvp(args[0], args) < 0){
                        printf("execvp() failed\n");
                        exit(1);
                } else {
                        wait(&status);
                }
        } else {
                wait(&status);
        }

	i = 0;
	int j = 0;
	int index;   /*index of final position in merged array*/
	pid_t pid3;
	for (i = left; i <= middle; i++){   /*merge process for left side of array*/
		if ((pid3 = fork()) == 0){
			sprintf(buf, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n", getpid(), getppid(), i, shared[i]);
                	write(1, buf, strlen(buf));
			index = i;
			for (j = (middle + 1); j <= right; j++){   /*for each element in right side of array*/
				if (shared[i] > shared[j]){   
					index++;   /*increment index if greater than element in right side of array*/
				} else {
					break;
				}
			}
			if (index == left){
                        	sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n", getpid(), i, shared[i], j, shared[j], index);
                        } else if (index == right){
				sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d]\n", getpid(), i, shared[i], j - 1, shared[j - 1], index);
                        } else {
				sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n", getpid(), i, shared[i], j-1, shared[j-1], j, shared[j], index);
                        }
			write(1, buf, strlen(buf));
			temp[index] = shared[i];
			exit(1);
		} else {
			wait(&status);
		}
	}

	for (i = (middle + 1); i <= right; i++){   /*merge process for right side of array*/
                if ((pid3 = fork()) == 0){
			sprintf(buf, "      $$$ B-PROC(%d): created by M-PROC(%d) for a[%d] = %d is created\n", getpid(), getppid(), i, shared[i]);
                        write(1, buf, strlen(buf));
			index = i - (middle + 1);
			for (j = left; j <= middle; j++){   /*for each element in left side of array*/
				if (shared[i] > shared[j]){
					index++;   /*increment index if greater than element in left side of array*/
				} else {
					break;
				}
			}
			if (index == left){
                                sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is smaller than a[%d] = %d and is written to temp[%d]\n", getpid(), i, shared[i], j, shared[j], index);
                        } else if (index == right){
                                sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is larger than a[%d] = %d and is written to temp[%d]\n", getpid(), i, shared[i], j - 1, shared[j - 1], index);
                        } else {
                                sprintf(buf, "      $$$ B-PROC(%d): a[%d] = %d is between a[%d] = %d and a[%d] = %d and is written to temp[%d]\n", getpid(), i, shared[i], j-1, shared[j-1], j, shared[j], index);
                        }
			write(1, buf, strlen(buf));
			temp[index + left] = shared[i];
			exit(1);
                } else {
                        wait(&status);
                }
        }
	
	while (pid = wait(&status) > 0){   /*wait for all children*/
        }

	for (i = left; i <= right; i++){   /*copy elements from shared temporary back to shared array*/
		shared[i] = temp[i];
	}

	if (getppid() == atoi(argv[4])){   /*if first child*/
		sprintf(buf, "   ### M-PROC(%d): entering with a[%d..%d] completed:\n", getpid(), left, right);
	} else {
		sprintf(buf, "   ### M-PROC(%d): created by M-PROC(%d): merge sort a[%d..%d] completed:\n", getpid(), getppid(), left, right);
	}
	write(1, buf, strlen(buf));
	sprintf(buf, "      ");
	write(1, buf, strlen(buf));
	for (i = left; i <= right; i++){   /*print shared array section*/
		sprintf(buf, "%d   ", shared[i]);
		write(1, buf, strlen(buf));
	}
	sprintf(buf, "\n");
        write(1, buf, strlen(buf));

	return 1;
}
