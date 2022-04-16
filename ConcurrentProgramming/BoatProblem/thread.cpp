// -----------------------------------------------------------
// NAME : Ryan Becotte                       User ID: rrbecott
// DUE DATE : 12/04/2020
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//    Carries out behavior for cannibal, missionary, and
//    boat threads
// -----------------------------------------------------------

#include <ThreadClass.h>
#include "thread.h"
#include "boat-monitor.h"
#include <string.h>
#include <stdio.h>
using namespace std;

static BoatMonitor mon("Boat");

thread::thread(int num, int type) : num(num), type(type)
{

}

void thread::ThreadFunc()
{
	Thread::ThreadFunc();
	char buf[100];
	if (type == 0){   //boat
		while (1){
			Delay();
			sprintf(buf, "***** The boat is ready\n");
                	write(1, buf, strlen(buf));
			mon.BoatReady();
			Delay();
			sprintf(buf, "***** Boat load (%d): Completed\n", x);
			write(1, buf, strlen(buf));
			mon.BoatDone();
		}
	} else if (type == 1){   //cannibal
		while (1){
			Delay();
			sprintf(buf, "%*sCannibal %d arrives\n", num, "", num);
			write(1, buf, strlen(buf));
			mon.CannibalArrives(num);
		}
	} else if (type == 2){   //missionary
		while (1){
			Delay();
			sprintf(buf, "%*sMissionary %d arrives\n", num, "", num);
                        write(1, buf, strlen(buf));
			mon.MissionaryArrives(num);
		}
	}
}
