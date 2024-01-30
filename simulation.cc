//simulation.cc
//Frederic Piguet : 30%
//Ismail Essaidi : 70%

#include "simulation.h"
using namespace std;

namespace 
{
	vector<Fourmiliere> fourmiliere_storage;
}

void lecture(string nom_fichier)
{
	string line;
	ifstream fichier;
	fichier.open(nom_fichier, ios::in);
	while(getline(fichier >> ws,line))
	{
		if(line[0]=='#') continue;
		if(!decodage_ligne(line))
		{
			return;
		}
	}
}

bool verif_fourmiliere(Fourmiliere f)
{
	if (squarecell::valid_carre(f.get_square()) 
		and f.verif_generator(fourmiliere_storage))
	{
		if (fourmiliere_storage.empty())
		{
			squarecell::add_carre((f.get_g()).get_square());
			return true;
		} else {
			for (size_t i(0);i<fourmiliere_storage.size();++i)
			{
				if(squarecell::superpo_carre(fourmiliere_storage[i].get_square(),
											 f.get_square()))
				{
					cout<<message::homes_overlap(i,fourmiliere_storage.size());
					return false;
				}
			}
			squarecell::add_carre((f.get_g()).get_square());
			return true;
		}
	}
	return false;
}

bool decodage_ligne(string line)
{
	istringstream data(line);
	enum Etat_lecture {NBN,NOURRITURE,NBF,FOURMILIERE,FIN};
	static int etat(NBN); 
	static int i(0), j(0), total(0), nbT(0);
	static bool first_line(true);
	squarecell::Carre c={0,0,5,false};
	Generator g(0,0,0,0,0,true);
	static Fourmiliere f(c,g,0,0,0);
	
	switch(etat)
	{
	case NBN:
		data >> total;
		i=0;
		if (total == 0)	etat = NBF;
		else etat = NOURRITURE ;
		break;
	case NOURRITURE:
		if(!(init_food(data)))
		{
			reset_lecture(i,j,total,nbT,first_line);
			etat = NBN;
			clear_simulation();
			return false;
		}
		++i;
		if (i == total)	etat = NBF;
		break;
	case NBF:
		data >> total;
		i = 0;
		if (total == 0)	etat=NBN;
		else etat = FOURMILIERE ;
		break;
	case FOURMILIERE:
		if (total == 0) etat = NBN;
		if (first_line)
		{
			f=init_fourmiliere(data);
			if (verif_fourmiliere(f))
			{
				f.set_couleur(fourmiliere_storage.size());
				fourmiliere_storage.push_back(f);
			} else {
				reset_lecture(i,j,total,nbT,first_line);
				etat = NBN;
				clear_simulation();
				return false;
			}
			nbT = f.get_nbT()-1;
			first_line = false;
		} else {
			if (!(f.creat_fourmis(i, data,fourmiliere_storage)))
			{
				reset_lecture(i,j,total,nbT,first_line);
				etat = NBN;
				clear_simulation();
				return false;
			}
			++j;
		}
		if (j == nbT)
		{
			first_line = true;
			j = 0;
			++i;
		}
		if(i == total)
		{
			reset_lecture(i,j,total,nbT,first_line);
			etat = NBN;
		}
		break;
	}
	return true;
}

void reset_lecture(int& i, int& j, int& total, int& nbT,bool& first_line)
{
	i = 0;
	j = 0;
	total = 0;
	nbT = 0;
	first_line=true;
}

string get_food_info()
{
	return food_info();
}

vector<Fourmiliere> get_fourmiliere_storage()
{
	return fourmiliere_storage;
}

void draw_modele()
{
	draw_food_storage();
	draw_fourmiliere_storage();
}

void draw_fourmiliere_storage()
{
	unsigned int size(fourmiliere_storage.size());
	
	for (unsigned int i(0); i < size; ++i)
	{
		fourmiliere_storage[i].draw_fourmiliere();
	}
}

void clear_simulation()
{
	clear_fourmiliere();
	clear_nourriture();
	squarecell::clear_squarecell();
}

void clear_fourmiliere()
{
	for (auto fourmiliere : fourmiliere_storage)
	{
		fourmiliere.clear_fourmis();
	}
	fourmiliere_storage.clear();
}

void save(string filename)
{
	ofstream myfile;
	myfile.open(filename);
	myfile << data_food() << data_fourmiliere(fourmiliere_storage);
	myfile.close();
}

void food_generation()
{
	if (squarecell::get_bool_random(food_rate))
	{
		unsigned int x(squarecell::get_int_random());
		unsigned int y(squarecell::get_int_random());
		
		squarecell::Carre c_new_food = {x, y, 1, true};
		
		for (int i(0); i <= max_food_trial; ++i)
		{
			unsigned int count(1);
			
			for (auto& f : fourmiliere_storage)
			{
				if (squarecell::verif_liberte_cellule(c_new_food))
				{
					if (!(squarecell::superpo_carre(c_new_food, f.get_square())))
					{
						if (count == fourmiliere_storage.size())
						{
							food_creation(c_new_food);
							return;
						}
					} else {
						return;
					}
				}
				++count;
			}
			
			if (fourmiliere_storage.size() == 0
				and squarecell::verif_liberte_cellule(c_new_food))
			{
				food_creation(c_new_food);
				return;
			}
		}
	}
	return;
}

void update_simulation()
{
	food_generation();
	add_food_to_grid();
	for (size_t i(0);  i < fourmiliere_storage.size(); ++i)
	{
		Fourmiliere f(fourmiliere_storage[i]);
		unsigned int size_F(f.sizeF());
		update_foumilieres(i,size_F);
		fourmiliere_storage[i].update_generator();
		
		bool end_of_klan(fourmiliere_storage[i].get_g().get_end_of_klan());
		if (end_of_klan==false)
		{
			fourmiliere_storage[i].move_vector_collector();
			fourmiliere_storage[i].move_vector_defensor(i, fourmiliere_storage);
			fourmiliere_storage[i].move_vector_predator(fourmiliere_storage,i);
		}
	}
	for (size_t i(0);  i < fourmiliere_storage.size(); ++i)
	{
		fourmiliere_storage[i].destroy_ants();
		bool end_of_klan(fourmiliere_storage[i].get_g().get_end_of_klan());
		if (end_of_klan)
		{
			fourmiliere_storage[i].clear_fourmis();
			fourmiliere_storage.erase(fourmiliere_storage.begin() + i);
			--i;
		}
	}
}

void update_foumilieres(size_t& i, unsigned int& size_F) 
{
	squarecell::Carre c(fourmiliere_storage[i].get_square());
	c.size=size_F+2;
	bool free(true);
	if (freespace(c,i) and valid_carre_no_cout(c)) //sup droite
	{
		fourmiliere_storage[i].set_carre(c);
		fourmiliere_storage[i].set_state(free);
	} else {
		unsigned int dy(c.size-(fourmiliere_storage[i].get_square()).size);
		c.y-=dy;
		if(freespace(c,i) and valid_carre_no_cout(c)) //inf droite
		{
			fourmiliere_storage[i].set_carre(c);
			fourmiliere_storage[i].set_state(free);
		} else {
			unsigned int dx(c.size-(fourmiliere_storage[i].get_square()).size);
			c.x-=dx;
			if(freespace(c,i) and valid_carre_no_cout(c)) //inf gauche
			{
				fourmiliere_storage[i].set_carre(c);
				fourmiliere_storage[i].set_state(free);
			} else {
				unsigned int dy(c.size-(fourmiliere_storage[i].get_square()).size);
				c.y+=dy;
				if (freespace(c,i) and valid_carre_no_cout(c)) //sup gauche
				{
					fourmiliere_storage[i].set_carre(c);
					fourmiliere_storage[i].set_state(free);
				} else {
					fourmiliere_storage[i].set_state(!free);
				}
			}
		}
	}
}

bool freespace(squarecell::Carre& c,size_t& i)
{
	for (size_t j(0);j<fourmiliere_storage.size();++j)
	{
		squarecell::Carre c_var(fourmiliere_storage[j].get_square());
		if(i!=j)
		{
			if(superpo_carre(c,c_var))
			{
				return false;
			}
		}
	}
	return true;
}
