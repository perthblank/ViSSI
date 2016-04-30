#ifndef CONTROLER_H
#define CONTROLER_H

#include <cstring>
#include <Windows.h>
#include <process.h>
#include <iostream>

using namespace std;

#include "pso_method.h"
#include "aco_method.h"
#include "aco_path.h"
#include "renderer.h"
#include "vssi_util.h"

extern struct METHOD_LABELSCLASS METHOD_LABELS;
extern struct COD_LABELSCLASS COD_LABELS;
extern struct MODEL_LABELSCLASS MODEL_LABELS;

struct METHOD_LABELSCLASS
{
	const char * PSO = "PSO";
	const char * PSO_DESCRETE = "PSO_discrete";
	const char * ACO = "ACO";
};

struct COD_LABELSCLASS
{
	const char * Countinuous = "Continuous";
	const char * Discrete = "Discrete";
	const char * Advance = "Advance";
};

struct MODEL_LABELSCLASS
{
	const char * Continuous_benchmark = "Continuous_Benchmark";
	const char * Continuous_coverage = ff_Coverage;
};

#define GOOD 0
#define ERR_SI 1
#define ERR_RENDER 2

#define MOVE_X 1
#define MOVE_Y 2

class Controler {
public:
	Controler();

	Controler(int ID);

	~Controler();

	bool is_OK();

	void runSI();

	void runRender();

	void toggleSI();

	void crossSI(int);

	void run();

	int make();

	void setMethod(const char *, MethodConfig *, const char *, unsigned);

	void setSpeed(float);

	float getProcessPrecent();

	float getFitnessValue();

	void destroyCurrent();

	void moveView(int op, float v);

private:

	SIMethod * sip;

	Renderer * rderp;

	HANDLE si_h, rder_h, window_h;

	bool is_ok;

	ThreadProc<Controler> si_proc, rder_proc, window_proc, thread_proc;

	unsigned population, dim;

	FitnessFunction * fitness_f;

	float pos_scale;

	int draw_type;

	unsigned int __stdcall WindowThread();

	unsigned int __stdcall RenderThread();

	unsigned int __stdcall SIThread();

	unsigned int __stdcall SIToggle();

	unsigned int __stdcall SICross(int t);

	unsigned iter_gap, iter_gap_default = 270;
};

#endif
