//nourriture.cc
//Frederic Piguet : 15%
//Ismail Essaidi : 85%

#include "nourriture.h"

using namespace std;
namespace 
{
	vector<Nourriture> food_storage;
}

Nourriture::Nourriture(squarecell::Carre c)
: c(c)
{}

bool Nourriture::verif_food()
{
	if (squarecell::valid_carre(c))
	{
		if (!(squarecell::verif_liberte_cellule(c)))
		{
			std::cout << message::food_overlap(c.x,c.y);
			return false;
		
		} else {
			return true;
		}
		
	} else {
		return false;
	}
}

void Nourriture::draw_food()
{
	squarecell::losange(c);
}

string Nourriture::get_coord()
{
	return to_string(c.x)+" "+to_string(c.y);
}

bool init_food(istringstream& data)
{
	squarecell::Carre c; 
	data>>c.x>>c.y;
	c.size=1;
	c.centered=true; 
	Nourriture n(c);
	
	if(n.verif_food())
	{
		squarecell::add_carre(c);
		food_storage.push_back(n);
		return true;
	}
	return false;
}

void draw_food_storage()
{
	for (auto& food : food_storage)
	{
		food.draw_food();
	}
}

string food_info()
{
	return to_string(food_storage.size());
}

void clear_nourriture()
{
	food_storage.clear();
}

squarecell::Carre Nourriture::get_square_n() const
{
	return c;
}

squarecell::Carre recherche_nourriture(squarecell::Carre carre)
{
	unsigned int distance(130);
	squarecell::Carre closest = {1000, 1000, 1, 1};
	
	for (size_t i(0); i < food_storage.size(); ++i)
	{
		unsigned f_x(food_storage[i].get_square_n().x);
		unsigned f_y(food_storage[i].get_square_n().y);
		
		if ((carre.x % 2) + (carre.y % 2) == (f_x % 2) + (f_y % 2)
			or (carre.x % 2) + (carre.y % 2) == (f_x % 2) + (f_y % 2) - 2
			or (carre.x % 2) + (carre.y % 2) == (f_x % 2) + (f_y % 2) + 2)
		{
			int dx(carre.x-f_x);
			int dy(carre.y-f_y);
			unsigned int distance_temp(max(abs(dx), abs(dy)));
			
			if (distance_temp < distance)
			{
				distance = distance_temp;
				closest = food_storage[i].get_square_n();
				
				if (distance <= 2)
				{
					contact_food(closest, carre, dx, dy, i);
					return closest;
				}
			}
		}
	}
	return closest;
}

void contact_food(squarecell::Carre& closest,
										 squarecell::Carre carre, int dx, int dy,
										 int i)
{
	int sgn_1(0);
	int sgn_2(0);
	if (dx != 0) sgn_1 = -(abs(dx)/dx);
	if (dy != 0) sgn_2 = -(abs(dy)/dy);
	
	squarecell::Carre temp_1({carre.x + sgn_1, carre.y + sgn_2, carre.size, 1});
	squarecell::Carre temp_2({carre.x + sgn_1, carre.y + sgn_2, carre.size, 1});
	
	if (dx == 0)
	{
		temp_1.x += 1;
		temp_2.x -= 1;
	} else if (dy == 0) {
		temp_1.y += 1;
		temp_2.y -= 1;
	}
	squarecell::remove_carre(closest);
	squarecell::remove_carre(carre);
	if (verif_liberte_cellule(temp_1) or verif_liberte_cellule(temp_2))
	{
		food_storage.erase(food_storage.begin() + i);
		closest.centered = false;
		squarecell::add_carre(carre);
	} else {
		squarecell::add_carre(closest);
		squarecell::add_carre(carre);
	}
}

string data_food()
{
	unsigned int nbn(food_storage.size());
	string data("# nb food\n"+ to_string(nbn)+"\n");

	if(nbn!=0)
	{
		data=data+"\n#food\n";
	}

	for(size_t i(0); i<nbn; ++i)
	{
		data=data+food_storage[i].get_coord()+"\n";
	}
	
	return data;
}

void food_creation(squarecell::Carre c_new_food)
{
	Nourriture f(c_new_food);
	food_storage.push_back(f);
	squarecell::add_carre(c_new_food);
}

void add_food_to_grid()
{
	for (auto& food : food_storage)
	{
		squarecell::add_carre(food.get_square_n());
	}
}