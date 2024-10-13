//fourmiliere.cc
//Frederic Piguet : 30%
//Ismail Essaidi : 70%

#include "fourmiliere.h"

using namespace std;

squarecell::Carre Fourmiliere::get_square() 
{
	return c;
}

bool Fourmiliere::verif_generator(vector<Fourmiliere>& fourmiliere_storage)
{
	if (!(squarecell::inside_fourmiliere(c,g.get_square())))
	{
		cout << message::generator_not_within_home((g.get_square()).x,
												   (g.get_square()).y,
												   fourmiliere_storage.size());
		return false;
		
	} else if (squarecell::valid_carre(g.get_square())) {
		if (!(squarecell::verif_liberte_cellule(g.get_square())))
		{
			vector<int> v(squarecell::indice_cellule(g.get_square()));
			cout << message::generator_overlap((g.get_square()).x, (g.get_square()).y,
												v[0], v[1]);
			return false;
		}
	}
	
	squarecell::add_carre(g.get_square());
	return true;
}

Fourmiliere::Fourmiliere(squarecell::Carre c, Generator g, 
						unsigned int nbC, unsigned nbD ,unsigned int nbP)
						: c(c), g(g),nbC(nbC),nbD(nbD), nbP(nbP)
{}

Fourmiliere init_fourmiliere(istringstream& data)
{
	unsigned int x,y,side,xg,yg,total_food,nbC,nbD,nbP;
	data >> x >> y >> side >> xg >> yg >> total_food >> nbC >> nbD >> nbP;
	squarecell::Carre c={x,y,side,false};
	Generator g(xg, yg,total_food);
	
	return Fourmiliere(c,g,nbC,nbD,nbP);
}

unsigned int Fourmiliere::get_nbT()
{
	return nbC+nbD+nbP+1;
}

void Fourmiliere::creat_collector(Collector *c)
{
	collectors.push_back(c);
}

void Fourmiliere::creat_defensor(Defensor *d)
{
	defensors.push_back(d);
}

void Fourmiliere::creat_predator(Predator *p)
{
	predators.push_back(p);
}

bool Fourmiliere::creat_fourmis(int i,istringstream& data,
								vector<Fourmiliere>& fourmiliere_storage)
{
	enum Etat_lecture {COLLECTOR,DEFENSOR,PREDATOR,FIN};
	static int type_fourmis(COLLECTOR);
	static unsigned int nbc(0),nbd(0),nbp(0);
	switch (type_fourmis)
	{
	case COLLECTOR:
		if(nbC > nbc)
		{
			Collector* ptr = init_collector(data);
			if(ptr==nullptr)
			{
				nbc=nbd=nbp=0;
				type_fourmis=COLLECTOR;
				return false;
			} else {
				fourmiliere_storage[i].creat_collector(ptr);
				++nbc;
			}
			if (nbc == nbC)
			{
				nbc = 0;
				if (nbD != 0)	type_fourmis=DEFENSOR;
				else if (nbP != 0)	type_fourmis=PREDATOR;
			}
			break;
		}
		if (nbD != 0)	type_fourmis=DEFENSOR;
		else if (nbP != 0)	type_fourmis=PREDATOR;

	case DEFENSOR:
		if(nbD > nbd)
		{
			Defensor* ptr = init_defensor(data,fourmiliere_storage[i].get_square(),i);
			if(ptr==nullptr)
			{
				nbc=nbd=nbp=0;
				type_fourmis=COLLECTOR;
				return false;
			} else {
				fourmiliere_storage[i].creat_defensor(ptr);
				++nbd;
			}
			if (nbD == nbd)
			{
				nbd = 0;
				if (nbP != 0)	type_fourmis=PREDATOR;
				else type_fourmis=COLLECTOR;
			}
			break;
		}
		if (nbP != 0)	type_fourmis=PREDATOR;
		else type_fourmis=COLLECTOR;

	case PREDATOR:
		if (nbP > nbp)
		{
			Predator* ptr = init_predator(data);
			if(ptr==nullptr)
			{
				nbc=nbd=nbp=0;
				type_fourmis=COLLECTOR;
				return false;
			} else {
				fourmiliere_storage[i].creat_predator(ptr);
				++nbp;
			}
		}
		if (nbP == nbp)
		{
			nbc=nbd=nbp=0;
			type_fourmis=COLLECTOR;
		}
		break;
	}
	return true;
}

unsigned int Fourmiliere::get_nbC()
{
	return nbC;
}
unsigned int Fourmiliere::get_nbD()
{
	return nbD;
}
unsigned int Fourmiliere::get_nbP()
{
	return nbP;
}
Generator Fourmiliere::get_g()
{
	return g;
}

void Fourmiliere::set_couleur(unsigned int i)
{
	couleur=i;
}

void Fourmiliere::draw_fourmiliere()
{
	vide(c, couleur);
	g.draw_fourmi(couleur);
	
	for (auto collector : collectors)
	{
		collector->draw_fourmi(couleur);
	}
	for (auto defensor : defensors)
	{
		defensor->draw_fourmi(couleur);
	}
	for (auto predator : predators)
	{
		predator->draw_fourmi(couleur);
	}
}

void Fourmiliere::clear_fourmis()
{
	for (auto collector : collectors)
	{
		delete collector;
	}
	collectors.clear();
	
	for (auto defensor : defensors)
	{
		delete defensor;
	}
	defensors.clear();
	
	for (auto predator : predators)
	{
		delete predator;
	}
	predators.clear();
	
	squarecell::remove_carre(g.get_square());
}

string Fourmiliere::coord_fourmilliere(unsigned int i)
{
	string data("\n 	"+to_string(c.x)+" "+to_string(c.y)+" "+to_string(c.size)+" "+
						to_string(g.get_square().x)+" "+to_string(g.get_square().y)+
						" "+to_string(g.get_tot_food())+" "+to_string(nbC)+" "+
						to_string(nbD)
						+" "+to_string(nbP)+" # anthill # "+to_string(i+1)+"\n");
	if(nbC!=0)
	{
		data=data+"\n	# collectors: \n";
	}
	for(size_t j(0); j<nbC; ++j)
	{
		data=data+" 	"+to_string(collectors[j]->get_square().x)+" "+
			to_string(collectors[j]->get_square().y)+" "+collectors[j]->get_age()
			+" "+collectors[j]->get_state();
	}
	if(nbD!=0)
	{
		data=data+"\n	# defensors:\n";
	}
	for(size_t j(0); j<nbD; ++j)
	{
		data=data+" 	"+to_string(defensors[j]->get_square().x)+" "+
			to_string(defensors[j]->get_square().y)+" "+defensors[j]->get_age()
			+" \n";
	}
	if(nbP!=0)
	{
		data=data+"\n	# predators:\n";
	}
	for(size_t j(0); j<nbP; ++j)
	{
		data=data+" 	"+to_string(predators[j]->get_square().x)+" "+
			to_string(predators[j]->get_square().y)+" "+predators[j]->get_age()
			+" \n";
	}
	return data;
}

unsigned int Fourmiliere::sizeF()
{
	return sqrt(4*(sqr(sizeG)+sqr(sizeC)*nbC+sqr(sizeD)*nbD+sqr(sizeP)*nbP));
}

string data_fourmiliere(vector<Fourmiliere>& fourmiliere_storage)
{
	string data("\n"+to_string(fourmiliere_storage.size())+" # nb anthill");
	for (size_t i(0); i<fourmiliere_storage.size() ; ++i)
	{
		data=data+fourmiliere_storage[i].coord_fourmilliere(i);
	}
	return data;
}

bool Fourmiliere::move_generator() 
{
	squarecell::Carre g_c(g.get_square());
	unsigned int dx(g_c.x-c.x);
	unsigned int dy (g_c.y-c.y);
	unsigned int x(g_c.x-3);
	unsigned int y(g_c.y-3);
	if (dx>3 and (squarecell::verif_liberte_cellule_generator_x(x,y)))
	{
		squarecell::remove_carre(g.get_square());
		g.move_x();
		squarecell::add_carre(g.get_square());
		--x;
	}
	
	if (dy>3 and (squarecell::verif_liberte_cellule_generator_y(x,y)))
	{
		squarecell::remove_carre(g.get_square());
		g.move_y();
		squarecell::add_carre(g.get_square());
	}
	if(inside_fourmiliere(c,g.get_square())) return true;
	return false;
}

void Fourmiliere::update_generator()
{
	unsigned int nbT(this->get_nbT());
	if(!move_generator() or (g.get_tot_food()<=0))
	{
		g.end_klan();
	} else {
		g.conso_food(nbT);
		bool free(true);
		if(state==CONSTRAINED)	free=false;
		unsigned int type(g.type_new_fourmi(free,nbT,nbC,nbD));
		this->add_fourmi(type);
	}
}

void Fourmiliere::add_fourmi(unsigned int type) 
{
	enum Nouvelle_fourmi{C,D,P};
	switch (type)
	{
	case C:
		if(freespace_C())
		{
			new_collector();
			break;
		}
	case D:
		if(freespace_C())
		{
			new_defensor();
			break;
		} 
	case P:
		if(freespace_P())
		{
			new_predator();
			break;
		} 
	default:
		return;
	}
}

void Fourmiliere::new_collector()
{
	squarecell::Carre carre(this->indice_freespace_C());
	unsigned int x(carre.x);
	unsigned int y(carre.y);
	unsigned int age(0);
	creat_collector(new Collector(x,y,age,false,sizeC,true));
	squarecell::add_carre(carre);
	++nbC;
}

void Fourmiliere::new_defensor()
{
	squarecell::Carre carre(this->indice_freespace_C());
	unsigned int x(carre.x);
	unsigned int y(carre.y);
	unsigned int age(0);
	creat_defensor(new Defensor(x,y,age,sizeD,true));
	squarecell::add_carre(carre);
	++nbD;
}

void Fourmiliere::new_predator()
{
	squarecell::Carre carre(this->indice_freespace_P());
	unsigned int x(carre.x);
	unsigned int y(carre.y);
	unsigned int age(0);
	creat_predator(new Predator(x,y,age,sizeP,true));
	squarecell::add_carre(carre);
	++nbP;
}

bool Fourmiliere::freespace_C()
{
	squarecell::Carre fourmi_c{c.x+2,c.y+c.size-3,3,true};
	for(size_t i(0) ; i<c.size-4 ; ++i)
	{
		for(size_t j(0) ; j<c.size-4 ;++j)
		{
			if(valid_carre_no_cout(fourmi_c) and verif_liberte_cellule(fourmi_c))
			{
				return true;
			}
			++fourmi_c.x;
		}
		fourmi_c.x=c.x+2;
		--fourmi_c.y;
	}
	return false;
}

squarecell::Carre Fourmiliere::indice_freespace_C()
{
	squarecell::Carre fourmi_c{c.x+2,c.y+c.size-3,3,true};
	for(size_t i(0) ; i<c.size-4 ; ++i)
	{
		for(size_t j(0) ; j<c.size-4 ;++j)
		{
			if(valid_carre_no_cout(fourmi_c) and verif_liberte_cellule(fourmi_c))
			{
				return fourmi_c;
			}
			++fourmi_c.x;
		}
		fourmi_c.x=c.x+2;
		--fourmi_c.y;
	}
	return fourmi_c;
}

bool Fourmiliere::freespace_P()
{
	squarecell::Carre predator_c{c.x+1,c.y+c.size-2,1,true};
	for(size_t i(0) ; i<c.size-2 ; ++i)
	{
		for(size_t j(0) ; j<c.size-2 ;++j)
		{
			if(squarecell::verif_liberte_cellule(predator_c))
			{
				return true;
			}
			++predator_c.x;
		}
		predator_c.x=c.x+1;
		--predator_c.y;
	}
	return false;
}

squarecell::Carre Fourmiliere::indice_freespace_P()
{
	squarecell::Carre predator_c{c.x+1,c.y+c.size-2,1,true};
	for(size_t i(0) ; i<c.size-2 ; ++i)
	{
		for(size_t j(0) ; j<c.size-2 ;++j)
		{
			if(squarecell::verif_liberte_cellule(predator_c))
			{
				return predator_c;
			}
			++predator_c.x;
		}
		predator_c.x=c.x+1;
		--predator_c.y;
	}
	return predator_c;
}

void Fourmiliere::set_carre(squarecell::Carre c)
{
	this->c=c;
}

void Fourmiliere::set_state(bool free)
{
	if (free)
	{
		state=FREE;
	} else {
		state=CONSTRAINED;
	}
}

void Fourmiliere::move_vector_collector()
{
	for (size_t i(0); i < collectors.size(); ++i)
	{
		collectors[i]->anniv();
		if (!collectors[i]->get_end_of_life() and 
			collectors[i]->deplacement(g.get_square(), c))	g.add_food();
	}
}

void Fourmiliere::move_vector_defensor(unsigned int indice, 
									   vector<Fourmiliere>& fourmiliere_storage)
{
	for (size_t i(0); i < defensors.size(); ++i)
	{
		defensors[i]->anniv();
		if (!squarecell::inside_fourmiliere(c, defensors[i]->get_square()))
		{
			defensors[i]->end_life();
		}
		if (!defensors[i]->get_end_of_life())	defensors[i]->deplacement(c);
		
		for (size_t j(0); j < fourmiliere_storage.size(); ++j)
		{
			if (j == indice) continue;
			for (auto& collector : fourmiliere_storage[j].collectors)
			{
				if (defensors[i]->contact_defensor(collector->get_square()))
				{
					collector->end_life();
				}
			}
		}
	}
}

void Fourmiliere::move_vector_predator(vector<Fourmiliere>& fourmiliere_storage,
										size_t k)
{
	for (size_t i(0); i<predators.size(); ++i)
	{
		predators[i]->anniv();
		squarecell::Carre closest(this->closest(predators[i]->get_square(),
												fourmiliere_storage, k));
		if(state==FREE)
		{
			if(squarecell::inside_fourmiliere(c,predators[i]->get_square()))
			{
				squarecell::Carre p(predators[i]->deplacement_in(closest,c));
				kill(p,fourmiliere_storage,k,predators[i]);
				predators[i]->move_predator(p);
			} else {
				
				squarecell::Carre p(predators[i]->
									deplacement(this->indice_freespace_C()));
				kill(p,fourmiliere_storage,k,predators[i]);
				predators[i]->move_predator(p);
			}
		} else {
			if(infiltre(fourmiliere_storage,k))
			{
				squarecell::Carre f_infiltre(indice_infiltre(fourmiliere_storage,k));
				if (f_infiltre.size!=0)
				{
					squarecell::Carre p(predators[i]->
									deplacement(f_infiltre));
					kill(p,fourmiliere_storage,k,predators[i]);
					predators[i]->move_predator(p);
				}
			} else {
				squarecell::Carre p(predators[i]->deplacement(closest));
				kill(p,fourmiliere_storage,k,predators[i]);
				predators[i]->move_predator(p);
			}
		}
	}
}

void Fourmiliere::kill(squarecell::Carre p, vector<Fourmiliere>& fourmiliere_storage,
					   size_t k, Predator* predator)
{
	p.size+=2;
	for(size_t i(0) ; i<fourmiliere_storage.size() ; ++i)
	{
		if (i!=k)
		{
			for(auto& collector : fourmiliere_storage[i].collectors)
			{
				if (squarecell::superpo_carre(collector->get_square(),p))
				{
					collector->end_life();
				}
			}
			for(auto& other_predator : fourmiliere_storage[i].predators)
			{
				if (squarecell::superpo_carre(other_predator->get_square(),p))
				{
					other_predator->end_life();
					predator->end_life();
				}
			}
		}
	}
}

squarecell::Carre Fourmiliere::closest(squarecell::Carre carre,
									   vector<Fourmiliere>& fourmiliere_storage,
									   size_t k)
{
	unsigned int distance(130);
	squarecell::Carre closest = {1000, 1000, 1, 1};
	for (size_t i(0) ; i<fourmiliere_storage.size(); ++i)
	{   
		if (i!=k)
		{
			for (size_t j(0); j < fourmiliere_storage[i].collectors.size(); ++j)
			{
				unsigned f_x(fourmiliere_storage[i].collectors[j]->get_square().x);
				unsigned f_y(fourmiliere_storage[i].collectors[j]->get_square().y);
			
				int dx(carre.x-f_x);
				int dy(carre.y-f_y);
				unsigned int distance_temp(max(abs(dx), abs(dy)));
						
				if (distance_temp < distance)
				{
					distance = distance_temp;
					closest = fourmiliere_storage[i].collectors[j]->get_square();
				}
			}
		}
	}
	squarecell::Carre closest_p(this->closest_predator(carre,fourmiliere_storage,k));
	closest=min_dist(carre,closest,closest_p);
	return closest;
}

squarecell::Carre min_dist(squarecell::Carre predator_c ,squarecell::Carre c1, 
							squarecell::Carre c2)
{
	double dx1(predator_c.x>c1.x ? predator_c.x-c1.x : c1.x-predator_c.x);
	double dy1(predator_c.y>c1.y ? predator_c.y-c1.y : c1.y-predator_c.y);
	double dist1(sqrt(sqr(dx1)+sqr(dy1)));
	double dx2(predator_c.x>c2.x ? predator_c.x-c2.x : c2.x-predator_c.x);
	double dy2(predator_c.y>c2.y ? predator_c.y-c2.y : c2.y-predator_c.y);
	double dist2(sqrt(sqr(dx2)+sqr(dy2)));
	
	if (min(dist1,dist2)==dist1) return c1;
	return c2;
}

squarecell::Carre Fourmiliere::closest_predator(squarecell::Carre carre,
												vector<Fourmiliere>& fourmiliere_storage,
												size_t k)
{
	unsigned int distance(130);
	squarecell::Carre closest = {1000, 1000, 1, 1};
	for (size_t i(0) ; i<fourmiliere_storage.size(); ++i)
	{   
		if (i!=k)
		{
			for (size_t j(0); j < fourmiliere_storage[i].predators.size(); ++j)
			{
				unsigned f_x(fourmiliere_storage[i].predators[j]->get_square().x);
				unsigned f_y(fourmiliere_storage[i].predators[j]->get_square().y);
			
				int dx(carre.x-f_x);
				int dy(carre.y-f_y);
				unsigned int distance_temp(max(abs(dx), abs(dy)));
						
				if (distance_temp < distance)
				{
					distance = distance_temp;
					closest = fourmiliere_storage[i].predators[j]->get_square();
				}
			}
		}
	}
	return closest;
}

void Fourmiliere::destroy_ants()
{
	for (size_t i(0); i<collectors.size() ; ++i)
	{
		if (collectors[i]->get_end_of_life())
		{
			delete collectors[i];
			collectors.erase(collectors.begin() + i);
			--nbC;
			--i;
		}
	}
	for (size_t i(0); i<defensors.size() ; ++i)
	{
		if (defensors[i]->get_end_of_life())
		{
			delete defensors[i];
			defensors.erase(defensors.begin() + i);
			--nbD;
			--i;
		}
	}
	for (size_t i(0) ; i<predators.size() ; ++i)
	{
		if (predators[i]->get_end_of_life())
		{
			delete predators[i];
			predators.erase(predators.begin() + i);
			--nbP;
			--i;
			continue;
		}
	}
}

bool Fourmiliere::infiltre(vector<Fourmiliere>& fourmiliere_storage,size_t k)
{
	for (size_t i(0) ; i<fourmiliere_storage.size(); ++i)
	{
		if(i!=k)
		{
			for (size_t j(0); j<fourmiliere_storage[i].collectors.size();++j)
			{
				if(superpo_carre(c,fourmiliere_storage[i].collectors[j]->get_square()))
				{
					return true;
				}
			}
			for (size_t j(0); j<fourmiliere_storage[i].predators.size();++j)
			{
				if(superpo_carre(c,fourmiliere_storage[i].predators[j]->get_square()))
				{
					return true;
				}
			}
		}
	}
	return false;
}

squarecell::Carre Fourmiliere::indice_infiltre(vector<Fourmiliere>& fourmiliere_storage
											   ,size_t k)
{
	for (size_t i(0) ; i<fourmiliere_storage.size(); ++i)
	{
		if(i!=k)
		{
			for (size_t j(0); j<fourmiliere_storage[i].collectors.size();++j)
			{
				if(superpo_carre(c,fourmiliere_storage[i].collectors[j]->get_square()))
				{
					return fourmiliere_storage[i].collectors[j]->get_square();
				}
			}
			for (size_t j(0); j<fourmiliere_storage[i].predators.size();++j)
			{
				if(superpo_carre(c,fourmiliere_storage[i].predators[j]->get_square()))
				{
					return fourmiliere_storage[i].predators[j]->get_square();
				}
			}
		}
	}
	squarecell::Carre c={1000,1000,0,true};
	return c;
}

double sqr(double x)
{
	return pow(x,2);
}