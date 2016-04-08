#include "controler.h"

Controler::Controler()
{
	sip = NULL;
	fitness_f = NULL;
	is_ok = make() == GOOD;
	iter_gap = 270;
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
	else if (!strcmp(fitness_function, ff_GoldsteinPrice))
	{

	}
	
	else if (!strcmp(fitness_function, ff_Coverage))
	{
		int node_num = 10;
		int scale = 50;
		fitness_f = new Coverage_function(node_num, 3.5, scale);
		draw_type = DRAR_COVERAGE;
		pconfig->dim = node_num * 2;
		pconfig->pos_scale = scale;
	}

	if (!strcmp(method, "PSO")) 
	{	
		sip = new PSOMethod(pconfig, fitness_f);
		if (!sip->is_OK())
			throw;
		population = pconfig->population;
		pos_scale = pconfig->pos_scale;
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