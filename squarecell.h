#ifndef SQUARECELL_H_INCLUDED
#define SQUARECELL_H_INCLUDED

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include "error_squarecell.h"
#include "graphic.h"

namespace squarecell
{
	//DEFINITIONS DE STRUCURES
	
	struct Carre
	{
		unsigned int x;
		unsigned int y;
		unsigned int size;
		bool centered;
	};
	
	//PROTOTYPES DE FONCTIONS
	
	bool valid_carre(Carre c);
	bool valid_centered(Carre c);
	bool valid_not_centered(Carre c);
	bool superpo_carre(Carre c1, Carre c2);
	bool verif_liberte_cellule(Carre c);
	bool inside_fourmiliere(Carre f, Carre c);
	bool valid_carre_no_cout(Carre c);
	bool valid_centered_no_cout(Carre c);
	bool valid_not_centered_no_cout(Carre c);
	std::vector<int> indice_cellule(Carre c);
	void losange(Carre &c);
	void uniforme(Carre &c, unsigned indice);
	void vide(Carre &c, unsigned indice);
	void diagonale(Carre &c, unsigned indice);
	void grille(Carre &c, unsigned indice);
	void clear_squarecell();
	void add_carre(Carre c);
	void remove_carre(Carre c);
	bool get_bool_random(double p);
	unsigned int get_int_random();
	bool verif_liberte_cellule_generator_x(unsigned int x, unsigned int y);
	bool verif_liberte_cellule_generator_y(unsigned int x, unsigned int y);
}

#endif 