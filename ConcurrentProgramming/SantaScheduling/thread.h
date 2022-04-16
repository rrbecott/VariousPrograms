/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/20/2020                         */
/* PROGRAM ASSIGNMENT #4                        */
/* FILE NAME: thread.h                          */
/* PROGRAM PURPOSE:                             */
/*    Header for the thread class               */
/* -------------------------------------------- */

#include <ThreadClass.h>
using namespace std;

extern int e;
extern int r;
extern int t;
extern int nums[3];
extern int rcount;
extern int ecount;
extern bool AllReindeer;
extern bool ThreeElves;
extern Semaphore ElfQuestions;
extern Semaphore ElfCheck;
extern Semaphore SantaSleep;
extern Semaphore ReindeerWait;
extern Semaphore Sleigh;
extern Semaphore Deliver;
extern Mutex Mlock;

class thread : public Thread
{
        public:
                thread(int num, int type);
        private:
                void ThreadFunc();
		int num;
		int type;
};
