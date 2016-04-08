#ifndef PSO_METHOD_H
#define PSO_METHOD_H

#include "si_method.h"

class PSOConfig : public MethodConfig
{
public:
	unsigned population;

	unsigned dim;

	float pos_scale;

	msize_t max_t;

	bool wof;

	float wof_value;

	float c1, c2;

};

class PSOMethod:public SIMethod 
{
public:
	PSOMethod(unsigned population, unsigned dim, float pos_scale,
		msize_t max_t,
		FitnessFunction* fitness_f
		);

	PSOMethod(PSOConfig* config,
		FitnessFunction* fitness_f
		);

	~PSOMethod();

	virtual bool iterate(bool force, int t);

	bool is_OK();

private:

	float * velocity_a;

	float * pbpos_a;

	float * pbval_a;

	msize_t msize;

	float pos_scale;

	float vel_scale;

	unsigned population;

	unsigned dim;

	float c1, c2, omega;

	inline float * loc(float * arr, const unsigned& id);

	inline void assign(float * a, float * b);

	inline float m_rand();

	inline float m_eval(float value, float limit);

	inline bool better(float v1, float v2);

	void init_pos();

	void update_velocity(const unsigned& pi);

	void update_position(const unsigned& pi);

	void evaluate_fitness(const unsigned& pi);

	void printgb();

};

#endif
