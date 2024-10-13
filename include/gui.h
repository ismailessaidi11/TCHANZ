#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <iostream>
#include <fstream>
#include <gtkmm.h>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>
#include "simulation.h"
#include "graphic.h"

struct Frame
{
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double asp;
	int height;
	int width;
};


class MyArea : public Gtk::DrawingArea {
public:
	MyArea();
	virtual ~MyArea();
	
	void setFrame(Frame x);
	void adjustFrame();
		
	void exit();
	void open();
	void save();
	void start();
	void step();
	void previous();
	void next();
	void refresh();
	
protected:
	bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
	
private:
	Frame frame;
};


class MyEvent : public Gtk::Window {
public:
	MyEvent();
	virtual ~MyEvent();

protected:
	void on_button_clicked_exit();
	void on_button_clicked_open();
	void on_button_clicked_save();
	void on_button_clicked_start();
	void on_button_clicked_step();
	void on_button_clicked_previous();
	void on_button_clicked_next();
	void update_display();
	
	bool on_key_press_event(GdkEventKey * key_event);

	Gtk::Box m_Box, m_Box_left, m_Box_right,
			 m_Box_General, m_Box_Info, m_Box_Fourmiliere;
	Gtk::Frame m_Frame_General, m_Frame_Info, m_Frame_Fourmiliere;
	Gtk::Label m_Info_Label, m_Fourmiliere_Label;

	MyArea m_Area;
	
	Gtk::Button m_exit;
	Gtk::Button m_open;
	Gtk::Button m_save;
	Gtk::Button m_start;
	Gtk::Button m_step;
	Gtk::Button m_previous;
	Gtk::Button m_next;
	
	bool on_timeout(); 
	bool timer_added;
	static bool disconnect;
	const int timeout_value;
	unsigned int i;
	bool selected;
};

#endif
