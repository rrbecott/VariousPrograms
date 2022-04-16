/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/20/2020                         */
/* PROGRAM ASSIGNMENT #4                        */
/* FILE NAME: thread-support.cpp                */
/* PROGRAM PURPOSE:                             */
/*    Implements helper methods for threads     */
/* -------------------------------------------- */

#include "thread.h"
#include "thread-support.h"
#include <stdio.h>
#include <string.h>
using namespace std;

int rcount = 0;   //number of reindeer returned
int ecount = 0;   //number of elves waiting
int nums[3] = {0, 0, 0};   //elves being helped by santa
bool AllReindeer = false;   //bool if we have all reindeer back
bool ThreeElves = false;   //bool if there are three elves waiting
Semaphore ElfQuestions("Elf", 0);
Semaphore ElfCheck("Elf Check", 3);
Semaphore SantaSleep("Santa", 0);
Semaphore ReindeerWait("Reindeer Wait", 0);
Semaphore ElfQueue("Elf Queue", 1);
Semaphore ReindeerQueue("Reindeer Queue", 1);
Semaphore Sleigh("Sleigh", 0);
Semaphore Deliver("Flying", 0);
Mutex Mlock;   //mutex for santa

threadsupport::threadsupport()
{
	
}

// ----------------------------------------------------------- 
// FUNCTION  AskQuestion :                          
//    Executes when an elf asks a question, creates an elf queue                            
// PARAMETER USAGE :                                           
//    num - The thread number               
// FUNCTION CALLED :                                           
//    Wait, strlen, write, sprintf, Signal          
// ----------------------------------------------------------- 
void threadsupport::AskQuestion(int num)
{
	ElfQueue.Wait();
	sprintf(buf, "         Elf %d has a problem\n", num);
        write(1, buf, strlen(buf));
	ElfQueue.Signal();
	ElfCheck.Wait();
	if (nums[0] == 0){
		nums[0] = num;
	} else if (nums[1] == 0){
		nums[1] = num;
	} else {
		nums[2] = num;
	}
	if (++ecount == 3){
		ThreeElves = true;
		ecount = ecount - 3;
		Mlock.Lock();
		sprintf(buf, "         Elves %d, %d, %d wake up the Santa\n", nums[0], nums[1], nums[2]);
        	write(1, buf, strlen(buf));
		SantaSleep.Signal();
	}
	ElfQuestions.Wait();
}

// ----------------------------------------------------------- 
// FUNCTION  ReindeerBack :                          
//    Signals that a reindeer is back from its trip                            
// PARAMETER USAGE :                                           
//    num - The thread number               
// FUNCTION CALLED :                                           
//    Wait, Signal, write, sprintf, strlen, Lock          
// ----------------------------------------------------------- 
void threadsupport::ReindeerBack(int num)
{
	ReindeerQueue.Wait();
	if (++rcount == r){
		Mlock.Lock();
		sprintf(buf, "    The last reindeer %d wakes up Santa\n", num);
                write(1, buf, strlen(buf));
		AllReindeer = true;
		SantaSleep.Signal();
	} else {
		sprintf(buf, "    Reindeer %d returns\n", num);
		write(1, buf, strlen(buf));
	}
	ReindeerQueue.Signal();
}

// ----------------------------------------------------------- 
// FUNCTION  WaitOthers :                          
//    Makes a reindeer wait for other reindeer                            
// PARAMETER USAGE :                                           
//                   
// FUNCTION CALLED :                                           
//    Wait          
// ----------------------------------------------------------- 
void threadsupport::WaitOthers()
{
	ReindeerWait.Wait();
}

// -----------------------------------------------------------
// FUNCTION  WaitSleigh :
//    Makes a reindeer wait to attach the sleigh
// PARAMETER USAGE :
//
// FUNCTION CALLED :
//    Wait
// -----------------------------------------------------------
void threadsupport::WaitSleigh()
{
	Sleigh.Wait();
}

// -----------------------------------------------------------
// FUNCTION  FlyOff :
//    Makes a reindeer fly off and deliver presents
// PARAMETER USAGE :
//
// FUNCTION CALLED :
//    Wait
// -----------------------------------------------------------
void threadsupport::FlyOff()
{
	Deliver.Wait();
}

// -----------------------------------------------------------
// FUNCTION  Sleep :
//    Suspends Santa until a signal
// PARAMETER USAGE :
//
// FUNCTION CALLED :
//    Wait
// -----------------------------------------------------------
void threadsupport::Sleep()
{
	SantaSleep.Wait();
}
