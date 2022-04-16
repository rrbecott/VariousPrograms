/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/02/2020                         */
/* PROGRAM ASSIGNMENT #3                        */
/* FILE NAME: thread.h                          */
/* PROGRAM PURPOSE:                             */
/*    Header for the thread class               */
/* -------------------------------------------- */

#include <ThreadClass.h>
using namespace std;

extern int** B;  //global array
class thread : public Thread
{
	public:
		thread(int k, int n);
	private:
		void ThreadFunc();
		int n;  //thread number
		int k;  //iteration
};
