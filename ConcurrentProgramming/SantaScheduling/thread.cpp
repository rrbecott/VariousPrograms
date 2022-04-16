/* -------------------------------------------- */
/* NAME: Ryan Becotte         User ID: rrbecott */
/* DUE DATE: 11/20/2020                         */
/* PROGRAM ASSIGNMENT #4                        */
/* FILE NAME: thread.cpp                        */
/* PROGRAM PURPOSE:                             */
/*    Creates procedure for Santa, reindeer,    */
/*    and elf threads that run concurrently     */
/* -------------------------------------------- */

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "thread.h"
#include "thread-support.h"
using namespace std;

thread::thread(int num, int type) : num(num), type(type)
{

}

// ----------------------------------------------------------- 
// FUNCTION  ThreadFunc :                           
//     Defines Santa thread, reindeer threads, and elf threads                            
// PARAMETER USAGE :                                           
//                   
// FUNCTION CALLED :                                           
//    ThreadFunc, threadsupport, Sleep, Signal, Wait,
//    Delay, strlen, sprintf, write, Unlock, Exit, 
//    ReindeerBack, WaitOthers, WaitSleigh, FlyOff,
//    AskQuestion         
// -----------------------------------------------------------
void thread::ThreadFunc()
{
	Thread::ThreadFunc();
	int counter = 0;   //counter for how many trips made
	int i;
	char buf[100];
	threadsupport *support;
	support = new threadsupport();
	bool retired = false;   //bool if santa is retired
	if (type == 0){   //If it is the Santa thread
		while(!retired){
			support->Sleep();   //Santa sleeps
			if (AllReindeer){   //if all reindeer are back
				for (i = 0; i < r; i++){   //release all waiting reindeer
					ReindeerWait.Signal();
				}
				sprintf(buf, "Santa is preparing sleighs\n", num);
                        	write(1, buf, strlen(buf));
				for (i = 0; i < r; i++){   //release all waiting reindeer for sleigh
					Sleigh.Signal();
				}
				counter = counter + 1;
				sprintf(buf, "The team flies off (%d)!\n", counter);
                                write(1, buf, strlen(buf));
				Delay();
				AllReindeer = false;   //released all reindeer
				rcount = 0;   //now have 0 reindeer back
				if (counter != t){
					for (i = 0; i < r; i++){   //release all reindeer for their trip
						Deliver.Signal();
					}
				}
				if (counter == t){   //if last trip
					sprintf(buf, "After (%d) deliveries, Santa retires and is on vacation!\n", counter);
                                	write(1, buf, strlen(buf));
					retired = true;
					Exit();
				}
				Mlock.Unlock();   //mutex for santa
			}
			else if (ThreeElves){   //if there are three elves waiting
				sprintf(buf, "Santa is helping elves\n");
        			write(1, buf, strlen(buf));
				Delay();
				sprintf(buf, "Santa answers the question posted by elves %d, %d, %d\n", nums[0], nums[1], nums[2]);
        			write(1, buf, strlen(buf));
				ThreeElves = false;   //took care of three elves
				ElfQuestions.Signal();   //release the three elves
				ElfQuestions.Signal();
				ElfQuestions.Signal();
				sprintf(buf, "         Elves %d, %d, %d return to work\n", nums[0], nums[1], nums[2]);
				write(1, buf, strlen(buf));
				nums[0] = 0;
				nums[1] = 0;
				nums[2] = 0;
				Mlock.Unlock();   //mutex for santa
				ElfCheck.Signal();
				ElfCheck.Signal();
				ElfCheck.Signal();
			}
		}
		Exit();
	}
	if (type == 1){   //if it is a reindeer thread
		while(1){
			Delay();
			support->ReindeerBack(num);
			support->WaitOthers();
			support->WaitSleigh();
			support->FlyOff();
			Delay();
		}
	}
	if (type == 2){   //if it is an elf thread
                while (1){
                        Delay();
                        support->AskQuestion(num);
                        Delay();
                }
        }
}
