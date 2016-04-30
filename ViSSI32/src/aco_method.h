#ifndef ACOMETHOD_H
#define ACOMETHOD_H

#include "si_method.h"
#include "vssi_util.h"
#include <cstdio>
#include <json\json.h>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;


#define JSON_NAME_CITIES "cities"
#define JSON_X "x"
#define JSON_Y "y"

class ACOConfig : public MethodConfig
{
public:
	int population;
	int max_t;
	float T0;
	float P0;
	const char * path;

	ACOConfig(const char *path) : path(path)
	{}
};

class TSP_function
{
public:
	TSP_function(const char * filename)
	{
		x_min = y_max = y_min = y_max = 0;

		ifstream is;
		is.open(filename, std::ios::binary);

		Json::Reader reader;
		Json::Value root;
		if (reader.parse(is, root))
		{
			c_num = root[JSON_NAME_CITIES].size();
			for (int i = 0; i < root[JSON_NAME_CITIES].size(); ++i)
			{
				int x = root[JSON_NAME_CITIES][i][JSON_X].asInt();
				int y = root[JSON_NAME_CITIES][i][JSON_Y].asInt();
				coords.push_back(x);
				coords.push_back(y);

				if (x < x_min)
					x_min = x;
				else if (x > x_max)
					x_max = x;
				if (y < y_min)
					y_min = y;
				else if (y > y_max)
					y_max = y;
			}

		}
		else
		{
			printf("cannot open file: %s", filename);
			throw;
			//throw CONFIG_NOT_VALID_EXCEPTION;
		}
		is.close();
	}

	float getRes(int * r)
	{
		float sum = 0;
		for (int i = 0; i < c_num - 1; ++i)
		{
			int &a = r[i];
			int &b = r[i + 1];
			sum += Util::dist(coords[a*2], coords[a*2+1], coords[b*2], coords[b*2+1]);
		}
		return sum;
	}

	float getDistOf(int a, int b)
	{
		return Util::dist(coords[a * 2], coords[a * 2 + 1], coords[b * 2], coords[b * 2 + 1]);
	}

	int x_min, y_min, x_max, y_max;
	int c_num;
	vector<int> coords;

};

class Route
{
public:
	Route() :c_num(0), route(NULL),value(0)
	{}

	Route(int size, TSP_function * func) :c_num(size), route(NULL)
	{
		route = new int[c_num];
		for (int i = 0; i < c_num; ++i)
			route[i] = i;
		value = func->getRes(route);
	}

	~Route()
	{
		if (route)
			delete[] route;
	}

	void updateValue(TSP_function * func)
	{
		value = func->getRes(route);
	}

	int& operator[](const int& i)
	{
		return route[i];
	}

	void assign(const Route & r)
	{
		value = r.value;
		c_num = r.c_num;
		if (route)
			delete[] route;
		route = new int[c_num];
		for (int i = 0; i < c_num; ++i)
			route[i] = r.route[i];

	}

	void print()
	{
		printf("Routine: ");
		for (int i = 0; i < c_num; i++)
			printf("%d ->", route[i]);
		printf(" Value: %f\n", value);
	}

	float value;
	int c_num;
	int * route;
};

class ListNode
{
public:
	int value;
	ListNode * next, *previous;
	ListNode(int v) :
		next(NULL), previous(NULL), value(v)
	{
	}
};

class LinkedList
{
public:
	LinkedList(int start, int end) :
		start(start), end(end)
	{
		size = end - start + 1;
		list = new ListNode(start);
		ListNode * current = list;
		for (int i = start + 1; i <= end; ++i)
		{
			current->next = new ListNode(i);
			current->next->previous = current;
			current = current->next;
		}
	}

	bool erase(int v)
	{
		ListNode * current = list;
		while (current != NULL)
		{
			if (current->value == v)
			{
				
				if (current->previous != NULL)
					current->previous->next = current->next;
				else
					list = current->next;

				if (current->next != NULL)
					current->next->previous = current->previous;
					
				delete current;
				current = NULL;
				--size;
				return true;
			}
			current = current->next;
		}
		return false;
	}

	int randEle()
	{
		int off = rand() % size;
		ListNode * current = list;
		for (int i = 0; i < off; ++i)
			current = current->next;
		return current->value;
	}

	~LinkedList()
	{
		ListNode * current = list;
		while (current != NULL)
		{
			ListNode *tmp = current;
			current = current->next;
			delete tmp;
		}
	}

	void print()
	{
		ListNode * current = list;
		while (current != NULL)
		{
			printf("%d -> ", current->value);
			current = current->next;
		}
		printf("|||\n");
	}

	ListNode * list;
	int size;
	int start, end;
};

class ACOMethod: public SIMethod
{
public:
	Route ** routes;

	Route * gb_route;

	TSP_function* tsp_function;

	float ** pher, //pheromone
		** prob,
		** note;

	float T0, P0;

	float q;

	float a, b;

	int max_t;

	int a_num, c_num;

	float ph_max = 310;

	float ph_min = 0.1;

	ACOMethod(ACOConfig * config);

	~ACOMethod();

	void setup(int c_num);

	void updatePher();

	void updateAnt(int x);

	void step0();

	virtual void iterate_do();

	int g_function(int x);
};

#endif