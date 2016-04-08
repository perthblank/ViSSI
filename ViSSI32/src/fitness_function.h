#ifndef FITNESS_FUNCTION_H
#define FITNESS_FUNCTION_H

#include<cmath>

#define FIND_MAX true
#define FIND_MIN false

#define ff_Eggholder "Eggholder_function"
#define ff_Sphere "Sphere_function"
#define ff_GoldsteinPrice "GoldsteinPrice_function"
#define ff_Coverage "Continuous_Covery"


class FitnessFunction {
public:
	FitnessFunction(bool findMax, unsigned dim) :findMax(findMax), dim(dim) {};
	virtual float operator() (const float* pos) = 0;
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

	float operator() (const float* pos) 
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

	float operator() (const float* pos) 
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

class GoldsteinPrice_function : public FitnessFunction
{
public:
	GoldsteinPrice_function() :FitnessFunction(FIND_MIN, 2) {}

	float operator() (const float* pos) 
	{
		float x = pos[0], y = pos[1];

		float sum = 0;

		return sum;
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

	float operator() (const float* pos) 
	{
		resetCP();
		int sum = 0;
	
		for (int i = 0; i < p_num; i+=2)
		{
			const int &x = pos[i];
			const int &y = pos[i + 1];
			for (int m = ceil(x - radius); m < floor(x + radius); ++m)
			{
				for (int n = ceil(y - radius); n < floor(y + radius); ++n)
				{
					if (m > 0 && m < scale &&n>0 && n < scale)
					{
						if (dist(m, n, x, y) <= radius)
						{
							if (cp_map[m][n] == 0)
							{
								cp_map[m][n] == 1;
								++sum;
							}
						}
					}
				}
			}

		}

		return sum;
	}

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


#endif
