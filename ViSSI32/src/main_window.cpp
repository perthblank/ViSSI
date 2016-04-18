#include "mainh.h"
#include "main_window.h"
#include "control_panel_3d.h"


Fl_Group *group_counti_coverage = (Fl_Group *)0;

Fl_Input *inp_coverage_numOfNode = (Fl_Input *)0;

Fl_Input *inp_coverage_radius = (Fl_Input *)0;

Fl_Input *inp_coverage_Scale = (Fl_Input *)0;

Fl_Double_Window *main_window = (Fl_Double_Window *)0;

Fl_Tabs *tabs_model = (Fl_Tabs *)0;

Fl_Group *group_counti = (Fl_Group *)0;

Fl_Choice *choice_counti_model = (Fl_Choice *)0;

Fl_Group *group_counti_benchmark = (Fl_Group *)0;

Fl_Choice *choice_counti_benchmark_func = (Fl_Choice *)0;

Fl_Group *rgroup_counti_benchmark_dim = (Fl_Group *)0;

Fl_Round_Button *rbtn_counti_benchmark_2d = (Fl_Round_Button *)0;

Fl_Round_Button *rbtn_counti_benchmark_3d = (Fl_Round_Button *)0;

Fl_Group *group_discrete = (Fl_Group *)0;

Fl_Choice *choice_discrete_model = (Fl_Choice *)0;

Fl_Group *group_discrete_seq = (Fl_Group *)0;

Fl_Choice *choice_discrete_seq_func = (Fl_Choice *)0;

Fl_Button *btn_discrete_tspConfig = (Fl_Button *)0;

Fl_Input *inp_discrete_tsp_citynum = (Fl_Input *)0;

Fl_Check_Button *chk_discrete_tsp_randPos = (Fl_Check_Button *)0;

Fl_Group *rgroup_discrete_draw_mode = (Fl_Group *)0;

Fl_Round_Button *rbtn_discrete_tsp_lines = (Fl_Round_Button *)0;

Fl_Round_Button *rbtn_discrete_tsp_lineStrip = (Fl_Round_Button *)0;

Fl_Group *group_adv = (Fl_Group *)0;

Fl_Button *btn_adv_load = (Fl_Button *)0;

Fl_Choice *choice_method = (Fl_Choice *)0;

Fl_Group *group_config_pso = (Fl_Group *)0;

Fl_Input *inp_pso_popSize = (Fl_Input *)0;

Fl_Input *inp_pso_spcScale = (Fl_Input *)0;

Fl_Input *inp_pso_maxT = (Fl_Input *)0;

Fl_Input *inp_pso_wValue = (Fl_Input *)0;

Fl_Input *inp_pso_c1 = (Fl_Input *)0;

Fl_Input *inp_pso_c2 = (Fl_Input *)0;

Fl_Group *rgroup_pso_mode = (Fl_Group *)0;

Fl_Round_Button *rbtn_pso_consFactor = (Fl_Round_Button *)0;

Fl_Round_Button *rbtn_pso_inerWeight = (Fl_Round_Button *)0;

Fl_Button *btn_start = (Fl_Button *)0;

Fl_Button *btn_save = (Fl_Button *)0;

Fl_Button *btn_exit = (Fl_Button *)0;

Fl_Button *btn_about = (Fl_Button *)0;


int strToInt(string s)
{
	stringstream ss(s.c_str());
	int v;
	ss >> v;
	return v;
}

float strToFloat(string s)
{
	stringstream ss(s.c_str());
	float v;
	ss >> v;
	return v;
}

void setOptions(Fl_Choice *c, LList & list)
{
	c->clear();
	for (int i = 0; i < list.size(); i++)
		c->add(list[i].c_str());
}

bool initCountiModelChoice()
{
	setOptions(choice_counti_model, config_table->getModels(CONTINUOUS));
	choice_counti_model->value(0);

	setOptions(choice_discrete_model, config_table->getModels(DISCRETE));
	choice_discrete_model->value(0);

	return true;
}

bool initBenchmarkChoice()
{
	choice_counti_benchmark_func->add(ff_Eggholder);
	choice_counti_benchmark_func->add(ff_Sphere);
	//choice_counti_benchmark_func->add("GoldsteinPrice_function ");
	choice_counti_benchmark_func->value(1);
	return true;
}

bool initMethodChoice(int flag)
{
	switch (flag)
	{
	case CONTINUOUS:
		setOptions(choice_method, config_table->getMethods(choice_counti_model->text()));
		break;

	case DISCRETE:
		setOptions(choice_method, config_table->getMethods(choice_discrete_model->text()));
		break;

	default:
		break;
	}

	choice_method->value(0);

	return true;
}

void setMethodOptions(const char * s)
{
	setOptions(choice_method, config_table->getMethods(s));
	choice_method->value(0);
}

void cb_counti_model(Fl_Choice *c, void*)
{
	if (c->value() == 0)
	{
		group_counti_benchmark->show();
		group_counti_coverage->hide();
	}
	else
	{
		group_counti_benchmark->hide();
		group_counti_coverage->show();
	}

	setMethodOptions(c->text());
}

void cb_model_tabs(Fl_Widget *wt, void *)
{
	Fl_Tabs * tabs = (Fl_Tabs *)wt;

	if (!strcmp(COD_LABELS.Countinuous, ((Fl_Tabs *)tabs->value())->label()))
	{
		initMethodChoice(CONTINUOUS);
	}
	else if (!strcmp(COD_LABELS.Discrete, ((Fl_Tabs *)tabs->value())->label()))
	{
		initMethodChoice(DISCRETE);
	}
	else
	{
		initMethodChoice(0);
	}
}

void initConfig()
{
	inp_pso_popSize->value("1000");
	inp_pso_spcScale->value("512");
	inp_pso_c1->value("0.2");
	inp_pso_c2->value("0.2");
	inp_pso_wValue->value("0.8");
	inp_pso_maxT->value("300");

	inp_coverage_numOfNode->value("20");
	inp_coverage_radius->value("3.5");
	inp_coverage_Scale->value("100");
}


void getConfig(PSOConfig *config)
{
	config->population = strToInt(inp_pso_popSize->value());
	config->pos_scale = strToInt(inp_pso_spcScale->value());
	config->c1 = strToFloat(inp_pso_c1->value());
	config->c2 = strToFloat(inp_pso_c2->value());
	config->wof = (bool)((Fl_Button*)rbtn_pso_inerWeight->value());
	config->wof_value = strToFloat(inp_pso_wValue->value());
	config->max_t = strToInt(inp_pso_maxT->value());

}

void cb_start(Fl_Widget *c, void *)
{
	string method;
	MethodConfig * config = 0;
	string fitness_function;
	unsigned dim = 0;

	method = choice_method->text();
	if (!strcmp(choice_method->text(), METHOD_LABELS.PSO)) {
		config = new PSOConfig;
		getConfig((PSOConfig*)config);
	}

	const char * model_type = ((Fl_Tabs *)tabs_model->value())->label();
	if (!strcmp(COD_LABELS.Countinuous, model_type))
	{
		const char * model = choice_counti_model->text();
		if (!strcmp(model, MODEL_LABELS.Continuous_benchmark))
		{
			fitness_function = choice_counti_benchmark_func->text();
			dim = ((bool)((Fl_Button*)rbtn_counti_benchmark_2d->value())) ? 2 : 3;;
		}
		else
		{
			fitness_function = model;
		}

	}
	else if (!strcmp(COD_LABELS.Discrete, model_type))
	{
	}

	try {
		main_controler->setMethod(method.c_str(), config, fitness_function.c_str(), dim);
	}
	catch (...)
	{
		cout << "parameter error" << endl;
		return;
	}
	main_controler->run();
	main_window->hide();
	setCp3d();
	
}


void destroyWindow()
{
	delete main_window;

	if (cp3d_window != 0)
	{
		delete cp3d_window;
	}
}


int setWindow(int argc, char **argv) {
	{
		main_window = new Fl_Double_Window(611, 612, "VSSI_main");
		{

			Fl_Box* o = new Fl_Box(10, 22, 130, 35, "Problem Model");

			o->labeltype(FL_ENGRAVED_LABEL);

			o->labelfont(1);

		} // Fl_Box* o

		{
			tabs_model = new Fl_Tabs(20, 60, 575, 245);
			tabs_model->selection_color((Fl_Color)4);
			tabs_model->labelcolor(FL_BACKGROUND2_COLOR);
			tabs_model->callback((Fl_Callback*)cb_model_tabs);

			{
				group_counti = new Fl_Group(23, 85, 565, 215, COD_LABELS.Countinuous);
				group_counti->selection_color((Fl_Color)29);

				{
					choice_counti_model = new Fl_Choice(73, 100, 205, 25, "Model:");
					choice_counti_model->down_box(FL_BORDER_BOX);
					choice_counti_model->callback((Fl_Callback*)cb_counti_model);
				} // Fl_Choice* choice_counti_model

				{
					group_counti_coverage = new Fl_Group(38, 140, 495, 150);
					group_counti_coverage->box(FL_EMBOSSED_BOX);

					{
						inp_coverage_numOfNode = new Fl_Input(150, 161, 100, 24, "Num Of Node:");
					} // Fl_Input* inp_coverage_numOfNode

					{
						inp_coverage_radius = new Fl_Input(150, 191, 100, 24, "Radius(m):");
					} // Fl_Input* inp_coverage_radius

					{
						inp_coverage_Scale = new Fl_Input(150, 221, 100, 24, "Area Scale (m):");
					} // Fl_Input* inp_coverage_Scale

					group_counti_coverage->end();
					group_counti_coverage->hide();
				} // Fl_Group* group_counti_coverage

				{
					group_counti_benchmark = new Fl_Group(38, 140, 495, 150);
					group_counti_benchmark->box(FL_EMBOSSED_BOX);

					{
						choice_counti_benchmark_func = new Fl_Choice(113, 155, 226, 25, "Function:");
						choice_counti_benchmark_func->down_box(FL_BORDER_BOX);
						choice_counti_benchmark_func->callback((Fl_Callback*)callback);
						initBenchmarkChoice();
					} // Fl_Choice* choice_counti_benchmark_func

					{
						rgroup_counti_benchmark_dim = new Fl_Group(65, 200, 230, 40);

						{
							rbtn_counti_benchmark_2d = new Fl_Round_Button(93, 205, 64, 20, "2D");
							rbtn_counti_benchmark_2d->type(102);
							rbtn_counti_benchmark_2d->down_box(FL_ROUND_DOWN_BOX);
							rbtn_counti_benchmark_2d->callback((Fl_Callback*)callback);
							rbtn_counti_benchmark_2d->set();
						} // Fl_Round_Button* rbtn_counti_benchmark_2d

						{
							rbtn_counti_benchmark_3d = new Fl_Round_Button(153, 205, 64, 20, "3D");
							rbtn_counti_benchmark_3d->type(102);
							rbtn_counti_benchmark_3d->down_box(FL_ROUND_DOWN_BOX);
							rbtn_counti_benchmark_3d->callback((Fl_Callback*)callback);
						} // Fl_Round_Button* rbtn_counti_benchmark_3d

						rgroup_counti_benchmark_dim->end();
					} // Fl_Group* rgroup_counti_benchmark_dim

					group_counti_benchmark->end();
				} // Fl_Group* group_counti_benchmark

				group_counti->end();

				Fl_Group::current()->resizable(group_counti);

			} // Fl_Group* group_counti

			{
				group_discrete = new Fl_Group(20, 85, 553, 205, COD_LABELS.Discrete);
				group_discrete->selection_color((Fl_Color)29);
				group_discrete->hide();

				{
					choice_discrete_model = new Fl_Choice(73, 100, 205, 25, "Model:");
					choice_discrete_model->down_box(FL_BORDER_BOX);
					choice_discrete_model->callback((Fl_Callback*)callback);
				} // Fl_Choice* choice_discrete_model

				{
					group_discrete_seq = new Fl_Group(38, 140, 495, 150);
					group_discrete_seq->box(FL_EMBOSSED_BOX);

					//{
					//	choice_discrete_seq_func = new Fl_Choice(113, 155, 226, 25, "Function:");
					//	choice_discrete_seq_func->down_box(FL_BORDER_BOX);
					//	choice_discrete_seq_func->callback((Fl_Callback*)callback);
					//} // Fl_Choice* choice_discrete_seq_func

					{
						btn_discrete_tspConfig = new Fl_Button(180, 223, 158, 27, "Configure Positions...");
						btn_discrete_tspConfig->callback((Fl_Callback*)callback);
					} // Fl_Button* btn_discrete_tspConfig

					{
						inp_discrete_tsp_citynum = new Fl_Input(113, 189, 85, 25, "City Num:");
					} // Fl_Input* inp_discrete_tsp_citynum

					{
						chk_discrete_tsp_randPos = new Fl_Check_Button(50, 225, 150, 25, "random position");
						chk_discrete_tsp_randPos->down_box(FL_DOWN_BOX);
					} // Fl_Check_Button* chk_discrete_tsp_randPos

					group_discrete_seq->end();

				} // Fl_Group* group_discrete_seq

				{
					rgroup_discrete_draw_mode = new Fl_Group(50, 255, 260, 31);
					rgroup_discrete_draw_mode->box(FL_THIN_UP_FRAME);

					{
						rbtn_discrete_tsp_lines = new Fl_Round_Button(55, 258, 105, 26, "draw lines");
						rbtn_discrete_tsp_lines->type(102);
						rbtn_discrete_tsp_lines->down_box(FL_ROUND_DOWN_BOX);
						rbtn_discrete_tsp_lines->callback((Fl_Callback*)callback);
					} // Fl_Round_Button* rbtn_discrete_tsp_lines

					{
						rbtn_discrete_tsp_lineStrip = new Fl_Round_Button(165, 259, 125, 25, "draw line-stripe");
						rbtn_discrete_tsp_lineStrip->type(102);
						rbtn_discrete_tsp_lineStrip->down_box(FL_ROUND_DOWN_BOX);
						rbtn_discrete_tsp_lineStrip->callback((Fl_Callback*)callback);
					} // Fl_Round_Button* rbtn_discrete_tsp_lineStrip

					rgroup_discrete_draw_mode->end();
				} // Fl_Group* rgroup_discrete_draw_mode

				group_discrete->end();
			} // Fl_Group* group_discrete

			{
				group_adv = new Fl_Group(20, 85, 570, 220, "Advance");
				group_adv->selection_color((Fl_Color)29);
				group_adv->hide();

				{
					Fl_Scroll* o = new Fl_Scroll(35, 110, 450, 190, "Problem Description");
					o->box(FL_SHADOW_BOX);
					o->end();
				} // Fl_Scroll* o

				{
					btn_adv_load = new Fl_Button(495, 265, 90, 30, "Load...");
					btn_adv_load->callback((Fl_Callback*)callback);
				} // Fl_Button* btn_adv_load

				group_adv->end();

			} // Fl_Group* group_adv

			tabs_model->end();
			Fl_Group::current()->resizable(tabs_model);
		} // Fl_Tabs* tabs_model

		{
			Fl_Box* o = new Fl_Box(17, 325, 210, 35, "Swarm Intelligence Method");
			o->labeltype(FL_ENGRAVED_LABEL);
			o->labelfont(1);
		} // Fl_Box* o

		{
			choice_method = new Fl_Choice(85, 359, 185, 24, "method:");
			choice_method->down_box(FL_BORDER_BOX);
			choice_method->callback((Fl_Callback*)callback);
		} // Fl_Choice* choice_method

		{
			group_config_pso = new Fl_Group(30, 390, 560, 140);
			group_config_pso->box(FL_EMBOSSED_BOX);
			group_config_pso->labeltype(FL_SHADOW_LABEL);

			{
				inp_pso_popSize = new Fl_Input(150, 401, 160, 25, "Population Size:");
			} // Fl_Input* inp_pso_popSize

			{
				inp_pso_spcScale = new Fl_Input(150, 432, 160, 25, "Space Sacle:");
			} // Fl_Input* inp_pso_spcScale

			{
				inp_pso_maxT = new Fl_Input(150, 465, 160, 25, "Max Iterations:");
			}

			{
				inp_pso_wValue = new Fl_Input(400, 440, 115, 25, "w/f:");
			} // Fl_Input* inp_pso_wValue

			{
				inp_pso_c1 = new Fl_Input(400, 460, 115, 25, "c1:");
			} // Fl_Input* inp_pso_c1

			{
				inp_pso_c2 = new Fl_Input(400, 484, 115, 25, "c2:");
			} // Fl_Input* inp_pso_c2

			{
				rgroup_pso_mode = new Fl_Group(375, 390, 170, 50);
				rgroup_pso_mode->box(FL_THIN_UP_FRAME);

				{
					rbtn_pso_consFactor = new Fl_Round_Button(385, 390, 155, 30, "constriciton factor");
					rbtn_pso_consFactor->type(102);
					rbtn_pso_consFactor->down_box(FL_ROUND_DOWN_BOX);
					rbtn_pso_consFactor->callback((Fl_Callback*)callback);
					rbtn_pso_consFactor->set();
				} // Fl_Round_Button* rbtn_pso_consFactor

				{
					rbtn_pso_inerWeight = new Fl_Round_Button(385, 410, 155, 25, "inertia weight");
					rbtn_pso_inerWeight->type(102);
					rbtn_pso_inerWeight->down_box(FL_ROUND_DOWN_BOX);
					rbtn_pso_inerWeight->callback((Fl_Callback*)callback);
				} // Fl_Round_Button* rbtn_pso_inerWeight

				rgroup_pso_mode->end();
			} // Fl_Group* rgroup_pso_mode

			group_config_pso->end();
		} // Fl_Group* group_config_pso

		{
			btn_start = new Fl_Button(30, 560, 80, 30, "Start");
			btn_start->callback((Fl_Callback*)cb_start);
		} // Fl_Button* btn_start

		{
			btn_save = new Fl_Button(120, 560, 80, 30, "Save");
			btn_save->callback((Fl_Callback*)callback);
		} // Fl_Button* btn_save

		{
			btn_exit = new Fl_Button(510, 560, 80, 30, "Exit");
			btn_exit->callback((Fl_Callback*)callback);
		} // Fl_Button* btn_exit

		{
			btn_about = new Fl_Button(505, 10, 85, 30, "About");
		} // Fl_Button* btn_about
		main_window->end();

	} // Fl_Double_Window* main_window


	initCountiModelChoice();

	initMethodChoice(CONTINUOUS);

	initConfig();

	main_window->show(argc, argv);

	return Fl::run();
}

void callback(Fl_Widget* o, void*)
{
	printf("Default callback\n");
}


