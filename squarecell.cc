//squarecell.cc
//Frederic Piguet : 50%
//Ismail Essaidi : 50%

#include "squarecell.h"

using namespace std;

namespace 
{
	constexpr short unsigned g_dim(7);
	constexpr short unsigned g_max(pow(2,g_dim));
	
	vector<vector<bool>> grid(g_max, vector<bool>(g_max));
	
	default_random_engine e;
}

bool squarecell::valid_carre(squarecell::Carre c)
{
	if (c.centered)
	{
		return squarecell::valid_centered(c);
	} else {
        return squarecell::valid_not_centered(c);
	}
}

bool squarecell::valid_centered(squarecell::Carre c)
{	
	int x(c.x);
	int y(c.y);
	int side(c.size);
	
	if ((side % 2) == 0)
	{
		cout << "side size is even\n";
		return false;
		
	} else if ((x >= g_max) or (x <= 0)) {
		cout << error_squarecell::print_index(x, g_max - 1);
		return false;

	} else if ((y >= g_max) or (y <= 0)) {
		cout << error_squarecell::print_index(y, g_max - 1);
		return false;
		
	} else if (((x + side/2) >= g_max) or ((x - side/2) <= 0)) {
		cout << error_squarecell::print_outside(x, side, g_max - 1);
		return false;
		
	} else if (((y + side/2) >= g_max) or ((y - side/2) <= 0)) {
		cout << error_squarecell::print_outside(y, side, g_max - 1);
		return false;
	}
	
	return true;
}

bool squarecell::valid_not_centered(squarecell::Carre c)
{
	int x(c.x);
	int y(c.y);
	int side(c.size);
	
	if ((x > g_max) or (x < 0)) {
		cout << error_squarecell::print_index(x, g_max - 1);
		return false;

	} else if ((y > g_max) or (y < 0)) {
		cout << error_squarecell::print_index(y, g_max - 1);
		return false;
		
	} else if ((x + side) > g_max) {
		cout << error_squarecell::print_outside(x, side, g_max - 1);
		return false;
		
	} else if ((y + side) > g_max) {
		cout << error_squarecell::print_outside(y, side, g_max - 1);
		return false;
	}
	
	return true;
}

bool squarecell::valid_carre_no_cout(squarecell::Carre c)
{
	if (c.centered)
	{
		return squarecell::valid_centered_no_cout(c);
	} else {
        return squarecell::valid_not_centered_no_cout(c);
	}
}

bool squarecell::valid_centered_no_cout(squarecell::Carre c)
{	
	int x(c.x);
	int y(c.y);
	int side(c.size);
	
	if ((x >= g_max-1) or (x <= 0)) {
		return false;

	} else if ((y >= g_max-1) or (y <= 0)) {
		return false;
		
	} else if (((x + side/2) >= g_max-1 ) or ((x - side/2) <= 0)) {
		return false;
		
	} else if (((y + side/2) >= g_max-1 ) or ((y - side/2) <= 0)) {
		return false;
	}
	
	return true;
}

bool squarecell::valid_not_centered_no_cout(squarecell::Carre c)
{
	int x(c.x);
	int y(c.y);
	int side(c.size);
	
	if ((x > g_max) or (x < 0)) {
		return false;
	
	} else if ((y > g_max) or (y < 0)) {
		return false;
		
	} else if ((x + side) > g_max) {
		return false;
		
	} else if ((y + side) > g_max) {
		return false;
	}
	
	return true;
}

bool squarecell::superpo_carre(squarecell::Carre c1,squarecell::Carre c2)
{
	unsigned int x1(c1.x);
	unsigned int x2(c2.x);
	unsigned int y1(c1.y);
	unsigned int y2(c2.y);

	if (c1.centered)
	{
		x1=c1.x-(c1.size/2);
		y1=c1.y-(c1.size/2);
	}
	if (c2.centered)
	{
		x2=c2.x-(c2.size/2);
		y2=c2.y-(c2.size/2);
	}
	
	unsigned int dx(x2-x1);
	unsigned int dy(y2-y1);
	unsigned int distance_x(c1.size);
	unsigned int distance_y(c1.size);

	if (x2<x1)
	{
		dx=x1-x2;
		distance_x=c2.size;
	}
	if (y2<y1)
	{
		dy=y1-y2;
		distance_y=c2.size;
	}

	if((dx<distance_x) and (dy<distance_y))
	{
		return true;
	}
	return false;
}

void squarecell::add_carre(squarecell::Carre c)
{
	unsigned int side(c.size);
	unsigned int x(c.x);
	unsigned int y(c.y);
	
	if (c.centered)
	{
		for (unsigned int i(y - side/2); i <= (y + side/2); ++i)
		{
			for (unsigned int j(x - side/2); j <= (x + side/2); ++j)
			{
				grid[(g_max-1-i)][j] = true;
			}
		}

	} else {
		for (unsigned int i(y); i <= (y + side - 1); ++i)
		{
			for (unsigned int j(x); j <= (x + side - 1); ++j)
			{
				grid[(g_max-1-i)][j] = true;
			}
		}
	}
}

void squarecell::remove_carre(squarecell::Carre c)
{
	unsigned int side(c.size);
	unsigned int x(c.x);
	unsigned int y(c.y);
	
	if (c.centered)
	{
		for (unsigned int i(y - side/2); i <= (y + side/2); ++i)
		{
			for (unsigned int j(x - side/2); j <= (x + side/2); ++j)
			{
				grid[(g_max-1-i)][j] = false;
			}
		}
		
	} else {
		for (unsigned int i(y); i <= (y + side - 1); ++i)
		{
			for (unsigned int j(x); j <= (x + side - 1); ++j)
			{
				grid[(g_max-1-i)][j] = false;
			}
		} 
	}
}

bool squarecell::verif_liberte_cellule(squarecell::Carre c)
{
	unsigned int side(c.size);
	unsigned int x(c.x);
	unsigned int y(c.y);
	
	if (c.centered)
	{
		for (unsigned int i(y - side/2); i <= (y + side/2); ++i)
		{
			for (unsigned int j(x - side/2); j <= (x + side/2); ++j)
			{
				if(grid[(g_max-1-i)][j] == true)
				{
					return false;
				}
			}
		}
	    return true;
	    
	} else {
		for (unsigned int i(y); i <= (y + side - 1); ++i)
		{
			for (unsigned int j(x); j <= (x + side - 1); ++j)
			{
			    if(grid[(g_max-1-i)][j] == true)
				{
					return false;
				}
			}
		}
		return true;
	}
}

vector<int> squarecell::indice_cellule(squarecell::Carre c)
{
	unsigned int side(c.size);
	unsigned int x(c.x);
	unsigned int y(c.y);
	vector<int> v;
	
	if (c.centered)
	{
		for (unsigned int i(y - side/2); i <= (y + side/2); ++i)
		{
			for (unsigned int j(x - side/2); j <= (x + side/2); ++j)
			{
				if(grid[(g_max-1-i)][j] == true)
				{
					v.push_back(j);
					v.push_back(i);
				    return v;
				}
			}
		}
		
	} else {
		for (unsigned int i(y); i <= (y + side - 1); ++i)
		{
			for (unsigned int j(x); j <= (x + side - 1); ++j)
			{
				if(grid[(g_max-1-i)][j] == true)
				{
					v.push_back(j);
					v.push_back(i);
					return v;
				}
			}
		}
	}
	return v;
}

bool squarecell::inside_fourmiliere(squarecell::Carre f, squarecell::Carre c)
{
	if(((c.x+c.size/2)<(f.x+f.size-1)) and ((c.y+c.size/2)<(f.y+f.size-1)))
	{
		if(((c.x-c.size/2)>f.x) and ((c.y-c.size/2)>f.y))
		{
			return true;
		}
	}
	return false;
}

void squarecell::losange(squarecell::Carre &c)
{
	dessin_losange(c.x, c.y);
}

void squarecell::uniforme(squarecell::Carre &c, unsigned indice)
{
	dessin_uniforme(c.x, c.y, c.size, indice);
}

void squarecell::vide(squarecell::Carre &c, unsigned indice)
{
	dessin_vide(c.x, c.y, c.size, indice);
}

void squarecell::diagonale(squarecell::Carre &c, unsigned indice)
{
	dessin_diagonale(c.x, c.y, indice);
}

void squarecell::grille(squarecell::Carre &c, unsigned indice)
{
	dessin_grille(c.x, c.y, indice);
}

void squarecell::clear_squarecell()
{
	for (unsigned i(0); i < g_max; ++i)
	{
		for (unsigned j(0); j < g_max; ++j)
		{
			grid[i][j] = false;
		}
	}
}

bool squarecell::verif_liberte_cellule_generator_y(unsigned int x,unsigned int y)
{
	squarecell::Carre c={x,y,1,true};
	for (size_t i(0); i<5;++i)
	{
		++c.x;
		if(!verif_liberte_cellule(c)) return false;
	}
	return true;
}

bool squarecell::verif_liberte_cellule_generator_x(unsigned int x,unsigned int y)
{
	squarecell::Carre c={x,y,1,true};
	for (size_t i(0); i<5 ;++i)
	{
		++c.y;
		if(!verif_liberte_cellule(c)) return false;
	}
	return true;
}

bool squarecell::get_bool_random(double p)
{
	bernoulli_distribution b(p);
	return b(e);
}

unsigned int squarecell::get_int_random()
{
	uniform_int_distribution<unsigned> u(1,g_max-1);
	return u(e);
}