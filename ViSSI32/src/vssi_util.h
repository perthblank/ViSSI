#ifndef VSSIUTIL_H
#define VSSIUTIL_H

#include <process.h>  
#include <windows.h> 
#include <string>
#include <sstream>
#include <vector>

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

	static bool inVec(vector<int> &vec, const int& i)
	{
		vector<int>::iterator it = vec.begin();
		while (it != vec.end())
			if (*it++ == i)
				return true;

		return false;
	}

	static float rand01()
	{
		return rand() % 100 * 0.01;
	}

	static int wheelGame(vector<float> probs)
	{
		float prob = Util::rand01();
		for (int i = 0; i < probs.size(); ++i)
		{
			//cout << probs[i] << endl;
			prob -= probs[i];
			if (prob <= 0)
				return i;
		}
		return -1; // probs not valid
	}

	static bool equalf(float a, float b)
	{
		return (a - b < 0.000001);
	}


};



#endif // !UTIL_H