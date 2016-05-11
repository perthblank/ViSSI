#include "mainh.h"
#include "control_panel_3d.h"

static void callback3d(Fl_Widget* o, void*) {
	printf("%g     \r", ((Fl_Valuator*)o)->value());
	fflush(stdout);
}



Fl_Group *cp3d_group_proceed = (Fl_Group *)0;

Fl_Button *cp3d_btn_play = (Fl_Button *)0;

Fl_Button *cp3d_btn_stop = (Fl_Button *)0;

Fl_Value_Slider *cp3d_slider_speed = (Fl_Value_Slider *)0;

Fl_Button *cp3d_btn_goT = (Fl_Button *)0;

Fl_Input *cp3d_input_T = (Fl_Input *)0;

Fl_Group *cp3d_group_view = (Fl_Group *)0;

Fl_Roller *cp3d_roll_camera = (Fl_Roller *)0;

Fl_Roller *cp3d_roll_xView = (Fl_Roller *)0;

Fl_Roller *cp3d_roll_yView = (Fl_Roller *)0;

Fl_Group *cp3d_group_trail = (Fl_Group *)0;

Fl_Slider *cp3d_slider_iterate = (Fl_Slider *)0;

Fl_Value_Output *cp3d_inp_time = (Fl_Value_Output *)0;

Fl_Value_Output *cp3d_inp_fitness = (Fl_Value_Output *)0;

Fl_Group *cp3d_group_info = (Fl_Group *)0;

Fl_Output *cp3d_inp_id = (Fl_Output *)0;

Fl_Output *cp3d_inp_method = (Fl_Output *)0;

Fl_Output *cp3d_inp_assign = (Fl_Output *)0;

Fl_Output *cp3d_inp_model = (Fl_Output *)0;

Fl_Output *cp3d_inp_vType = (Fl_Output *)0;

Fl_Double_Window *cp3d_window = 0;

HANDLE update_h = 0;

float roll_viewX_old = 0.5, roll_viewY_old = 0.5, roll_viewCamera_old = 0.5;

const char * roller_viewX_label = "X-way View";
const char * roller_viewY_label = "Y-way View";
const char * roller_viewCamera_label = "Camera View";

unsigned time_second = 0;

int setCp3d() {
	
	{
		cp3d_window = new Fl_Double_Window(567, 381, "Control Panel for 3D");
		cp3d_window->box(FL_ENGRAVED_BOX);
		cp3d_window->color((Fl_Color)43);
		cp3d_window->selection_color((Fl_Color)43);
		cp3d_window->callback((Fl_Callback*)cb_close_cp3d);
		{
			Fl_Box* o = new Fl_Box(10, 10, 555, 370, "Control Panel");
			o->box(FL_THIN_DOWN_BOX);
			o->labelfont(1);
			o->align(Fl_Align(FL_ALIGN_TOP | FL_ALIGN_INSIDE));
		} // Fl_Box* o

		{
			cp3d_group_proceed = new Fl_Group(25, 60, 285, 130, "Proceed");
			cp3d_group_proceed->box(FL_ENGRAVED_BOX);
			{
				cp3d_btn_play = new Fl_Button(50, 75, 40, 35, "@>");
				cp3d_btn_play->callback(cb_play);
			} // Fl_Button* cp3d_btn_play

			{
				cp3d_btn_stop = new Fl_Button(105, 75, 40, 35, "@square");
				cp3d_btn_stop->labelfont(1);
			} // Fl_Button* cp3d_btn_stop

			{
				cp3d_slider_speed = new Fl_Value_Slider(50, 130, 170, 20, "speed");
				cp3d_slider_speed->type(1);
				cp3d_slider_speed->selection_color((Fl_Color)1);
				cp3d_slider_speed->value(0.1);
				cp3d_slider_speed->callback((Fl_Callback*)cb_speed_slider);
			} // Fl_Value_Slider* cp3d_slider_speed

			{
				cp3d_btn_goT = new Fl_Button(255, 75, 40, 35, "@>|");
				cp3d_btn_goT->labelfont(1);
				cp3d_btn_goT->callback((Fl_Callback*)cb_gotT);
			} // Fl_Button* cp3d_btn_goT

			{
				cp3d_input_T = new Fl_Input(180, 81, 65, 24, "T:");
			} // Fl_Input* cp3d_input_T

			cp3d_group_proceed->end();
		} // Fl_Group* cp3d_group_proceed

		{
			cp3d_group_view = new Fl_Group(320, 60, 230, 130, "View");
			cp3d_group_view->box(FL_ENGRAVED_BOX);
			{
				cp3d_roll_camera = new Fl_Roller(365, 75, 21, 90, roller_viewCamera_label);
				cp3d_roll_camera->value(0.5);
				cp3d_roll_camera->callback((Fl_Callback*)cb_view_roll);
			} // Fl_Roller* cp3d_roll_camera

			{
				cp3d_roll_xView = new Fl_Roller(400, 130, 120, 20, roller_viewX_label);
				cp3d_roll_xView->type(1);
				cp3d_roll_xView->value(0.5);
				cp3d_roll_xView->callback((Fl_Callback*)cb_view_roll);
			} // Fl_Roller* cp3d_roll_xView

			{
				cp3d_roll_yView = new Fl_Roller(400, 80, 120, 20, roller_viewY_label);
				cp3d_roll_yView->type(1);
				cp3d_roll_yView->value(0.5);
				cp3d_roll_yView->callback((Fl_Callback*)cb_view_roll);
			} // Fl_Roller* cp3d_roll_yView

			cp3d_group_view->end();
		} // Fl_Group* cp3d_group_view

		{
			cp3d_group_trail = new Fl_Group(25, 220, 375, 135, "Trail");
			cp3d_group_trail->box(FL_ENGRAVED_BOX);
			{
				Fl_Box* o = new Fl_Box(65, 230, 135, 60, "Time Spent");
				o->box(FL_ENGRAVED_BOX);
				o->labelfont(1);
				o->align(Fl_Align(FL_ALIGN_TOP | FL_ALIGN_INSIDE));
			} // Fl_Box* o

			{
				Fl_Box* o = new Fl_Box(230, 230, 135, 60, "Fitness Value");
				o->box(FL_ENGRAVED_BOX);
				o->labelfont(1);
				o->align(Fl_Align(FL_ALIGN_TOP | FL_ALIGN_INSIDE));
			} // Fl_Box* o

			{
				cp3d_slider_iterate = new Fl_Slider(35, 305, 355, 20, "Iterations");
				cp3d_slider_iterate->type(3);
				cp3d_slider_iterate->selection_color((Fl_Color)1);
				cp3d_slider_iterate->value(0.03);
				//cp3d_slider_iterate->dis
			} // Fl_Slider* cp3d_slider_iterate

			{
				cp3d_inp_time = new Fl_Value_Output(80, 255, 105, 25);
				cp3d_inp_time->tooltip("Value Output");
				cp3d_inp_time->maximum(100);
				cp3d_inp_time->step(0.1);
				cp3d_inp_time->callback((Fl_Callback*)callback3d);
			} // Fl_Value_Output* cp3d_inp_time

			{
				cp3d_inp_fitness = new Fl_Value_Output(245, 255, 105, 25);
				cp3d_inp_fitness->tooltip("Value Output");
				cp3d_inp_fitness->maximum(100);
				cp3d_inp_fitness->step(0.1);
				cp3d_inp_fitness->callback((Fl_Callback*)callback3d);
			} // Fl_Value_Output* cp3d_inp_fitness

			cp3d_group_trail->end();
		} // Fl_Group* cp3d_group_trail

		{
			cp3d_group_info = new Fl_Group(410, 220, 135, 135, "---");
			//cp3d_group_info->box(FL_PLASTIC_THIN_DOWN_BOX);
			{
				Fl_Button *btn_save_gbest = new Fl_Button(420, 275, 130, 27, "Save");
				btn_save_gbest->callback((Fl_Callback*)cb_save_gbest);
			} // Fl_Output* cp3d_inp_id

			cp3d_group_info->end();
		} // Fl_Group* cp3d_group_info
		cp3d_window->end();
	} // Fl_Double_Window* o
	cp3d_window->show(0, NULL);

	update_h =
		(HANDLE)_beginthreadex(
			NULL, 0,
			(_beginthreadex_proc_type)update_process,
			0,
			0, NULL);

	time_second = 0;

	return Fl::run();
}

void cb_play(Fl_Widget *o, void *)
{
	if (!strcmp(cp3d_btn_play->label(), "@||"))
	{
		cp3d_btn_play->label("@>");
	}
	else
	{
		cp3d_btn_play->label("@||");
	}

	main_controler->toggleSI();
}

void cb_save_gbest(Fl_Widget* o, void*) 
{
	if (!strcmp(cp3d_btn_play->label(), "@||"))
	{
		cb_play(NULL, NULL);
	}


	Fl_Native_File_Chooser native;
	native.title("Pick a file");
	native.type(Fl_Native_File_Chooser::BROWSE_FILE);
	native.filter("Text\t*.txt\n");	
	// Show native chooser
	switch (native.show()) {
	case -1: fprintf(stderr, "ERROR: %s\n", native.errmsg()); break;	// ERROR
	case  1: fprintf(stderr, "*** CANCEL\n"); fl_beep(); break;		// CANCEL
	default: 								// PICKED FILE
		main_controler->save_gbest(native.filename());
		break;
	}

	
}

void cb_stop(Fl_Widget *o, void *)
{
	
}


int strToInt2(string s)
{
	stringstream ss(s.c_str());
	int v;
	ss >> v;
	return v;
}

void cb_gotT(Fl_Widget *o, void *)
{
	int t = strToInt2(cp3d_input_T->value());
	cout << "cross " << t << endl;
	main_controler->crossSI(t);
}

void cb_speed_slider(Fl_Widget *o, void *)
{
	//cout << cp3d_slider_speed->value() << endl;
	main_controler->setSpeed(cp3d_slider_speed->value());
}

void update_process(void *)
{
	while (!main_controler->is_OK());
	float tmp;
	while (main_controler->is_OK() && cp3d_window)
	{
		cp3d_slider_iterate->value(main_controler->getProcessPrecent());
		cp3d_inp_fitness->value(main_controler->getFitnessValue());
		cp3d_inp_time->value(main_controler->getIterate());
	}

	cp3d_window->redraw();
}

void cb_close_cp3d(Fl_Widget *, void *)
{
	TerminateThread(update_h, 0);
	main_window->show();
	cp3d_window->hide();
	Fl::delete_widget(cp3d_window);
	cp3d_window = 0;
	main_controler->destroyCurrent();
}

void cb_view_roll(Fl_Widget * o, void *)
{
	Fl_Roller * roller = (Fl_Roller*)o;
	float cur = roller->value();
	if(!strcmp(roller->label(), roller_viewX_label))
	{
		main_controler->moveView(MOVE_X, cur - roll_viewX_old);
		roll_viewX_old = cur;
	}
	else if (!strcmp(roller->label(), roller_viewY_label))
	{
		main_controler->moveView(MOVE_Y, cur - roll_viewY_old);
		roll_viewY_old = cur;
	}
	else if (!strcmp(roller->label(), roller_viewCamera_label))
	{
		main_controler->moveView(MOVE_R, cur - roll_viewCamera_old);
		roll_viewCamera_old = cur;
	}
	
}
