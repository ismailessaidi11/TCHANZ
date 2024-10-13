#ifndef NOURRITURE_H_INCLUDED
#define NOURRITURE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "squarecell.h"
#include "message.h"
#include "constantes.h"

using namespace std;

//PROTOTYPES DE FONCTIONS

bool init_food(istringstream& data);
void draw_food_storage();
void clear_nourriture();
void food_creation(squarecell::Carre c_new_food);
void add_food_to_grid();
void contact_food(squarecell::Carre& closest, squarecell::Carre carre,
							  int dx, int dy, int i);
squarecell::Carre recherche_nourriture(squarecell::Carre carre);
string food_info();
string data_food();

//DEFINITIONS DE CLASSES

class Nourriture
{
private:
	squarecell::Carre c;
	
public:
	Nourriture(squarecell::Carre c);
	bool verif_food();
	void draw_food();
	squarecell::Carre get_square_n() const;
	string get_coord();
};

#endif