#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include "mainh.h"

#define DEFAULT_TSP_CONFIG_FILE "..\\ViSSI32\\cities.json"

void callback(Fl_Widget* o, void*);

int setWindow(int argc, char **argv);

int strToInt(string s);

float strToFloat(string s);

void setOptions(Fl_Choice *c, LList & list);

bool initCountiModelChoice();

bool initBenchmarkChoice();

bool initMethodChoice(int flag);

void setMethodOptions(const char * s);

void cb_counti_model(Fl_Choice *c, void*);

void cb_model_tabs(Fl_Widget *wt, void *);

void cb_start(Fl_Widget *c, void *);

void getConfig(PSOConfig *config);

void cb_show_tsp_fc();

void cb_show_adv_fc();

void cb_method();

void cb_close_main_window(Fl_Widget *, void *);

#endif

