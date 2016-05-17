#include "mainh.h"
#include "pso_method.h"


PSOMethod::PSOMethod(PSOConfig* config,
	FitnessFunction* fitness_f
	) :
	SIMethod(config->max_t, fitness_f),
	population(config->population), dim(config->dim), pos_scale(config->pos_scale)
{
	is_ok = false;
	if (!fitness_f || !fitness_f->is_dim_valid(dim)) return;

	msize = population*dim;
	position_a = new float[msize];
	velocity_a = new float[msize];
	pbval_a = new float[population];
	pcval_a = new float[population];
	pbpos_a = new float[msize];

	gb_pos = new float[dim];

	omega = config->wof_value;
	c1 = config->c1;
	c2 = config->c2;

	vel_scale = pos_scale / 200;

	init_pos();
	assign(loc(position_a, 0), gb_pos);
	gb_val = pbval_a[0];

	is_ok = true;
}

PSOMethod::~PSOMethod()
{
	delete[] position_a;
	delete[] velocity_a;
	delete[] pbpos_a;
	delete[] pbval_a;
	delete[] pcval_a;
	delete[] gb_pos;
	delete fitness_f;
}

void PSOMethod::iterate_do()
{
	for (unsigned pi = 0; pi < population; ++pi) 
	{
	//	cout << "particle " << pi << "----------------" << endl;
		update_velocity(pi);
		update_position(pi);
		evaluate_fitness(pi);
		
	}
	//cout << endl << endl;
}

void PSOMethod::save_gbest(const char* name)
{
	fstream out(name, ios::out);
	out << name << endl;
	out << "Current Global Best Value: " << gb_val <<endl;
	out << "Current Global Best Position:" << endl;
	for (int i = 0; i < dim; ++i)
		out << gb_pos[i] << endl;

	out.close();

	//cout << "saved";
}

inline float * PSOMethod::loc(float * arr, const unsigned& id)
{
	return arr + id*dim;
}

inline void PSOMethod::assign(float * a, float * b)
{
	//assign a to b
	for (size_t i = 0; i < dim; i++)
	{
		b[i] = a[i];
	}
}

inline float PSOMethod::m_rand()
{
	return rand() % 1000 * 0.001;
}

inline float PSOMethod::m_eval(float value, float limit)
{
	if (value > limit) value = limit;
	else if (value < -limit) value = -limit;
	return value;
}

inline bool PSOMethod::better(float v1, float v2) 
{
	// if v1 is better than v2
	if (fitness_f->findMax)
		return v1>v2;
	else
		return v1<v2;
}

void PSOMethod::init_pos()
{
	for (unsigned i = 0; i < population; ++i)
	{
		float * pos = loc(position_a, i);
		float * vel = loc(velocity_a, i);
		float * pb = loc(pbpos_a, i);
		for (unsigned d = 0; d < dim; ++d) 
		{
			pos[d] = m_rand()*pos_scale;
			pb[d] = pos[d];
			vel[d] = m_rand()*vel_scale;
		}
		pbval_a[i] = (*fitness_f)(pos);
		pcval_a[i] = pbval_a[i];
	}

}

void PSOMethod::update_velocity(const unsigned& pi)
{
	float * pos = loc(position_a, pi);
	float * vel = loc(velocity_a, pi);
	float * pb = loc(pbpos_a, pi);
	
	for (size_t d = 0; d<dim; ++d) 
	{
		float r1 = m_rand();
		float r2 = m_rand();

		float tmp =
			omega * vel[d] +
			c1*r1*(pb[d] - pos[d]) +
			c2*r2*(gb_pos[d] - pos[d]);

		vel[d] = m_eval(tmp, vel_scale);	
	}

	//cout << "Velocity: ";
	//Util::print(vel, dim);
	//printf("omega %f\n", omega);
}

void PSOMethod::update_position(const unsigned& pi)
{
	float * pos = loc(position_a, pi);
	float * vel = loc(velocity_a, pi);
	for (unsigned d = 0; d<dim; ++d)
	{
		pos[d] = m_eval(pos[d] + vel[d], pos_scale);
	}
}

void PSOMethod::evaluate_fitness(const unsigned& pi)
{
	float * pos = loc(position_a, pi);
	float fitness = (*fitness_f)(pos);

	pcval_a[pi] = fitness;
	//printf("fitness: %f \n", fitness);

	//cout << "gb_val " << gb_val << endl;

	if (better(fitness, pbval_a[pi])) 
	{
		pbval_a[pi] = fitness;
		assign(pos, loc(pbpos_a, pi));
	}

	if (better(fitness, gb_val)) 
	{
		gb_val = fitness;
		assign(pos, gb_pos);
		//cout << "update gb--------------------------------------" << endl;
	}
}

void PSOMethod::printgb()
{
	for (size_t i = 0; i < dim; i++)
		cout << gb_pos[i] << " ";
	cout << endl;
}

float Coverage_function::operator() (float* pos)
{
	resetCP();
	int sum = 0;
	
	for (int i = 0; i < p_num * 2 - 1; i += 2)
	{
		float &x = pos[i];
		float &y = pos[i + 1];
		//printf("node(%d,%d): ",x, y);
		int count = 0;
		for (int m = ceil(x - radius); m <= floor(x + radius); ++m)
		{
			for (int n = ceil(y - radius); n <= floor(y + radius); ++n)
			{

				if (m >= 0 && m < scale && n >= 0 && n < scale)
				{

					if (dist(m, n, x, y) <= radius)
					{

						if (cp_map[m][n] == 0)
						{
							cp_map[m][n] = 1;
							//printf("(%d, %d) ",m,n);
							++count;
							//++sum;
						}
					}
				}
			}
		}
		//if (count <= 2)
		//{
		//	int factor =
		//		//ceil(radius);
		//		2;

		//	x += rand() % (int)(4 * factor + 1) - 2 * factor ;
		//	y += rand() % (int)(4 * factor + 1) - 2 * factor;

		//}

		//else 
			if (count <= 5)
		{
			int factor =
				//ceil(radius);
				2;

			x += rand() % (int)(2 * factor + 1) - 1 * factor;
			y += rand() % (int)(2 * factor + 1) - 1 * factor;

		}
		sum += count;
	}
	//cout << endl;
	return sum;
}