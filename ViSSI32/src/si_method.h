#ifndef SI_METHOD_H
#define SI_METHOD_H

#include "fitness_function.h"

typedef short msize_t;

class SIMethod {
public:
	SIMethod(int max_t, FitnessFunction* fitness_f) :
		max_t(max_t),
		fitness_f(fitness_f),
		current_t(0), 
		is_running(false), 
		alive(true){}

	virtual bool iterate(bool force, int t)
	{

		if (!alive)
		{
			return false;
		}

		if (!force)
		{
			if (!is_running) return true;
		}

		for (int i = 0; i < t; ++i)
		{
			if (current_t++ > max_t) return false;
			iterate_do();
		}

		return true;

	};

	virtual void iterate_do() = 0;

	virtual bool is_OK()
	{
		return is_ok;
	}

	float * getPosArr() 
	{
		return position_a;
	}

	float * getCurValueArr()
	{
		return pcval_a;
	}

	float * getGBPosArr()
	{
		return gb_pos;
	}

	void togglePlay()
	{
		is_running = !is_running;
		//cout << "toogle" << endl;
	}

	float getProcessPrecent()
	{
		return (float)current_t / max_t;
	}

	void kill()
	{
		alive = false;
	}

	FitnessFunction * getFitnessFunction()
	{
		return fitness_f;
	}

	virtual float getGBValue()
	{
		return gb_val;
	}

protected:
	msize_t current_t, max_t;

	float * position_a, * pcval_a, * gb_pos;

	float gb_val;

	bool is_ok;

	bool is_running;

	bool alive;

	FitnessFunction * fitness_f;
};


class MethodConfig
{

};


#endif
