#ifndef RENDER_CONTROL_H
#define RENDER_CONTROL_H

#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GLFW/glfw3.h>

struct InputFactorClass
{
	double oldmx = 0;

	double oldmy = 0;

	bool flag_ok = false;

	double degx = 0, degy = 0;

	double h = 1.0;

	double f_a2r = 0.0174;

	double radius = 1.0;

	void refreshView()
	{
		oldmx = 0;
		oldmy = 0;
		flag_ok = false;
		degx = 0;
		degy = 0;
		h = 1.0;
		f_a2r = 0.0174;
		radius = 1.0;
	}

};

extern InputFactorClass InputFactor;

void mouseClick(GLFWwindow * window, int id, int action, int mods);

void mouseMove(GLFWwindow * window, double x, double y);

void adjustView(double x, double y);

void adjustView(double, double, double);

void mouseScroll(GLFWwindow* window, double xoffset, double yoffset);

#endif