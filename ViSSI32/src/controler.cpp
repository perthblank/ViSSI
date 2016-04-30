#include "controler.h"

struct METHOD_LABELSCLASS METHOD_LABELS;
struct COD_LABELSCLASS COD_LABELS;
struct MODEL_LABELSCLASS MODEL_LABELS;

Controler::Controler()
{
	sip = NULL;
	fitness_f = NULL;
	is_ok = make() == GOOD;
	iter_gap = iter_gap_default;
}

Controler::Controler(int ID)
{
	Controler();
}

Controler::~Controler()
{
	delete sip;
}

bool Controler::is_OK()
{
	return is_ok;
}

unsigned int __stdcall Controler::WindowThread()
{
	return 0;
}

unsigned int __stdcall Controler::RenderThread()
{
	rderp = new Renderer(population, pos_scale, sip, draw_type);
	if (!rderp->is_OK()) return ERR_RENDER;
	rderp->doRender();
	return 0;
}

unsigned int __stdcall Controler::SIThread()
{
	while (sip->iterate(false,1)) 
	{
		Sleep(iter_gap);
	}
	return 0;
}

unsigned int __stdcall Controler::SICross(int t)
{
	sip->iterate(true, t);
	return 0;
}

unsigned int __stdcall Controler::SIToggle()
{
	sip->togglePlay();
	return 0;
}

void Controler::runSI()
{
	thread_proc.Member = &Controler::SIThread;
	si_h =
		(HANDLE)_beginthreadex(
			NULL, 0,
			(_beginthreadex_proc_type)thread_proc.Proc,
			(LPVOID)this,
			0, NULL);
}

void Controler::toggleSI()
{
	thread_proc.Member = &Controler::SIToggle;
	(HANDLE)_beginthreadex(
		NULL, 0,
		(_beginthreadex_proc_type)thread_proc.Proc,
		(LPVOID)this,
		0, NULL);
}

void Controler::crossSI(int t)
{
	thread_proc.Member = &Controler::SIToggle;
	(HANDLE)_beginthreadex(
		NULL, 0,
		(_beginthreadex_proc_type)thread_proc.Proc,
		(LPVOID)this,
		0, NULL);
}

void Controler::runRender()
{
	rder_h =
		(HANDLE)_beginthreadex(
			NULL, 0,
			(_beginthreadex_proc_type)rder_proc.Proc,
			(LPVOID)this,
			0, NULL);
}

void Controler::run()
{
	if (!is_OK())
		return;

	rder_proc.Member = &Controler::RenderThread;
	si_proc.Member = &Controler::SIThread;
	window_proc.Member = &Controler::WindowThread;

	runRender();
	runSI();

}

int Controler::make()
{
	return GOOD;
}


void Controler::setMethod(
	const char * method, 
	MethodConfig * config, 
	const char * fitness_function, 
	unsigned dim
	)
{
	
	if (sip != NULL)
	{
		delete sip;
	}
	if (fitness_f != NULL)
	{
		delete fitness_f;
	}

	if (!strcmp(method, METHOD_LABELS.PSO))
	{

		PSOConfig * pconfig = (PSOConfig*)config;

		pconfig->dim = dim;

		if (!strcmp(fitness_function, ff_Eggholder)) 
		{
			fitness_f = new Eggholder_function(dim);
			if (dim == 2)
				draw_type = DRAW_2D_XZ;
			else
				draw_type = DRAW_3D;
			pconfig->pos_scale = 512.0;
		}
		else if (!strcmp(fitness_function, ff_Sphere))
		{
			fitness_f = new Sphere_function(dim);
			if (dim == 2)
				draw_type = DRAW_2D_XZ;
			else
				draw_type = DRAW_3D;
			pconfig->pos_scale = 2.0;
		}
		else if (!strcmp(fitness_function, ff_HolderTable))
		{
			fitness_f = new Sphere_function(dim);
			draw_type = DRAW_2D_XZ;
			pconfig->pos_scale = 10.0;
		}
		else if (!strcmp(fitness_function, ff_Coverage))
		{
			CoverageConfig * cconfig = (CoverageConfig *)pconfig->ptr;

			fitness_f = new Coverage_function(cconfig->numOfNode, cconfig->radius, cconfig->scale);
			draw_type = DRAW_COVERAGE;
			pconfig->dim = cconfig->numOfNode * 2;
			pconfig->pos_scale = cconfig->scale;
		}
		

		sip = new PSOMethod(pconfig, fitness_f);
		if (!sip->is_OK())
			throw 1;
		population = pconfig->population;
		pos_scale = pconfig->pos_scale;
	}

	else if (!strcmp(method, METHOD_LABELS.ACO))
	{

		if (!strcmp(fitness_function, ff_TSP))
		{
			draw_type = DRAW_TSP;
			ACOConfig * aconfig = (ACOConfig*)config;
			sip = new ACOMethod(aconfig);
			pos_scale = 100;
			population = 20;
		}
		else if (!strcmp(fitness_function, ff_PATH))
		{
			sip = new ACO_PathPlan(10,1,30);
			draw_type = DRAW_PATHP;
			pos_scale = 10;
			population = 5;
		}

	}

}

void  Controler::setSpeed(float i)
{
	iter_gap = 301 - 300 * i;
}

float Controler::getProcessPrecent()
{
	return sip->getProcessPrecent();
}

float Controler::getFitnessValue()
{
	return sip->getGBValue();
}

void Controler::destroyCurrent()
{
	sip->kill();
	rderp->kill();

	WaitForSingleObject(rder_h, INFINITE);
	WaitForSingleObject(si_h, INFINITE);

	delete sip;
	delete rderp;
	sip = 0;
	rderp = 0;
	iter_gap = iter_gap_default;
}

void Controler::moveView(int op, float diff)
{
	float move;
	switch (op)
	{
	case MOVE_X:
		move = diff*50;
		adjustView(move, 0);
		break;

	case MOVE_Y:
		move = diff * 30;
		adjustView(0, move);
		break;
	}
}