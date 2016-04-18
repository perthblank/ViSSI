#ifndef VSSIUTIL_H
#define VSSIUTIL_H

#include <process.h>  
#include <windows.h> 
#include <string>
#include <sstream>

using namespace std;

template<typename T>
union ThreadProc 
{
	unsigned int(__stdcall * Proc)();
	unsigned int(__stdcall T::*Member)();
};

template<typename T>
class TS
{
public:
	TS()
	{
		sem = CreateSemaphore(NULL, 0, 1, NULL);
		ReleaseSemaphore(sem, 1, NULL);
	}

	void get(T& hand)
	{
		WaitForSingleObject(sem);
		hand = data;
		ReleaseSemaphore(sem, 1, NULL);
	}
	void set(const T& data)
	{
		WaitForSingleObject(sem);
		this->data = data;
		ReleaseSemaphore(sem, 1, NULL);
	}
private:
	T data;
	HANDLE sem;
};

class Util
{
public:
	static float dist(int x1, int y1, int x2, int y2)
	{
		return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
	}

	static void exchange(int & a, int & b)
	{
		int tmp = a;
		a = b;
		b = tmp;
	}
};




#endif // !UTIL_H

