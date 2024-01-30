//gui.cc
//Frederic Piguet : 35%
//Ismail Essaidi : 65%

#include "gui.h"

using namespace std;

namespace 
{
	unsigned int val(1);
	constexpr int taille_dessin(500);
}

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									Frame frame);

static Frame default_frame = {0.0, 500, 0.0, 500, 1, taille_dessin, taille_dessin}; 

static void orthographic_projection(const Cairo::RefPtr<Cairo::Context>& cr, 
									Frame frame)
{
	cr->translate(frame.width/2, frame.height/2);
  
	cr->scale(frame.width/(frame.xMax - frame.xMin), 
             -frame.height/(frame.yMax - frame.yMin));

	cr->translate(-(frame.xMin + frame.xMax)/2, -(frame.yMin + frame.yMax)/2);
}


MyArea::MyArea()
{
	setFrame(default_frame);
}

MyArea::~MyArea()
{}

void MyArea::refresh()
{
	auto win = get_window();
	if(win)
	{
		Gdk::Rectangle r(0,0, get_allocation().get_width(), 
						      get_allocation().get_height());
								
		win->invalidate_rect(r,false);
	}
}

void MyArea::adjustFrame() 
{
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();
	
	frame.width  = width;
	frame.height = height;

    double new_aspect_ratio((double)width/height);
    if( new_aspect_ratio > default_frame.asp)
    {
	    frame.yMax = default_frame.yMax;
	    frame.yMin = default_frame.yMin;
	  
	    double delta(default_frame.xMax - default_frame.xMin);
	    double mid((default_frame.xMax + default_frame.xMin)/2);

	    frame.xMax = mid + 0.5*(new_aspect_ratio/default_frame.asp)*delta;
	    frame.xMin = mid - 0.5*(new_aspect_ratio/default_frame.asp)*delta; 
    }
    else
    {
	    frame.xMax = default_frame.xMax;
	    frame.xMin = default_frame.xMin;
	
 	    double delta(default_frame.yMax - default_frame.yMin);
	    double mid((default_frame.yMax + default_frame.yMin)/2);
	    frame.yMax = mid + 0.5*(default_frame.asp/new_aspect_ratio)*delta;
	    frame.yMin = mid - 0.5*(default_frame.asp/new_aspect_ratio)*delta;
    }
}

void MyArea::setFrame(Frame f)
{
	f.asp = f.width/f.height;
	frame = f;
} 

bool MyArea::on_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	graphic_set_context(cr);
	
	adjustFrame();
	orthographic_projection(cr, frame);
	
	cr->set_source_rgb(0., 0., 0.);
	cr->paint();
	
	draw_grid();
	draw_modele();
	
	return true;
}

MyEvent::MyEvent() :
m_Box(Gtk::ORIENTATION_HORIZONTAL,5),
m_Box_left(Gtk::ORIENTATION_VERTICAL,5),
m_Box_right(Gtk::ORIENTATION_VERTICAL),
m_Box_General(Gtk::ORIENTATION_VERTICAL),
m_Box_Info(Gtk::ORIENTATION_VERTICAL),
m_Box_Fourmiliere(Gtk::ORIENTATION_VERTICAL),
m_Frame_General("General"), m_Frame_Info("Info"), m_Frame_Fourmiliere("Fourmiliere"),
m_Info_Label("nb food: "+get_food_info(),true), m_Fourmiliere_Label("None selected"),
m_exit("exit"), m_open("open"), m_save("save"),
m_start("start"), m_step("step"), m_previous("previous"), m_next("next"),
timer_added(false), timeout_value(10), i(0),  selected(false)
{
	set_border_width(5);
	add(m_Box);
	m_Box_left.set_size_request(150);
	m_Box.pack_start(m_Box_left,false,false);
	m_Box.pack_start(m_Box_right,true,true);
	m_Box_Info.add(m_Info_Label);
	m_Box_left.pack_start(m_Frame_General,true,true);
	m_Box_left.pack_start(m_Frame_Info,true,true);
	m_Box_left.pack_start(m_Frame_Fourmiliere,true,true);
	m_Frame_General.add(m_Box_General);
	m_Frame_Info.add(m_Box_Info);
	m_Frame_Fourmiliere.add(m_Box_Fourmiliere);
	m_Box_General.pack_start(m_exit,true,false);
	m_Box_General.pack_start(m_open,true,false);
	m_Box_General.pack_start(m_save,true,false);
	m_Box_General.pack_start(m_start,true,false);
	m_Box_General.pack_start(m_step,true,false);
	m_Box_Fourmiliere.pack_start(m_previous,true,false);
	m_Box_Fourmiliere.pack_start(m_next,true,false);
	m_Box_Fourmiliere.add(m_Fourmiliere_Label);
	m_Area.set_size_request(taille_dessin,taille_dessin);
	m_Box_right.pack_start(m_Area);
	show_all_children();
	add_events(Gdk::KEY_RELEASE_MASK);
	m_exit.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_exit) );
	m_open.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_open) );
	m_save.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_save) );
	m_start.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_start) );
	m_step.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_step) );
	m_previous.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_previous) );
	m_next.signal_clicked().connect(sigc::mem_fun(*this,
	&MyEvent::on_button_clicked_next) );
}

MyEvent::~MyEvent()
{}

void MyEvent::on_button_clicked_exit()
{
	exit(EXIT_SUCCESS);
}

void MyEvent::on_button_clicked_open()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Open", Gtk::RESPONSE_OK);

	int result = dialog.run();
	
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			clear_simulation();

			cout << "Open clicked." << endl;
			string filename = dialog.get_filename();
			cout << "File selected: " << filename << endl;
			i=0;
			m_Fourmiliere_Label.set_text("None selected");
			lecture(filename);
			m_Area.refresh();
			m_Info_Label.set_text("nb food: " + get_food_info());
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			cout << "Cancel clicked." << endl;
			break;
		}
		default:
		{
			cout << "Unexpected button clicked." << endl;
			break;
		}
	  }
}

void MyEvent::on_button_clicked_save()
{
	Gtk::FileChooserDialog dialog("Please choose a file",
								  Gtk::FILE_CHOOSER_ACTION_SAVE);
    dialog.set_transient_for(*this);

	dialog.add_button("_Cancel", Gtk::RESPONSE_CANCEL);
	dialog.add_button("_Save", Gtk::RESPONSE_OK);

	int result = dialog.run();
	
	switch(result)
	{
		case(Gtk::RESPONSE_OK):
		{
			cout << "save clicked." << endl;
			string filename = dialog.get_filename();
			cout << "File selected: " <<  filename << endl;
			save(filename);
			break;
		}
		case(Gtk::RESPONSE_CANCEL):
		{
			cout << "Cancel clicked." << endl;
			break;
		}
		default:
		{
			cout << "Unexpected button clicked." << endl;
			break;
		}
	}
}

bool MyEvent::disconnect(true);

void MyEvent::on_button_clicked_start()
{
	if(not timer_added)
	{	
		disconnect = false;
		
		m_start.set_label("stop");
		Glib::signal_timeout().connect( sigc::mem_fun(*this, &MyEvent::on_timeout),
									   timeout_value );
		
		timer_added = true;
	
		cout << "Timer added" << endl;
	} else {
		cout << "manually disconnecting the timer " << endl;
		disconnect  = true;
		timer_added = false;
		m_start.set_label("start");
	}
}

bool MyEvent::on_timeout()
{
    if(disconnect)
    {
		return false; 
    }
  
    cout << "This is simulation update number : " << val << endl;
    ++val;
    
    update_simulation();
    update_display();
    m_Area.refresh();

    return true; 
}

void MyEvent::update_display()
{
	if (i >= get_fourmiliere_storage().size()) selected = false;
	if (selected==true)
	{
		Fourmiliere f(get_fourmiliere_storage()[i]);
		m_Fourmiliere_Label.set_text("id: "+ to_string(i)+"\nTotal food: "+
										to_string(f.get_g().get_tot_food())+"\n\nnbC: "
										+ to_string(f.get_nbC())+"\nnbD: "+
										to_string(f.get_nbD())+"\nnbP: "+
										to_string(f.get_nbP())+"\n");
	} else {
		m_Fourmiliere_Label.set_text("None selected");
	}
	m_Info_Label.set_text("nb food: " + get_food_info());
}

void MyEvent::on_button_clicked_step()
{
	if(disconnect)
	{
		cout << "This is simulation update number : " << val << endl;
		++val;
		
		update_simulation();
		update_display();
		m_Area.refresh();
	}
}

void MyEvent::on_button_clicked_previous()
{
	if (get_fourmiliere_storage().size() == 0)
	{
		m_Fourmiliere_Label.set_text("None selected");
		selected=false;
	} else {
		unsigned int nbF(get_fourmiliere_storage().size());
		if(selected==false)
		{
			i=nbF-1;
			Fourmiliere f(get_fourmiliere_storage()[i]);
			Generator g(f.get_g());
			m_Fourmiliere_Label.set_text("id: "+ to_string(i)+"\nTotal food: "+
		                            	to_string(g.get_tot_food())+"\n\nnbC: "
		                            	+ to_string(f.get_nbC())+"\nnbD: "+
		                            	to_string(f.get_nbD())+"\nnbP: "+
			                            to_string(f.get_nbP())+"\n");
			selected=true;
			return;
		}
		if (i==0)
		{
			m_Fourmiliere_Label.set_text("None selected");
			selected=false;
		} else {
			--i;
			Fourmiliere f(get_fourmiliere_storage()[i]);
			Generator g(f.get_g());
			m_Fourmiliere_Label.set_text("id: "+ to_string(i)+"\nTotal food: "+
			                            to_string(g.get_tot_food())+"\n\nnbC: "
			                            + to_string(f.get_nbC())+"\nnbD: "+
			                            to_string(f.get_nbD())+"\nnbP: "+
			                            to_string(f.get_nbP())+"\n");
		}
	}
}

void MyEvent::on_button_clicked_next()
{
	if (get_fourmiliere_storage().size() == 0)
	{
		m_Fourmiliere_Label.set_text("None selected");
		selected=false;
	} else {
		unsigned int nbF(get_fourmiliere_storage().size());
		if(selected==false)
		{
			i=0;
			Fourmiliere f(get_fourmiliere_storage()[i]);
			Generator g(f.get_g());
			m_Fourmiliere_Label.set_text("id: 0\nTotal food: "+
		                            	to_string(g.get_tot_food())+"\n\nnbC: "
		                            	+ to_string(f.get_nbC())+"\nnbD: "+
                            			to_string(f.get_nbD())+"\nnbP: "+
                            			to_string(f.get_nbP())+"\n");
			selected=true;
			return;
		}
		if (i==nbF-1)
		{
			m_Fourmiliere_Label.set_text("None selected");
			i=0;
			selected=false;
		} else {
			++i;
			Fourmiliere f(get_fourmiliere_storage()[i]);
			Generator g(f.get_g());
			m_Fourmiliere_Label.set_text("id: "+ to_string(i)+"\nTotal food: "+
                            			to_string(g.get_tot_food())+"\n\nnbC: "
                            			+ to_string(f.get_nbC())+"\nnbD: "+
                            			to_string(f.get_nbD())+"\nnbP: "+
                            			to_string(f.get_nbP())+"\n");
		}
	}
}

bool MyEvent::on_key_press_event(GdkEventKey * key_event)
{
	if(key_event->type == GDK_KEY_PRESS)
	{
		switch(gdk_keyval_to_unicode(key_event->keyval))
		{
			case 'n':
				this->on_button_clicked_next();
				return true;
			case 'p':
				this->on_button_clicked_previous();
				return true;
			case 's':
				this->on_button_clicked_start();
				break;
			case '1':
				this->on_button_clicked_step();
				return true;
			case 'q':
				this->on_button_clicked_exit();
				break;
		}
	}
	
	return Gtk::Window::on_key_press_event(key_event);
}
