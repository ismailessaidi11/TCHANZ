//projet.cc
//Frederic Piguet : 50%
//Ismail Essaidi : 50%

#include <iostream>
#include <gtkmm.h>
#include <cairomm/context.h>

#include "gui.h"
#include "simulation.h"

using namespace std;

int main(int argc, char * argv[])
{
	if (argc != 2)
	{
		cout << "Pas de fichier\n";
		auto app = Gtk::Application::create();
		MyEvent page;

		return app->run(page);
	} else {
		string filename(argv[1]);
		lecture(filename);
    
		auto app = Gtk::Application::create();
		MyEvent page;

		return app->run(page);
	}
	return 0;
}