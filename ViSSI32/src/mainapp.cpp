#include "main_window.h"

#include "aco_method.h"
#include <ctime>

Controler * main_controler;
ConfigTable * config_table;

void initAll()
{
	main_controler = new Controler();
	config_table = new ConfigTable();
}


void destroyAll()
{
	delete main_controler;
	delete config_table;
}

void initControler()
{

}


int main(int argc, char **argv)
{
	initAll();
	setWindow(argc, argv);
	destroyAll();
	destroyWindow();

	//srand(time(0));

	//ACOMethod a(20,100);


	//system("pause");
}
