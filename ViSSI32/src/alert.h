#ifndef ALERT_H
#define ALERT_H

#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Text_Display.H>

void initAlert();

void alertMsg(const char * msg);

void destroyAlert();

#endif
