#include "aco_method.h"

ACOMethod::ACOMethod(ACOConfig *config)
	:SIMethod(config->max_t, NULL)
{
	a_num = (config->population);
	max_t = (config->max_t);
	T0 = config->T0;
	P0 = config->P0;
	tsp_function = new TSP_function(config->path);
	c_num = tsp_function->c_num;
	setup(c_num);
	gb_route = new Route(c_num, tsp_function);
	step0();
}

ACOMethod::~ACOMethod()
{
	for (int i = 0; i < c_num; ++i)
	{
		delete[] pher[i];
		delete[] prob[i];
		delete[] note[i];
	}
	delete gb_route;

	for (int i = 0; i < a_num; ++i)
	{
		cout << sizeof(*routes[i]) << endl;
		delete routes[i];
	}

	delete[] pher;
	delete[] prob;
	delete[] note;
	delete[] routes;
	delete tsp_function;

}

void ACOMethod::setup(int c_num)
{
	a = 2.0;
	b = 3.0;
	q = 0.05;


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

void ACOMethod::updatePher()
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

void ACOMethod::updateAnt(int x)
{
	int start = 0;
	(*routes[x])[0] = 0;
	LinkedList rest(1, c_num - 1);
	for (int step = 1; step < c_num; step++)
	{
		int numOfCityToChoose = 5;
		pair <int, float>dj(0, 0.0); //record the next city
		float rn = 0;
		int count = 0;

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

	(*routes[x]).value = tsp_function->getRes((*routes[x]).route);

	//local_search()

	for (int i = 0; i < c_num - 1; i++)
		note[(*routes[x])[i]][(*routes[x])[i + 1]] += (*routes[x]).value;

	if ((*routes[x]).value < (*gb_route).value || (*gb_route).value == 0)
	{
		(*gb_route).assign((*routes[x]));
		gb_val = gb_route->value;
	}
		
}

void ACOMethod::iterate_do()
{
	for (int x = 0; x < a_num; ++x)
	{
		updateAnt(x);
	}
	updatePher();
}

void ACOMethod::save_gbest(const char * name)
{
	fstream out(name, ios::out);
	out << name << endl;
	out << "Current Global Best Value: " << gb_route->value << endl;
	out << "Current Global Best Rotine:" << endl;

	for (int i = 0; i < gb_route->c_num; ++i)
	{
		out << gb_route->route[i] << endl;
	}

	out.close();
}

void ACOMethod::step0()
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


}

int ACOMethod::g_function(int x)
{
	return (300 - x / 25);
}