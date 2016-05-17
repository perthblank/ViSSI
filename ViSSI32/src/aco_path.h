#ifndef ACO_PATH_H
#define ACO_PATH_H

#include "si_method.h"
#include "vssi_util.h"
#include <iostream>
#include <cstdio>
#include <ctime>
#include <vector>
#include <cmath>
#include <map>
#include <fstream>
#include <Windows.h>
using namespace std;

class Ant
{
public:
	Ant()
	{
		mutex = CreateMutex(NULL, false, NULL);
	}

	void choose(int i, int dir)
	{
		WaitForSingleObject(mutex, INFINITE);
		tabu.push_back(i);
		cur_length += len_of_dir[dir];
		ReleaseMutex(mutex);
	}

	void clear()
	{
		WaitForSingleObject(mutex, INFINITE);
		tabu.clear();
		is_done = false;
		cur_length = 0;
		ReleaseMutex(mutex);
	}

	float getCurrentLength()
	{
		return cur_length;
	}

	bool contain(int i)
	{
		return Util::inVec(tabu, i);
	}

	void print()
	{
		for (int i = 0; i < tabu.size(); ++i)
			cout << tabu[i] << " ";
		cout << endl;
	}

	void assign(Ant * a)
	{
		tabu = a->tabu;
		cur_length = a->cur_length;
	}

	bool is_done = false;
	vector<int> tabu;
	float cur_length = 0;
	static const float len_of_dir[];
	static const int move[][2];
	HANDLE mutex;
};


class Pher
{
public:
	Pher()
	{
	}

	int id;
	map<int, float> pher;
	float & operator [](const int &i)
	{
		if (Util::equalf(pher[i], 0))
			pher[i] = 0.1;
		return pher[i];
	}
};

class ACO_PathPlan: public SIMethod
{
public:
	ACO_PathPlan(int scale, int a_num, int max_t);

	~ACO_PathPlan();

	bool randGrid();

	void printMap();

	int chooseNext(int x);

	void step0();

	void iterate_do();

	void evap_pher(float f);

	inline void swapPher();

	inline void restore();

	inline bool is_valid_id(int j_id);

	void getRouteArr(vector<int> &v, int x);

	float ita_function(int i);

	virtual float getGBValue()
	{
		return gbest.cur_length;
	}

	int xC(int i);

	int yC(int i);

	int xyId(int x, int y);

	virtual void save_gbest(const char * name);

	int g_begin, g_end;

	int scale, grid_num;

	int a_num;

	float D = 10;

	Ant * ants;

	Ant gbest;

	bool **map;

	int ob_num = 0;

	Pher *pher, *pher_new, *note;

	float Tmin = 0.1, T0 = 0.1;

	float q0 = 0.3;

	float beta = 1.5, alpha = 0.05, ro = 0.05, Q1 = 20, Q2 = 20;
};


#endif