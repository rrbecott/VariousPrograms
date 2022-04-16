// ----------------------------------------------------------- 
// NAME : Ryan Becotte                      User ID: rrbecott 
// DUE DATE : 12/11/2020                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread-main.cpp            
// PROGRAM PURPOSE :                                           
//    Starts row, column, and processor threads for matrix
//    multiplication        
// -----------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <ThreadClass.h>
#include "thread.h"
using namespace std;

int** C;

int main(int argc, char* argv)
{
	char buf[100];
	int l;   //matrix A rows
	int m;   //matrix A columns
	int u;   //matrix B rows
	int v;   //matrix B columns
	int i;
	int j;
	fscanf(stdin, "%d %d", &l, &m);
	int A[l][m];

	for (i = 0; i < l; i++){   //reads in matrix A
		for (j = 0; j < m; j++){
			fscanf(stdin, "%d", &A[i][j]);
		}
	}

	fscanf(stdin, "%d %d", &u, &v);
	if (m != u){   //Error if matrices can't be multiplied
		sprintf(buf, "Error: Invalid input, m != u\n");
		write(1, buf, strlen(buf));
	}
	int B[v][u];
	
	for (i = 0; i < u; i++){   //reads in matrix B
                for (j = 0; j < v; j++){
                        fscanf(stdin, "%d", &B[j][i]);
                }
        }

	C = new int*[v];  //initializes global array C
        for (i = 0; i < l; i++){
                C[i] = new int[v];
        }
	rthread *rows[l];   //threads for rows of matrix A
	cthread *cols[v];   //threads for columns of matrix B
	pthread *procs[l*v];   //processor threads

	int offset = l*10;
        int index = (l*v)-1;
        for (i = l; i > 0; i--){   //starts processor threads
                for (j = v; j > 0; j--){
                        if (i == l && j == v){
                                procs[index] = new pthread(offset + j, NULL, NULL);
                        } else if (i == l){
                                procs[index] = new pthread(offset + j, procs[index + 1], NULL);
                        } else if (j == v){
                                procs[index] = new pthread(offset + j, NULL, procs[index + v]);
                        } else {
                                procs[index] = new pthread(offset + j, procs[index + 1], procs[index + v]);
                        }
                        procs[index]->Begin();
			sprintf(buf, "      Thread P[%d,%d] started\n", i, j);
                	write(1, buf, strlen(buf));
                        index--;
                }
        offset -= 10;
        }

	for (i = 1; i <= l; i++){   //starts row threads
                rows[i-1] = new rthread(i * 10, A[i-1], m, procs[(i-1) * v]);
                rows[i-1]->Begin();
                sprintf(buf, "Row thread r[%d] started\n", i);
                write(1, buf, strlen(buf));
        }

	for (i = 1; i <= v; i++){   //starts column threads
		cols[i-1] = new cthread(i, B[i-1], m, procs[i-1]);
                cols[i-1]->Begin();
		sprintf(buf, "   Column thread c[%d] started\n", i);
		write(1, buf, strlen(buf));
	}

	for (i = 0; i < (l*v); i++){   //joins main onto the processor threads
                procs[i]->Join();
        }

	sprintf(buf, "\n*** From main ***\n");
	write(1, buf, strlen(buf));

	sprintf(buf, "Matrix A: %d rows and %d columns\n", l, m);
        write(1, buf, strlen(buf));
        for (i = 0; i < l; i++){   //prints matrix A
                for (j = 0; j < m; j++){
                        sprintf(buf, "%4d ", A[i][j]);
                        write(1, buf, strlen(buf));
                }
                sprintf(buf, "\n");
                write(1, buf, strlen(buf));
        }

	sprintf(buf, "\nMatrix B: %d rows and %d columns\n", u, v);
        write(1, buf, strlen(buf));
        for (i = 0; i < u; i++){   //prints matrix B
                for (j = 0; j < v; j++){
                        sprintf(buf, "%4d ", B[j][i]);
                        write(1, buf, strlen(buf));
                }
                sprintf(buf, "\n");
                write(1, buf, strlen(buf));
        }

	sprintf(buf, "\nMatrix C = A*B: %d rows and %d columns\n", l, v);
        write(1, buf, strlen(buf));
	for (i = 0; i < l; i++){   //prints matrix C
                for (j = 0; j < v; j++){
                        sprintf(buf, "%4d ", C[i][j]);
        		write(1, buf, strlen(buf));
                }
                sprintf(buf, "\n");
        	write(1, buf, strlen(buf));
        }

	return 0;
}
