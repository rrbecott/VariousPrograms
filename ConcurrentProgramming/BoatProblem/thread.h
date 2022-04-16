#include <ThreadClass.h>
using namespace std;

extern int b;

class thread : public Thread
{
	public:
		thread(int num, int type);
	private:
		void ThreadFunc();
		int type;
		int num;
};
