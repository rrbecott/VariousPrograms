// ----------------------------------------------------------- 
//NAME : Ryan Becotte                       User ID: rrbecott 
// DUE DATE : 12/04/2020                                       
// PROGRAM ASSIGNMENT #5                                        
// FILE NAME : boat-monitor.cpp            
// PROGRAM PURPOSE :                                           
//    Implements functions for the boat monitor for the
//    cannibal-missionary river crossing problem        
// ----------------------------------------------------------- 

#include <ThreadClass.h>
#include "boat-monitor.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

// ----------------------------------------------------------- 
// FUNCTION  BoatMonitor :                          
//    Constructor for the boat monitor                            
// PARAMETER USAGE :                                           
//    Name - the monitor name               
// FUNCTION CALLED :                                           
//              
// ----------------------------------------------------------- 
BoatMonitor::BoatMonitor(char* Name) : Monitor(Name, HOARE)
{
	waitingCan = 0;   //Cannibals waiting in queue
	waitingMis = 0;   //Missionaries waiting in queue
	passengers[3][2] = {};   //Current passengers on the boat
	Cannibals = new Condition("Cannibals");   //Cannibal queue
	Missionaries = new Condition("Missionaries");   //Missionary queue
	onTrip = new Condition("OnTrip");   //Queue for passengers on boat trip
	safeLoads = new Condition("SafeLoad");   //Checks if there is a safe combination
}

// ----------------------------------------------------------- 
// FUNCTION  CannibalArrives :                          
//    Makes a cannibal wait for the boat                            
// PARAMETER USAGE :                                           
//    num - thread number               
// FUNCTION CALLED :                                           
//    SafeLoad, sprintf, write, strlen, Wait, Signal, LoadPassenger          
// ----------------------------------------------------------- 
void BoatMonitor::CannibalArrives(int num)
{
	MonitorBegin();
		if (SafeLoad()){   //if there is a safe combo
			safeLoads->Signal();   //signal boat
		}
		waitingCan++;
		Cannibals->Wait();   //wait in cannibal queue
		LoadPassenger(num, 1);   //loads cannibal into boat
		onTrip->Wait();   //on the trip
		waitingCan--;
	MonitorEnd();
}

// -----------------------------------------------------------
// FUNCTION  MissionaryArrives :
//    Makes a missionary wait for the boat
// PARAMETER USAGE :
//    num - thread number
// FUNCTION CALLED :
//    SafeLoad, sprintf, write, strlen, Wait, Signal, LoadPassenger
// -----------------------------------------------------------
void BoatMonitor::MissionaryArrives(int num)
{
	MonitorBegin();
		if (SafeLoad()){   //if there is a safe combo
			safeLoads->Signal();   //signal boat
		}
                waitingMis++;
		Missionaries->Wait();   //wait in missionary queue
		LoadPassenger(num, 2);   //loads missionary into boat
		onTrip->Wait();   //on the trip
		waitingMis--;
        MonitorEnd();
}

// -----------------------------------------------------------
// FUNCTION  BoatReady :
//    Checks if the boat can be used and loads passengers
// PARAMETER USAGE :
//    
// FUNCTION CALLED :
//    SafeLoad, sprintf, write, strlen, Wait, GetPassengers
// -----------------------------------------------------------
void BoatMonitor::BoatReady()
{
	MonitorBegin();
		if (!SafeLoad()){   //if there is no safe combo
			safeLoads->Wait();   //the boat waits
		}
		GetPassengers();
		if (passengers[0][1] + passengers[1][1] + passengers[2][1] == 3){
			sprintf(buf, "MONITOR(%d): three cannibals (%d, %d, %d) are selected\n", x, passengers[0][0], passengers[1][0], passengers[2][0]);
		} else if (passengers[0][1] + passengers[1][1] + passengers[2][1] == 5){
			if (passengers[0][1] == 1){
				sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", x, passengers[0][0], passengers[1][0], passengers[2][0]);
			} else if (passengers[1][1] == 1){
				sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", x, passengers[1][0], passengers[0][0], passengers[2][0]);
			} else {
				sprintf(buf, "MONITOR(%d): one cannibal (%d) and two missionaries (%d, %d) are selected\n", x, passengers[2][0], passengers[0][0], passengers[1][0]);
			}
		} else {
			sprintf(buf, "MONITOR(%d): three missionaries (%d, %d, %d) are selected\n", x, passengers[0][0], passengers[1][0], passengers[2][0]);
		}
		write(1, buf, strlen(buf));
	MonitorEnd();
}

// -----------------------------------------------------------
// FUNCTION  BoatDone :
//    Resets the boat after it completes the trip
// PARAMETER USAGE :
//    
// FUNCTION CALLED :
//    sprintf, write, strlen, Exit
// -----------------------------------------------------------
void BoatMonitor::BoatDone()
{
	MonitorBegin();
		passengers[0][0] = 0;   //resets passenger array
		passengers[1][0] = 0;
		passengers[2][0] = 0;
		sprintf(buf, "MONITOR: %d crosses have been made.\n", x);
                write(1, buf, strlen(buf));
		if (x >= b){   //if trips are done
			sprintf(buf, "MONITOR: This river cross is closed indefinitely for renovation\n");
			write(1, buf, strlen(buf));
			Exit();
		}
		x++;
		onTrip->Signal();   //releases passengers on boat
		onTrip->Signal();
		onTrip->Signal();
	MonitorEnd();
}

// -----------------------------------------------------------
// FUNCTION  SafeLoad :
//    Checks if a safe combination can be made
// PARAMETER USAGE :
//    
// FUNCTION CALLED :
//    
// -----------------------------------------------------------
bool BoatMonitor::SafeLoad()
{
	if ((waitingMis + waitingCan) >= 4)
		return true;
	else if ((waitingMis >= 2) && (waitingCan >= 1))
		return true;
	else if (waitingMis >= 3)
		return true;
	else if (waitingCan >= 3)
		return true;
	else
		return false;
}

// -----------------------------------------------------------
// FUNCTION  LoadPassenger :
//    Loads passenger information into passenger array
// PARAMETER USAGE :
//    num - thread number
//    type - type of thread
// FUNCTION CALLED :
//    
// -----------------------------------------------------------
void BoatMonitor::LoadPassenger(int num, int type)
{
	if (passengers[0][0] == 0){
        	passengers[0][0] = num;
                passengers[0][1] = type;
        } else if (passengers[1][0] == 0){
                passengers[1][0] = num;
                passengers[1][1] = type;
        } else {
                passengers[2][0] = num;
                passengers[2][1] = type;
        }
}

// -----------------------------------------------------------
// FUNCTION  GetPassengers :
//    Randomly gets passengers to board the boat
// PARAMETER USAGE :
//
// FUNCTION CALLED :
//    rand, Signal
// -----------------------------------------------------------
void BoatMonitor::GetPassengers()
{
	if (waitingCan >= 3 && waitingMis >= 3){
		v1 = rand() % 3;
		if (v1 == 0){
			Cannibals->Signal();
                	Missionaries->Signal();
                	Missionaries->Signal();	
		} else if (v1 == 1){
			Cannibals->Signal();
                	Cannibals->Signal();
                	Cannibals->Signal();
		} else {
			Missionaries->Signal();
                	Missionaries->Signal();
                	Missionaries->Signal();
		}
	} else if (waitingCan >= 1 && waitingMis >= 2){
		Cannibals->Signal();
		Missionaries->Signal();
		Missionaries->Signal();
	} else if (waitingCan >= 3) {
		Cannibals->Signal();
		Cannibals->Signal();
		Cannibals->Signal();
	} else {
		Missionaries->Signal();
		Missionaries->Signal();
		Missionaries->Signal();
	}
}
