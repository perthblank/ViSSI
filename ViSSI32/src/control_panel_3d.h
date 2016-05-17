#ifndef CONTROL_PANEL_3D_H
#define CONTROL_PANEL_3D_H

#include "mainh.h"

int setCp3d();

void cb_play(Fl_Widget *o, void *);

void cb_stop(Fl_Widget *o, void *);

void cb_gotT(Fl_Widget *o, void *);

void cb_save_gbest(Fl_Widget *o, void *);

void cb_toggle_gbest(Fl_Widget* o, void*);

void cb_speed_slider(Fl_Widget *o, void *);

void update_process(void *);

void cb_close_cp3d(Fl_Widget *o, void *);

void cb_view_roll(Fl_Widget *, void *);

#endif