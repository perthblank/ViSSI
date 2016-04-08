#ifndef MAINH_H
#define MAINH_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Roller.H>
#include <FL/Fl_Slider.H>
#include <FL/Fl_Value_Output.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Tabs.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Scroll.H>

#include <cstdio>
#include <cstring>
#include <Windows.h>
#include <process.h>
#include <iostream>
#include <sstream>
using namespace std;

#define WIN32

#include "controler.h"
#include "type_mgr.h"
#include "vssi_util.h"

extern Controler * main_controler;

extern ConfigTable * config_table;

extern Fl_Double_Window *cp3d_window;

extern Fl_Double_Window *main_window;


#endif
