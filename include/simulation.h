#ifndef SIMULATION_H_INCLUDED
#define SIMULATION_H_INCLUDED

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "squarecell.h"
#include "fourmiliere.h"
#include "nourriture.h"

using namespace std;

//PROTOTYPES DES FONCTIONS
string get_food_info();
void lecture(string nom_fichier);
bool verif_fourmiliere(Fourmiliere f);
bool decodage_ligne(string line);
void reset_lecture(int& i, int& j, int& total, int& nbT, bool& first_line); 
void draw_modele();
void draw_fourmiliere_storage();
void clear_simulation();
void clear_fourmiliere();
void save(string filename);
void food_generation();
vector<Fourmiliere> get_fourmiliere_storage();
void update_simulation();
void update_foumilieres(size_t& i, unsigned int& size_F);
bool freespace(squarecell::Carre& c,size_t& i);

#endif