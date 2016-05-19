#include "render_control.h"


InputFactorClass InputFactor;

void mouseClick(GLFWwindow * window, int id, int action, int mods) {
	if (action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &InputFactor.oldmx, &InputFactor.oldmy);
		InputFactor.flag_ok = true;
	}
	else
	{
		InputFactor.flag_ok = false;
	}
}

void mouseMove(GLFWwindow * window, double x, double y)
{
	if (!InputFactor.flag_ok) return;
	InputFactor.degx += x - InputFactor.oldmx;
	InputFactor.degy += y - InputFactor.oldmy;
	InputFactor.h += 0.1f * (y - InputFactor.oldmy);
	if (InputFactor.h>1.5) InputFactor.h = 1.5;
	else if (InputFactor.h<-1.5) InputFactor.h = -1.5;
	InputFactor.oldmx = x;
	InputFactor.oldmy = y;
}

void adjustView(double x, double y)
{
	InputFactor.degx += x;
	InputFactor.degy += y;
	InputFactor.h += 0.1f*(y);
	
	//if (InputFactor.h>1.5) InputFactor.h = 1.5;
	//else if (InputFactor.h<-1.5) InputFactor.h = -1.5;

	InputFactor.oldmx += x;
	InputFactor.oldmy += y;
}

void adjustView(double, double, double r)
{
	InputFactor.radius += r;
}

void mouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	InputFactor.radius += yoffset * InputFactor.incr;
	//cout << InputFactor.radius << endl;
}

