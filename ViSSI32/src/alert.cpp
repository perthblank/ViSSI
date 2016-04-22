#include "alert.h"
#include "mainh.h"

Fl_Double_Window *window_alert = (Fl_Double_Window *)0;

Fl_Box *text_alert = (Fl_Box *)0;

void cb_close_alert(Fl_Widget *, void *)
{
	window_alert->hide();
}

void initAlert()
{
	window_alert = new Fl_Double_Window(308, 107, "ViSSI Alert");
	{
		text_alert = new Fl_Box(50, 30, 150, 40, "Invalid Parameters!");
		//text_alert->box(FL_THIN_UP_FRAME);
	}
	window_alert->end();
	window_alert->callback(cb_close_alert);
}

void alertMsg(const char * msg)
{
	text_alert->label(msg);
	window_alert->show();
}

void destroyAlert()
{
	Fl::delete_widget(window_alert);
}
