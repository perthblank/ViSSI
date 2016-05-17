#ifndef FITNESS_FUNCTION_H
#define FITNESS_FUNCTION_H

#include<cmath>

#define FIND_MAX true
#define FIND_MIN false

#define ff_Eggholder "Eggholder_function"
#define ff_Sphere "Sphere_function"
#define ff_HolderTable "HolderTable_function"
#define ff_GoldsteinPrice "GoldsteinPrice_function"
#define ff_Coverage "Continuous_Coverage"
#define ff_TSP "Discrete_TSP"
#define ff_PATH "Path_Planning"

#define PI 3.1415

class FitnessFunction {
public:
	FitnessFunction(bool findMax, unsigned dim) :findMax(findMax), dim(dim) {};
	virtual float operator() ( float* pos) = 0;
	virtual bool is_dim_valid(unsigned)
	{
		return true;
	}
	bool findMax;
	bool dim;
};

class Eggholder_function : public FitnessFunction
{
public:
	Eggholder_function(unsigned dim) :FitnessFunction(FIND_MIN, dim) {}

	float operator() ( float* pos) 
	{
		const float & x = pos[0];
		const float & y = pos[1];
		return -(y + 47)*sin(sqrt(abs(y + x / 2 + 47))) - x*sin(sqrt(abs(x - (y + 47))));
	}

	bool is_dim_valid(unsigned dim)
	{
		return dim == 2;
	}
};

class Sphere_function : public FitnessFunction
{
public:
	Sphere_function(unsigned dim) :FitnessFunction(FIND_MIN, dim) {}

	float operator() ( float* pos) 
	{
		float sum = 0;
		for (int i = 0; i < dim; ++i)
		{
			sum += pos[i] * pos[i];
		}
		return sum;
	}

	bool is_dim_valid(unsigned dim)
	{
		return dim == 2 || dim == 3;
	}
};

class HolderTable_function : public FitnessFunction
{
public:
	HolderTable_function(int) :FitnessFunction(FIND_MIN, 2) {}

	float operator() (float* pos) 
	{
		float x = pos[0], y = pos[1];

		return -abs(sin(x)*cos(y)*exp(abs(1 - sqrt(x*x + y*y) / PI)));
	}

	bool is_dim_valid(unsigned dim)
	{
		return dim == 2;
	}
};

#define CP_MAP_TYPE bool

class Coverage_function :public FitnessFunction
{
public:
	Coverage_function(int p_num, float radius, int scale):
		FitnessFunction(FIND_MAX, p_num*2),
		p_num(p_num),
		radius(radius),
		scale(scale)
	{
		cp_map = (CP_MAP_TYPE **) new CP_MAP_TYPE *[scale];
		for (int i = 0; i < scale; ++i)
		{
			cp_map[i] = new CP_MAP_TYPE[scale];
		}

		resetCP();
	}

	~Coverage_function()
	{
		for (int i = 0; i < scale; ++i)
		{
			delete[] cp_map[i];
		}
		delete cp_map;
	}

	void resetCP()
	{
		for (int i = 0; i < scale; ++i)
		{
			for (int j = 0; j < scale; ++j)
			{
				cp_map[i][j] = 0;
			}
		}
	}

	float operator() (float* pos);

	float dist(int x, int y, int m, int n)
	{
		return sqrt((x - m)*(x - m) + (y - n)*(y - n));
	}

	CP_MAP_TYPE ** getCP()
	{
		return cp_map;
	}

	int p_num;

	float radius;

	int scale;

	CP_MAP_TYPE ** cp_map;
};


class CoverageConfig
{
public:
	int numOfNode;

	float radius;

	int scale;
};

#endif