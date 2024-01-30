#ifndef FOURMI_H_INCLUDED
#define FOURMI_H_INCLUDED

#include <string>
#include <iostream>
#include <sstream>
#include <cmath>
#include "squarecell.h"
#include "nourriture.h"
#include "constantes.h"
#include "message.h"

//DEFINITIONS DE CLASSES

class Fourmi
{
protected:
	int age;
	squarecell::Carre c;
	bool end_of_life = false;
	virtual void draw_fourmi(unsigned int indice) = 0;

public:
	Fourmi(unsigned int x, unsigned int y, unsigned int size,
		   bool centered, unsigned int age);
	virtual ~Fourmi();
	void anniv();
	void end_life();
	bool verif_fourmis(unsigned int type);
	bool get_end_of_life();
	squarecell::Carre get_square();
	std::string get_age();
};


class Collector : public Fourmi
{
private:
	Etat_collector state;
	void move_collector(squarecell::Carre food);
	void move_one(int vx, int vy, int sgn_1, int sgn_2);
	void move_two(int vx, int vy, int sgn_1, int sgn_2);
	
public:
	Collector(unsigned int x, unsigned int y, unsigned int age,
			  bool state, unsigned int size, bool centered);
	~Collector();
	virtual void draw_fourmi(unsigned int indice);
	bool deplacement(squarecell::Carre generator,
					 squarecell::Carre fourmiliere);
	void get_out(squarecell::Carre fourmiliere);
	std::string get_state();
};


class Defensor : public Fourmi
{
private:
	squarecell::Carre free_defensor_spot(squarecell::Carre fourmiliere) const;
	void move_to_goal(squarecell::Carre goal);
	
public:
	Defensor(unsigned int x, unsigned int y, unsigned int age,
			 unsigned int size, bool centered);
	~Defensor();
	virtual void draw_fourmi(unsigned int indice);
	void deplacement(squarecell::Carre fourmiliere);
	bool contact_defensor(squarecell::Carre collector) const;
};


class Predator : public Fourmi
{
private:
	squarecell::Carre move_up_r(bool xdirection);
	squarecell::Carre move_up_l(bool xdirection);
	squarecell::Carre move_down_r(bool xdirection);
	squarecell::Carre move_down_l(bool xdirection);
	
public:
	Predator(unsigned int x, unsigned int y, unsigned int age,
			 unsigned int size, bool centered);
	~Predator();
	virtual void draw_fourmi(unsigned int indice);
	squarecell::Carre deplacement(squarecell::Carre target);
	squarecell::Carre deplacement_in(squarecell::Carre target, 
									 squarecell::Carre fourmiliere);
	squarecell::Carre choose_path_up_r(squarecell::Carre &f);
	squarecell::Carre choose_path_up_l(squarecell::Carre &f);
	squarecell::Carre choose_path_down_r(squarecell::Carre &f);
	squarecell::Carre choose_path_down_l(squarecell::Carre &f);
	void move_predator(squarecell::Carre p);
};


class Generator : public Fourmi	
{
private:
	unsigned int total_food;
	bool end_of_klan;
	
public:
	Generator(unsigned int x, unsigned int y,unsigned int total_food, 
			  unsigned int age = 0,unsigned int size = sizeG, bool centered = true);
	virtual void draw_fourmi(unsigned int indice);
	void move_x();
	void move_y();
	void end_klan();
	bool get_end_of_klan();
	unsigned int get_tot_food();
	void add_food();
	void conso_food(unsigned int nbT);
	unsigned int type_new_fourmi(bool free,unsigned int nbT,unsigned int nbC,
								 unsigned int nbD);
};

//PROTOTYPES DE FONCTIONS

double min(double a, double b);
Collector* init_collector(std::istringstream& data);
Defensor* init_defensor(std::istringstream& data, squarecell::Carre c, unsigned int i);
Predator* init_predator(std::istringstream& data);
int count_first_path(int vx, int vy, int sgn_1, int sgn_2, squarecell::Carre c);
int count_second_path(int vx, int vy, int sgn_1, int sgn_2, squarecell::Carre c);

#endif