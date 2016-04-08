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

	virtual bool iterate(bool, int) = 0;

	virtual bool is_OK() = 0;

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

	float getGBValue()
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
