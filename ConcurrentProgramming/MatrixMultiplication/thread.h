#include <ThreadClass.h>
using namespace std;

class pthread : public Thread
{
        public:
                pthread(int id, pthread *neighbor1, pthread *neighbor2);
		SynOneToOneChannel *channel1;
                SynOneToOneChannel *channel2;
        private:
                void ThreadFunc();
                int id;
                int sum;
                int num1;
                int num2;
                pthread *neighbor1;
                pthread *neighbor2;
};

class rthread : public Thread
{
	public:
		rthread(int id, int* A, int size, pthread *proc);
	private:
		void ThreadFunc();
		int id;
		int* A;
		int size;
		pthread *proc;
};

class cthread : public Thread
{
        public:
                cthread(int id, int* B, int size, pthread *proc);
        private:
                void ThreadFunc();
		int id;
		int* B;
		int size;
		pthread *proc;
};
