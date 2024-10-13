#ifndef FOURMILIERE_H_INCLUDED
#define FOURMILIERE_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include "fourmi.h"
#include "message.h"
#include "nourriture.h"
#include "squarecell.h"
#include "constantes.h"
using namespace std;

//DEFINITIONS DE CLASSES

class Fourmiliere
{
private:
	squarecell::Carre c;
	Generator g;
	unsigned int nbC,nbD,nbP;
	vector<Collector*> collectors;
	vector<Defensor*> defensors;
	vector<Predator*> predators;
	Etat_fourmiliere state;
	unsigned int couleur;
	
public:
	Fourmiliere(squarecell::Carre c, Generator g,
				unsigned int nbC=0, unsigned nbD=0, unsigned int nbP=0);
	void creat_collector(Collector* c);
	void creat_defensor(Defensor* d);
	void creat_predator(Predator* p);
	bool verif_generator(vector<Fourmiliere>& fourmiliere_storage);
	unsigned int get_tot_food();
	unsigned int get_nbC();
	unsigned int get_nbD();
	unsigned int get_nbP();
	Generator get_g();
	squarecell::Carre get_square();
	unsigned int get_nbT();
	bool creat_fourmis(int i,istringstream& data,
					   vector<Fourmiliere>& fourmiliere_storage);
	void draw_fourmiliere();
	void clear_fourmis();
	string coord_fourmilliere(unsigned int i);
	void move_vector_collector();
	void move_vector_defensor(unsigned int indice,
							  vector<Fourmiliere>& fourmiliere_storage);
	unsigned int sizeF();
	void set_carre(squarecell::Carre c);
	void set_state(bool free);
	bool move_generator();
	void update_generator();
	void add_fourmi(unsigned int type);
	void new_collector();
	void new_defensor();
	void new_predator();
	bool freespace_C();
	squarecell::Carre indice_freespace_C();
	bool freespace_P();
	squarecell::Carre indice_freespace_P();
	void set_couleur(unsigned int i);
	void move_vector_predator(vector<Fourmiliere>& fourmiliere_storage,size_t k);
	squarecell::Carre closest(squarecell::Carre carre,
									vector<Fourmiliere>& fourmiliere_storage,size_t k);
	squarecell::Carre closest_predator(squarecell::Carre carre,
									vector<Fourmiliere>& fourmiliere_storage,size_t k);
	void kill(squarecell::Carre p, vector<Fourmiliere>& fourmiliere_storage,size_t k,
			  Predator* predator);
	void destroy_ants();
	bool infiltre(vector<Fourmiliere>& fourmiliere_storage,size_t k);
	squarecell::Carre indice_infiltre(vector<Fourmiliere>& fourmiliere_storage,
									  size_t k);
};

//PROTOTYPES DE FONCTIONS
Fourmiliere init_fourmiliere(istringstream& data);
string data_fourmiliere(vector<Fourmiliere>& fourmiliere_storage);
double sqr(double x);
squarecell::Carre min_dist(squarecell::Carre predator_c ,squarecell::Carre c1,
						   squarecell::Carre c2);

#endif