#include "mainh.h"
#include "main_window.h"
#include "control_panel_3d.h"
#include "alert.h"


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

Fl_Group *group_discrete_path = (Fl_Group *)0;

Fl_Input *inp_path_scale = (Fl_Input *)0;

Fl_Choice *choice_discrete_seq_func = (Fl_Choice *)0;

Fl_Button *btn_discrete_tspConfig = (Fl_Button *)0;

Fl_Input *inp_discrete_tsp_citynum = (Fl_Input *)0;

Fl_Check_Button *chk_discrete_tsp_randPos = (Fl_Check_Button *)0;

Fl_Group *rgroup_discrete_draw_mode = (Fl_Group *)0;

Fl_Round_Button *rbtn_discrete_tsp_lines = (Fl_Round_Button *)0;

Fl_Round_Button *rbtn_discrete_tsp_lineStrip = (Fl_Round_Button *)0;

Fl_Group *group_adv = (Fl_Group *)0;

Fl_Button *btn_adv_load = (Fl_Button *)0;

Fl_Group *group_config_si = 0;

Fl_Choice *choice_method = (Fl_Choice *)0;

Fl_Group *group_config_pso = (Fl_Group *)0;

Fl_Group *group_config_aco = (Fl_Group *)0;

Fl_Group *group_config_pso_discrete = (Fl_Group *)0;

Fl_Input *inp_pso_popSize = (Fl_Input *)0;

Fl_Input *inp_pso_spcScale = (Fl_Input *)0;

Fl_Input *inp_pso_maxT = (Fl_Input *)0;

Fl_Input *inp_pso_wValue = (Fl_Input *)0;

Fl_Input *inp_pso_c1 = (Fl_Input *)0;

Fl_Input *inp_pso_c2 = (Fl_Input *)0;

Fl_Input *inp_aco_popSize = (Fl_Input *)0;

Fl_Input *inp_aco_maxT = (Fl_Input *)0;

Fl_Input *inp_aco_T0 = (Fl_Input *)0;

Fl_Input *inp_aco_P0 = (Fl_Input *)0;

Fl_Group *rgroup_pso_mode = (Fl_Group *)0;

Fl_Round_Button *rbtn_pso_consFactor = (Fl_Round_Button *)0;

Fl_Round_Button *rbtn_pso_inerWeight = (Fl_Round_Button *)0;

Fl_Button *btn_start = (Fl_Button *)0;

Fl_Button *btn_save = (Fl_Button *)0;

Fl_Button *btn_exit = (Fl_Button *)0;

Fl_Button *btn_about = (Fl_Button *)0;

Fl_File_Chooser *fc_tsp_file = 0;

Fl_Input *inp_tsp_file = 0;

Fl_Text_Display *text_adv_config;

Fl_Text_Buffer *buffer_adv_config;

Fl_Input *inp_adv_config_file;

map<string, int> map_counti_benchmark_index;

#define LABEL_ViSSI_MAIN "ViSSI"

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
	int i = 0;
	choice_counti_benchmark_func->add(ff_Eggholder);
	map_counti_benchmark_index[ff_Eggholder] = i++;
	choice_counti_benchmark_func->add(ff_Sphere);
	map_counti_benchmark_index[ff_Sphere] = i++;
	choice_counti_benchmark_func->add(ff_HolderTable);
	map_counti_benchmark_index[ff_HolderTable] = i++;

	choice_counti_benchmark_func->value(1);
	return true;
}

bool initMethodChoice(int flag)
{
	group_config_si->show();
	group_adv->hide();
	tabs_model->resize(20, 60, 575, 245);
	switch (flag)
	{
	case CONTINUOUS:
		setOptions(choice_method, config_table->getMethods(choice_counti_model->text()));
		group_config_aco->hide();
		group_config_pso->show();
		group_config_pso_discrete->hide();
		break;

	case DISCRETE:
		setOptions(choice_method, config_table->getMethods(choice_discrete_model->text()));
		group_config_aco->show();
		group_config_pso->hide();
		group_config_pso_discrete->hide();
		break;

	default:
		group_config_si->hide();
		group_adv->show();
		tabs_model->resize(20, 60, 575, 450);
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

void cb_discrete_model(Fl_Choice *c, void*)
{
	if (c->value() == 0)
	{
		group_discrete_seq->show();
		group_discrete_path->hide();
	}
	else
	{
		group_discrete_seq->hide();
		group_discrete_path->show();
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

void loadConfigFile()
{
	const char * filename = inp_adv_config_file->value();
	if (!strcmp(filename,""))
		return;

	Json::Reader reader;
	Json::Value root;
	std::ifstream is;
	is.open(filename, std::ios::binary);

	if (reader.parse(is, root))
	{
		string mode = root["mode"].asString();
		string model = root["model"].asString();
		int method_ind = root["method"].asInt();
		if (!strcmp(mode.c_str(), COD_LABELS.Countinuous))
		{
			group_adv->hide();
			group_counti->show();
			initMethodChoice(CONTINUOUS);

			if (! config_table->isValidConfig(model, 1, method_ind))
			{
				throw 1;
			}
			if (!strcmp(model.c_str(), MODEL_LABELS.Continuous_benchmark))
			{
				choice_counti_model->value(0);
				cout << map_counti_benchmark_index[root["model_config"]["function"].asString()] << endl;
				choice_counti_benchmark_func->value(
					map_counti_benchmark_index[root["model_config"]["function"].asString()]
					);

				rbtn_counti_benchmark_2d->value(0);
				rbtn_counti_benchmark_3d->value(0);
				if (root["model_config"]["dim"].asInt() == 2)
					rbtn_counti_benchmark_2d->set();
				else
					rbtn_counti_benchmark_3d->set();
			}
		}
		else if (!strcmp(mode.c_str(), COD_LABELS.Discrete))
		{
			group_adv->hide();
			group_discrete->show();
			initMethodChoice(DISCRETE);
		}
		else
		{
			throw 1;
		}

		string method = config_table->method_map[method_ind];
		if (!strcmp(method.c_str(), METHOD_LABELS.PSO))
		{
			inp_pso_popSize->value(root["method_config"]["population"].asString().c_str());
			inp_pso_spcScale->value(root["method_config"]["scale"].asString().c_str());
			inp_pso_c1->value(root["method_config"]["c1"].asString().c_str());
			inp_pso_c2->value(root["method_config"]["c2"].asString().c_str());
			inp_pso_wValue->value(root["method_config"]["wof_value"].asString().c_str());
			inp_pso_maxT->value(root["method_config"]["max_t"].asString().c_str());

			rbtn_pso_consFactor->value(0);
			rbtn_pso_inerWeight->value(0);
			if (root["method_config"]["wof"] == 1)
				rbtn_pso_inerWeight->value(1);
			else
				rbtn_pso_consFactor->value(1);

		}
	}
	else
	{
		cout << "cannot parse file " << filename << endl;
		//alertMsg("Config File Not Valid!");
		throw 1;
	}
	is.close();
}

void initConfig()
{
	inp_pso_popSize->value("10");
	inp_pso_spcScale->value("512");
	inp_pso_c1->value("0.2");
	inp_pso_c2->value("0.2");
	inp_pso_wValue->value("0.8");
	inp_pso_maxT->value("300");

	inp_coverage_numOfNode->value("30");
	inp_coverage_radius->value("1.5");
	inp_coverage_Scale->value("20");

	inp_aco_popSize->value("5");
	inp_aco_maxT->value("100");
	inp_aco_T0->value("190");
	inp_aco_P0->value("0.7");

	inp_path_scale->value("100");
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

void getConfig(ACOConfig *config)
{
	config->population = strToInt(inp_aco_popSize->value());
	config->max_t = strToInt(inp_aco_maxT->value());
	config->P0 = strToFloat(inp_aco_P0->value());
	config->T0 = strToFloat(inp_aco_T0->value());
	config->value = strToInt(inp_path_scale->value());
}

void getConfig(CoverageConfig * config)
{
	config->numOfNode = strToInt(inp_coverage_numOfNode->value());
	config->scale = strToFloat(inp_coverage_Scale->value());
	config->radius = strToFloat(inp_coverage_radius->value());
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
	else if (!strcmp(choice_method->text(), METHOD_LABELS.ACO)) {
		//if (!inp_tsp_file->value())
		//	return;
		config = new ACOConfig(inp_tsp_file->value());
		getConfig((ACOConfig*)config);
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

			if (!strcmp(model, MODEL_LABELS.Continuous_coverage))
			{
				((PSOConfig*)config)->ptr = (CoverageConfig *) new CoverageConfig;
				getConfig((CoverageConfig *)((PSOConfig*)config)->ptr);
			}
		}

	}
	else if (!strcmp(COD_LABELS.Discrete, model_type))
	{
		fitness_function = choice_discrete_model->text();
		//if(!strcmp(fitness_function, ))
	}
	else
	{
		loadConfigFile();
		return;
	}

	try
	{
		main_controler->setMethod(method.c_str(), config, fitness_function.c_str(), dim);
	}
	catch (...)
	{
		//cout << "parameter error" << endl;
		alertMsg("Parameter Error");
		return;
	}
	main_controler->run();
	main_window->hide();
	setCp3d();

}

void cb_show_tsp_fc()
{
	char file_path[FL_PATH_MAX];

	fc_tsp_file->show();

	while (fc_tsp_file->visible())
	{
		Fl::wait();
	}

	int count = fc_tsp_file->count();
	if (count > 0)
	{
		inp_tsp_file->value("");

		for (int i = 1; i <= count; ++i)
		{
			if (!fc_tsp_file->value(i))
				break;

			fl_filename_relative(file_path, sizeof(file_path), fc_tsp_file->value(i));
			inp_tsp_file->value(file_path);
		}

		inp_tsp_file->redraw();
	}
}

void cb_show_adv_fc()
{
	char file_path[FL_PATH_MAX];

	fc_tsp_file->show();

	while (fc_tsp_file->visible())
	{
		Fl::wait();
	}
	int count = fc_tsp_file->count();
	if (count > 0)
	{
		fl_filename_relative(file_path, sizeof(file_path), fc_tsp_file->value(1));
		buffer_adv_config->loadfile(file_path);
		inp_adv_config_file->value(file_path);
	}
}

void cb_method()
{
	const char * method = choice_method->text();

	group_config_aco->hide();
	group_config_pso->hide();
	group_config_pso_discrete->hide();

	if (!strcmp(method, METHOD_LABELS.PSO))
	{
		group_config_pso->show();
	}
	else if (!strcmp(method, METHOD_LABELS.ACO))
	{
		group_config_aco->show();
	}
	else if (!strcmp(method, METHOD_LABELS.PSO_DESCRETE))
	{

	}
}

int setWindow(int argc, char **argv) {
	{
		main_window = new Fl_Double_Window(611, 612, LABEL_ViSSI_MAIN);
		main_window->callback(cb_close_main_window);

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
					choice_discrete_model->callback((Fl_Callback*)cb_discrete_model);
				} // Fl_Choice* choice_discrete_model

				{
					group_discrete_seq = new Fl_Group(38, 140, 495, 150);
					group_discrete_seq->box(FL_EMBOSSED_BOX);

					//{
					//	choice_discrete_seq_func = new Fl_Choice(113, 155, 226, 25, "Function:");
					//	choice_discrete_seq_func->down_box(FL_BORDER_BOX);
					//	choice_discrete_seq_func->callback((Fl_Callback*)callback);
					//} // Fl_Choice* choice_discrete_seq_func
					//{
					//	btn_discrete_tspConfig = new Fl_Button(180, 223, 158, 27, "Configure Positions...");
					//	btn_discrete_tspConfig->callback((Fl_Callback*)callback);
					//} // Fl_Button* btn_discrete_tspConfig
					//{
					//	inp_discrete_tsp_citynum = new Fl_Input(113, 189, 85, 25, "City Num:");
					//} // Fl_Input* inp_discrete_tsp_citynum
					//{
					//	chk_discrete_tsp_randPos = new Fl_Check_Button(50, 225, 150, 25, "random position");
					//	chk_discrete_tsp_randPos->down_box(FL_DOWN_BOX);
					//} // Fl_Check_Button* chk_discrete_tsp_randPos

					Fl_Button* button = new Fl_Button(430, 160, 25, 25);
					button->labelcolor(FL_YELLOW);
					button->callback((Fl_Callback *)cb_show_tsp_fc);
					Fl_File_Icon::load_system_icons();
					Fl_File_Icon * icon = Fl_File_Icon::find(".", Fl_File_Icon::DIRECTORY);
					icon->label(button);

					inp_tsp_file = new Fl_Input(140, 160, 270, 25, "TSP Config:");
					inp_tsp_file->value(DEFAULT_TSP_CONFIG_FILE);
					inp_tsp_file->align(FL_ALIGN_LEFT);

					group_discrete_seq->end();

				} // Fl_Group* group_discrete_seq

				{
					group_discrete_path = new Fl_Group(38, 140, 495, 150);
					group_discrete_path->box(FL_EMBOSSED_BOX);

					{
						inp_path_scale = new Fl_Input(150, 161, 100, 24, "Grid Scale:");
					}

					group_discrete_path->end();
					group_discrete_path->hide();
				}

				{
					//rgroup_discrete_draw_mode = new Fl_Group(50, 255, 260, 31);
					//rgroup_discrete_draw_mode->box(FL_THIN_UP_FRAME);

					//{
					//	rbtn_discrete_tsp_lines = new Fl_Round_Button(55, 258, 105, 26, "draw lines");
					//	rbtn_discrete_tsp_lines->type(102);
					//	rbtn_discrete_tsp_lines->down_box(FL_ROUND_DOWN_BOX);
					//	rbtn_discrete_tsp_lines->callback((Fl_Callback*)callback);
					//} // Fl_Round_Button* rbtn_discrete_tsp_lines
					//{
					//	rbtn_discrete_tsp_lineStrip = new Fl_Round_Button(165, 259, 125, 25, "draw line-stripe");
					//	rbtn_discrete_tsp_lineStrip->type(102);
					//	rbtn_discrete_tsp_lineStrip->down_box(FL_ROUND_DOWN_BOX);
					//	rbtn_discrete_tsp_lineStrip->callback((Fl_Callback*)callback);
					//} // Fl_Round_Button* rbtn_discrete_tsp_lineStrip

					//rgroup_discrete_draw_mode->end();
				} // Fl_Group* rgroup_discrete_draw_mode

				group_discrete->end();
			} // Fl_Group* group_discrete

			{
				group_adv = new Fl_Group(20, 85, 570, 500, "Advance");
				group_adv->selection_color((Fl_Color)29);
				

				{
					text_adv_config = new Fl_Text_Display(35, 110, 450, 220, "Problem Config");
					buffer_adv_config = new Fl_Text_Buffer();
					text_adv_config->buffer(buffer_adv_config);
					//buffer_adv_config->loadfile("..\\src\\config\\cities.json");

				} // Fl_Scroll* o

				{
					btn_adv_load = new Fl_Button(35, 340, 90, 25, "Load...");
					btn_adv_load->callback((Fl_Callback*)cb_show_adv_fc);
					inp_adv_config_file = new Fl_Input(130, 340, 300, 25);

				} // Fl_Button* btn_adv_load

				group_adv->end();

			} // Fl_Group* group_adv

			tabs_model->end();
			Fl_Group::current()->resizable(tabs_model);
		} // Fl_Tabs* tabs_model

		{
			group_config_si = new Fl_Group(15, 325, 590, 220);

			{
				Fl_Box* o = new Fl_Box(17, 325, 210, 35, "Swarm Intelligence Method");
				o->labeltype(FL_ENGRAVED_LABEL);
				o->labelfont(1);
			} // Fl_Box* o

			{
				choice_method = new Fl_Choice(85, 359, 185, 24, "method:");
				choice_method->down_box(FL_BORDER_BOX);
				choice_method->callback((Fl_Callback*)cb_method);
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
				group_config_aco = new Fl_Group(30, 390, 560, 140);
				group_config_aco->box(FL_EMBOSSED_BOX);
				group_config_aco->labeltype(FL_SHADOW_LABEL);

				inp_aco_popSize = new Fl_Input(150, 401, 160, 25, "Population Size:");
				inp_aco_maxT = new Fl_Input(150, 432, 160, 25, "Max Iterations:");
				inp_aco_T0 = new Fl_Input(150, 465, 160, 25, "Pher0:");
				inp_aco_P0 = new Fl_Input(150, 497, 160, 25, "Prob0:");

				group_config_aco->end();
			}// Fl_Group* group_config_aco

			{
				group_config_pso_discrete = new Fl_Group(30, 390, 560, 140);
				group_config_pso_discrete->box(FL_EMBOSSED_BOX);
				group_config_pso_discrete->labeltype(FL_SHADOW_LABEL);



				group_config_pso_discrete->end();

			}// Fl_Group* group_config_pso_discrete

			group_config_si->end();
		} //group_config_si

		{
			btn_start = new Fl_Button(30, 560, 80, 30, "Start");
			btn_start->callback((Fl_Callback*)cb_start);
		} // Fl_Button* btn_start

		{
			//btn_save = new Fl_Button(120, 560, 80, 30, "Save");
			//btn_save->callback((Fl_Callback*)callback);
		} // Fl_Button* btn_save

		{
			//btn_exit = new Fl_Button(510, 560, 80, 30, "Exit");
			//btn_exit->callback((Fl_Callback*)callback);
		} // Fl_Button* btn_exit

		{
			//btn_about = new Fl_Button(505, 10, 85, 30, "About");
		} // Fl_Button* btn_about
		main_window->end();

	} // Fl_Double_Window* main_window

	{
		fc_tsp_file = new Fl_File_Chooser(".", "*", Fl_File_Chooser::SINGLE, "Choose Config File");
		//fc_tsp_file->callback(fc_callback);
		fc_tsp_file->filter("JSON File (*.json)\t");
	}


	initCountiModelChoice();
	initMethodChoice(CONTINUOUS);
	initConfig();
	initAlert();

	main_window->show(argc, argv);

	return Fl::run();
}

void cb_close_main_window(Fl_Widget *, void *)
{
	destroyAlert();
	main_window->hide();
	Fl::delete_widget(main_window);
}

void callback(Fl_Widget* o, void*)
{
	//printf("Default callback\n");
}

