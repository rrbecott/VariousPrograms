#include <ThreadClass.h>
using namespace std;

extern int x;
extern int b;

class BoatMonitor : public Monitor
{
        public:
		BoatMonitor(char* Name);
		void CannibalArrives(int num);
		void MissionaryArrives(int num);
		void BoatReady();
		void BoatDone(); 
		bool SafeLoad();
		void LoadPassenger(int num, int type);               
		void GetPassengers();
        private:
		Condition *onTrip;
		Condition *safeLoads;
		Condition *Cannibals;
		Condition *Missionaries;
		int waitingCan;
		int waitingMis;
		int v1;
		int passengers[3][2];
		char buf[100];
};
