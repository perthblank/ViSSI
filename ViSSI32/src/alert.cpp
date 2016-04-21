#include "alert.h"

Fl_Double_Window *window_alert = (Fl_Double_Window *)0;

Fl_Text_Display *text_alert = (Fl_Text_Display *)0;

Fl_Text_Buffer *buffer_text_alert = 0;

void cb_close_alert(Fl_Widget *, void *)
{
	window_alert->hide();
}
void initAlert()
{
	window_alert = new Fl_Double_Window(308, 107, "ViSSI Alert");
	{
		text_alert = new Fl_Text_Display(15, 15, 280, 75);
		text_alert->box(FL_THIN_UP_FRAME);
	}
	window_alert->end();
	window_alert->callback(cb_close_alert);

	buffer_text_alert = new Fl_Text_Buffer();
	text_alert->buffer(buffer_text_alert);
}

void alertMsg(const char * msg)
{
	buffer_text_alert->remove(0, 100);
	buffer_text_alert->append(msg);
	window_alert->show();
}



void destroyAlert()
{
	Fl::delete_widget(window_alert);
}
