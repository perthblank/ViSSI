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

#define DEFAULT_TSP_CONFIG_FILE "..\\src\\config\\cities.json"
#define JSON_NAME_CITIES "cities"
#define JSON_X "x"
#define JSON_Y "y"

class Coord
{
public:
	int x, y;
	int id;

	Coord(int x, int y, int id) :
		x(x), y(y), id(id)
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
				Coord c(x, y, i);
				coords.push_back(c);

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
			throw CONFIG_NOT_VALID_EXCEPTION;
		}
		is.close();
	}

	TSP_function() :TSP_function(DEFAULT_TSP_CONFIG_FILE)
	{}

	float getRes(int * r)
	{
		float sum = 0;
		for (int i = 0; i < c_num - 1; ++i)
		{
			int &a = r[i];
			int &b = r[i + 1];
			sum += Util::dist(coords[a].x, coords[a].y, coords[b].x, coords[b].y);
		}
		return sum;
	}

	float getDistOf(int a, int b)
	{
		return Util::dist(coords[a].x, coords[a].y, coords[b].x, coords[b].y);
	}

	int x_min, y_min, x_max, y_max;
	int c_num;
	vector<Coord> coords;

};

class Route
{
public:
	Route() :c_num(0), route(NULL)
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
		if (!route)
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

	int value;
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
				if (current->previous == NULL)
					list = current->next;
				else
					current->previous->next = current->next;
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
	}

	ListNode * list;
	int size;
	int start, end;
};

class ACOMethod
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

	ACOMethod(int a_num, int max_t)
		:a_num(a_num), max_t(max_t)
	{
		tsp_function = new TSP_function;
		c_num = tsp_function->c_num;
		setup(c_num);
		gb_route = new Route(c_num, tsp_function);
	}

	~ACOMethod()
	{
		for (int i = 0; i < c_num; ++i)
		{
			delete[] pher[i];
			delete[] prob[i];
			delete[] note[i];
		}
		for (int i = 0; i < a_num; ++i)
			delete routes[i];

		delete[] pher;
		delete[] prob;
		delete[] note;
		delete[] routes;
		delete gb_route;
		delete tsp_function;

	}

	void setup(int c_num)
	{
		a = 2.0;
		b = 3.0;
		q = 0.05;
		T0 = 110;
		P0 = 0.7;

		pher = (float **) new float *[c_num];
		prob = (float **) new float *[c_num];
		note = (float **) new float *[c_num];

		for (int i = 0; i < c_num; ++i)
		{
			pher[i] = new float[c_num];
			prob[i] = new float[c_num];
			note[i] = new float[c_num];

			for (int j = 0; j < c_num; ++j)
			{
				pher[i][j] = T0;
				prob[i][j] = P0;
			}
		}

		routes = (Route **) new Route *[a_num];

		for (int i = 0; i < a_num; i++)
			routes[i] = new Route(c_num, tsp_function);
		
	}

	void updatePher()
	{
		//deal with pheromone
		for (int i = 0; i < c_num; i++)
		{
			for (int j = 0; j < c_num; j++)
			{
				if (i != j)
				{
					if (note[i][j] != 0)
						pher[i][j] = (1 - q)*pher[i][j] + g_function(note[i][j]);
				}
			}

			//MMAS job
			for (int j = 0; j < c_num - 1; j++) {
				float s;
				s = note[(*gb_route)[j]][(*gb_route)[j + 1]];
				float tmp = (1 - q)*pher[j][(*gb_route)[j + 1]] + g_function(s);
				pher[(*gb_route)[j]][(*gb_route)[j + 1]] = (tmp) ? 410 : ph_max;
			}

			float u2 = 0;

			for (int k = 0; k < c_num; k++)
			{
				if (k != i)
				{
					u2 += pow(pher[i][k], a)*pow(1 / tsp_function->getDistOf(i, k), b);
				}
			}

			for (int j = 0; j < c_num; j++)
			{
				if (i != j)
				{
					float u1 = pow(pher[i][j], a)*pow(1 / tsp_function->getDistOf(i, j), b);
					prob[i][j] = u1 / u2;
				}
			}
		}

		for (int i = 0; i < c_num; i++)
			for (int j = 0; j < c_num; j++)
				note[i][j] = 0;

		for (int i = 0; i < c_num - 1; i++)
			note[(*gb_route)[i]][(*gb_route)[i + 1]] = (*gb_route).value;
	}

	void updateAnt(int x)
	{
		int start = 0;
		(*routes[x])[0] = 0;

		for (int step = 1; step < c_num; step++)
		{
			int numOfCityToChoose = 5;

			pair <int, float>dj(0, 0.0); //record the next city

			float rn = 0;

			int count = 0;

			LinkedList rest(0, c_num - 1);

			for (int j = 0; j < numOfCityToChoose;)
			{
				int e = rest.randEle();
				rn = prob[start][e];

				if (rn >= dj.second || dj.first == 0)
				{
					dj.first = e;
					dj.second = rn;
					count++;
				}
				if (count != 0)
					j++;

			}

			(*routes[x])[step] = dj.first;
			start = dj.first;
			rest.erase(dj.first);

		}
		(*routes[x])[c_num] = 0;
		(*routes[x]).value = tsp_function->getRes((*routes[x]).route);

		//local_search()

		for (int i = 0; i < c_num; i++)
			note[(*routes[x])[i]][(*routes[x])[i + 1]] += (*routes[x]).value;

		if ((*routes[x]).value < (*gb_route).value || (*gb_route).value == 0)
			(*gb_route).assign((*routes[x]));
	}

	bool iterate()
	{
		for (int x = 0; x < a_num; ++x)
		{
			updateAnt(x);
		}
		updatePher();
		(*gb_route).print();
		return true;
	}

	void main()
	{
		const int start = 0;
		for (int x = 0; x < a_num; ++x)
		{
			(*routes[x])[0] = start;

			//exchange for c_num times to make routine random
			for (int s = 0; s < c_num; ++s)
			{
				int a = rand() % c_num;
				int b = rand() % c_num;
				Util::exchange((*routes[x])[a], (*routes[x])[b]);
			}
			(*routes[x]).value = tsp_function->getRes((*routes[x]).route);

			for (int i = 0; i < c_num - 1; ++i)
				note[(*routes[x])[i]][(*routes[x])[i + 1]] += (*routes[x]).value;

			if ((*routes[x]).value < gb_route->value || gb_route->value == 0)
			{
				gb_route->assign((*routes[x]));
			}

		}

		updatePher();

		//iteration 2+ 

		for (int iter = 0; iter < max_t; ++iter)
		{
			iterate();
		}

	}

	int g_function(int x)
	{
		return (300 - x / 25);
	}
};

#endif