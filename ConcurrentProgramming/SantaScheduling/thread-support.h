/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/20/2020                         */
/* PROGRAM ASSIGNMENT #4                        */
/* FILE NAME: thread-support.h                  */
/* PROGRAM PURPOSE:                             */
/*    Header for the thread support class       */
/* -------------------------------------------- */

#include <ThreadClass.h>
using namespace std;

extern int e;
extern int r;
extern int t;

class threadsupport: public Thread
{
        public:
                threadsupport();
		void AskQuestion(int num);
		void ReindeerBack(int num);
		void WaitOthers();
		void WaitSleigh();
		void FlyOff();
		void Sleep();
	private:
		char buf[100];
};
