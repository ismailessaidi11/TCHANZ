//fourmi.cc
//Frederic Piguet : 90%
//Ismail Essaidi : 10%

#include "fourmi.h"

using namespace std;

Fourmi::Fourmi(unsigned int x, unsigned int y, unsigned int size,
			   bool centered, unsigned int age)
: age(age)
{
	c = {x, y, size, centered};
}

Fourmi::~Fourmi()
{}

squarecell::Carre Fourmi::get_square()
{
	return c;
}

string Fourmi::get_age()
{
	return to_string(age);
}

void Fourmi::anniv()
{
	++age;
	if (age == bug_life) end_of_life = true;
}

bool Fourmi::verif_fourmis(unsigned int type)
{
	enum type_{C,D,P};
	if (squarecell::valid_carre(Fourmi::c))
	{
		if (squarecell::verif_liberte_cellule(Fourmi::c))
		{
			return true;
		
		} else {
			vector<int> v(squarecell::indice_cellule(Fourmi::c));
			switch(type)
			{
				case C:
					cout << message::collector_overlap(c.x, c.y, v[0], v[1]);
					break;
				case D:
					cout << message::defensor_overlap(c.x, c.y, v[0], v[1]);
					break;
				case P:
					cout << message::predator_overlap(c.x, c.y);
					break;
			}
			return false;
		}
	}
	return false;
}

Collector::Collector(unsigned int x, unsigned int y, unsigned int age,
					 bool etat, unsigned int size = sizeC, bool centered = true)
: Fourmi(x, y, size, centered, age)
{
	if (etat)
	{
		state = LOADED;
	} else {
		state = EMPTY;
	}
}

Collector::~Collector()
{
	if (state == LOADED)
	{
		squarecell::Carre new_food = {c.x, c.y, 1, 1};
		food_creation(new_food);
	}
	squarecell::remove_carre(c);
}

string Collector::get_state()
{
	if(state)
	{
		return "true\n";
	}
	return "false\n";
}

Defensor::Defensor(unsigned int x, unsigned int y, unsigned int age,
				   unsigned int size = sizeD, bool centered = true)
: Fourmi(x, y, size, centered, age)
{}

Defensor::~Defensor()
{
	squarecell::remove_carre(c);
}

Predator::Predator(unsigned int x, unsigned int y, unsigned int age,
				   unsigned int size = sizeP, bool centered = true)
: Fourmi(x, y, size, centered, age)
{}

Predator::~Predator()
{
	squarecell::remove_carre(c);
}

Generator::Generator(unsigned int x, unsigned int y, unsigned int total_food,
					 unsigned int age,unsigned int size, bool centered)
: Fourmi(x, y ,size, centered, age),total_food(total_food)
{}

void Collector::draw_fourmi(unsigned int indice)
{
	diagonale(c, indice);
	
	if (state)
	{
		squarecell::losange(c);
	}
}

void Defensor::draw_fourmi(unsigned int indice)
{
	grille(c, indice);
}

void Predator::draw_fourmi(unsigned int indice)
{
	uniforme(c, indice);
}

void Generator::draw_fourmi(unsigned int indice)
{
	uniforme(c, indice);
}

void Generator::move_x()
{
	c.x-=1;
}

void Generator::move_y()
{
	c.y-=1;
}

void Generator::end_klan()
{
	end_of_klan=true;
}

unsigned int Generator::get_tot_food()
{
	return total_food;
}
	
void Generator::add_food()
{
	total_food+=val_food;
}

void Generator::conso_food(unsigned int nbT)
{
	if (nbT*food_rate <= total_food)	total_food-=nbT*food_rate;
	else this->end_klan();
}

unsigned int Generator::type_new_fourmi(bool free,unsigned int nbT,unsigned int nbC, 
										 unsigned int nbD)
{
	enum Nouvelle_fourmi{C,D,P,N};
	unsigned int type;
	double prop_collector(prop_constrained_collector);
	double prop_defensor(prop_constrained_defensor);
	if(free)
	{
		prop_collector=prop_free_collector;
		prop_defensor=prop_free_defensor;
	}
	
	double p(min(1,total_food*birth_rate));
	bool generate(squarecell::get_bool_random(p));
	if (generate)
	{
		if(prop_collector*(nbT-1)>nbC)
		{
			type=C;
		} else if (prop_defensor*(nbT-1)>nbD){
			type=D;
		} else {
			type=P;
		}
		return type;
	}
	type=N;
	return type;
}

bool Generator::get_end_of_klan()
{
	return end_of_klan;
}

Collector* init_collector(istringstream& data)
{
	unsigned int x, y, age;
	string string_state;
	bool state;
	enum type{C,D,P};
	
	data >> x >> y >> age >> string_state;
	
	if (string_state == "true")
	{
		state = true;
	} else {
		state = false;
	}
	
	Collector* ptr = new Collector(x, y, age, state);
	
	if((*ptr).verif_fourmis(C))
	{
		squarecell::add_carre((*ptr).get_square());
		return ptr;
		
	} else {
		ptr = nullptr;
		return ptr;
	}
}

Defensor* init_defensor(istringstream& data, squarecell::Carre c, unsigned int i)
{
	unsigned int x, y, age;
	enum type{C,D,P};
	
	data >> x >> y >> age;
	
	Defensor* ptr = new Defensor(x, y, age);
	
	if((*ptr).verif_fourmis(D))
	{
		if (!(squarecell::inside_fourmiliere(c, (*ptr).get_square())))
		{
			cout << message::defensor_not_within_home(x, y, i);
			ptr = nullptr;
			return ptr;
		}
		squarecell::add_carre((*ptr).get_square());
		return ptr;
		
	} else {
		ptr = nullptr;
		return ptr;
	}
}

Predator* init_predator(istringstream& data)
{
	unsigned int x, y, age;
	enum type{C,D,P};
	
	data >> x >> y >> age;
	
	Predator* ptr = new Predator(x, y, age);
	
	if((*ptr).verif_fourmis(P))
	{
		squarecell::add_carre((*ptr).get_square());
		return ptr;
		
	} else {
		ptr = nullptr;
		return ptr;
	}
}

bool Collector::deplacement(squarecell::Carre generator, squarecell::Carre fourmiliere)
{
	bool gave_food(false);
	if (state == LOADED)
	{
		squarecell::Carre c_contact = {c.x, c.y, c.size+2, 1};
		if (squarecell::superpo_carre(c_contact, fourmiliere))
		{
			state = EMPTY;
			gave_food = true;
		}
	}
	if (state == LOADED)
	{
		this->move_collector(generator);
	} else {
		squarecell::Carre closest_food(recherche_nourriture(c));
		
		if (closest_food.x == 1000)
		{
			if (squarecell::superpo_carre(c, fourmiliere))
			{
				this->get_out(fourmiliere);
			}
			return gave_food;
		} else if (closest_food.centered == false) {
			state = LOADED;
		}
		
		this->move_collector(closest_food);
	}
	return gave_food;
}

void Collector::move_collector(squarecell::Carre food)
{
	squarecell::Carre test(food);
	squarecell::remove_carre(c);
	
	int vx(food.x - c.x);
	int vy(food.y - c.y);
	
	int sgn_1((vx+vy)/2);
	if (sgn_1 != 0)	sgn_1 = abs(sgn_1)/sgn_1;
	int sgn_2((vx-vy)/2);
	if (sgn_2 != 0)	sgn_2 = abs(sgn_2)/sgn_2;
	
	int count_one(count_first_path(vx, vy, sgn_1, sgn_2, c));
	if (count_one == 0 or sgn_1 == 0)
	{
		this->move_one(vx, vy, sgn_1, sgn_2);
		return;
	}
	int count_two(count_second_path(vx, vy, sgn_1, sgn_2, c));
	if (count_two == 0 or sgn_2 == 0)
	{
		this->move_two(vx, vy, sgn_1, sgn_2);
		return;
	}
	
	if (count_one <= count_two)
	{
		this->move_one(vx, vy, sgn_1, sgn_2);
		return;
	} else {
		this->move_two(vx, vy, sgn_1, sgn_2);
		return;
	}
}

int count_first_path(int vx, int vy, int sgn_1, int sgn_2, squarecell::Carre c)
{
	int count(0);
	bool inside_x(true);
	bool inside_y(true);
	for (int i(1); i <= abs((vx+vy)/2); ++i)
	{
		if ((c.x + sgn_1)>125 or (c.x + sgn_1)<2)	inside_x = false;
		if ((c.y + sgn_1)>125 or (c.y + sgn_1)<2)	inside_y = false;
		
		if (inside_x and inside_y)
		{
			c.x += sgn_1;
			c.y += sgn_1;
		} else if (!inside_x) {
			c.x -= sgn_1;
			c.y += sgn_1;
		} else if (!inside_y) {
			c.x += sgn_1;
			c.y -= sgn_1;
		}
		if (valid_carre_no_cout(c) and !verif_liberte_cellule(c))	count += 1;
	}
	for (int i(1); i <= abs((vx-vy)/2); ++i)
	{
		if ((c.x - sgn_2)>125 or (c.x - sgn_2)<2)	inside_x = true;
		if ((c.y + sgn_2)>125 or (c.y + sgn_2)<2)	inside_y = true;
		if (inside_x and inside_y)
		{
			c.x += sgn_2;
			c.y -= sgn_2;
		} else if (!inside_x) {
			c.x -= sgn_2;
			c.y -= sgn_2;
		} else if (!inside_y) {
			c.x += sgn_2;
			c.y += sgn_2;
		}
		if (valid_carre_no_cout(c) and !verif_liberte_cellule(c))	count += 1;
	}
	return count;
}

int count_second_path(int vx, int vy, int sgn_1, int sgn_2, squarecell::Carre c)
{
	int count(0);
	bool inside_x(true);
	bool inside_y(true);
	for (int i(1); i <= abs((vx-vy)/2); ++i)
	{
		if ((c.x + sgn_2)>125 or (c.x + sgn_2)<2)	inside_x = false;
		if ((c.y - sgn_2)>125 or (c.y - sgn_2)<2)	inside_y = false;
		if (inside_x and inside_y)
		{
			c.x += sgn_2;
			c.y -= sgn_2;
		} else if (!inside_x) {
			c.x -= sgn_2;
			c.y -= sgn_2;
		} else if (!inside_y) {
			c.x += sgn_2;
			c.y += sgn_2;
		}
		if (valid_carre_no_cout(c) and !verif_liberte_cellule(c)) count += 1;
	}
	for (int i(1); i <= abs((vx+vy)/2); ++i)
	{
		if ((c.x - sgn_1)>125 or (c.x - sgn_1)<2)	inside_x = true;
		if ((c.y - sgn_1)>125 or (c.y - sgn_1)<2)	inside_y = true;
		if (inside_x and inside_y)
		{
			c.x += sgn_1;
			c.y += sgn_1;
		} else if (!inside_x) {
			c.x -= sgn_1;
			c.y += sgn_1;
		} else if (!inside_y) {
			c.x += sgn_1;
			c.y -= sgn_1;
		}
		if (valid_carre_no_cout(c) and !verif_liberte_cellule(c)) count += 1;
	}
	return count;
}

void Collector::move_one(int vx, int vy, int sgn_1, int sgn_2)
{
	squarecell::Carre c_temp(c);
	int x(c_temp.x);
	int y(c_temp.y);
	if ((vx+vy)/2 != 0)
	{
		if ((c.x + sgn_1)>125 or (c.x + sgn_1)<2)
		{
			c_temp.x -= sgn_1;
			c_temp.y += sgn_1;
		} else if ((c.y + sgn_1)>125 or (c.y + sgn_1)<2) {
			c_temp.x += sgn_1;
			c_temp.y -= sgn_1;
		} else {
			c_temp.x += sgn_1;
			c_temp.y += sgn_1;
		}
		if (valid_carre_no_cout(c_temp) and verif_liberte_cellule(c_temp))
		{
			c = c_temp;
			squarecell::add_carre(c);
			return;
		}
	}
	c_temp.x = x;
	c_temp.y = y;
	if ((vx-vy)/2 != 0)
	{
		c_temp.x += sgn_2;
		c_temp.y -= sgn_2;
		if (valid_carre_no_cout(c_temp) and verif_liberte_cellule(c_temp))
		{
			c = c_temp;
			squarecell::add_carre(c);
			return;
		}
	}
	squarecell::add_carre(c);
}

void Collector::move_two(int vx, int vy, int sgn_1, int sgn_2)
{
	squarecell::Carre c_temp(c);
	int x(c_temp.x);
	int y(c_temp.y);
	if ((vx-vy)/2 != 0)
	{
		if ((c.x + sgn_2)>125 or (c.x + sgn_2)<2)
		{
			c_temp.x -= sgn_2;
			c_temp.y -= sgn_2;
		} else if ((c.y - sgn_2)>125 or (c.y - sgn_2)<2) {
			c_temp.x += sgn_2;
			c_temp.y += sgn_2;
		} else {
			c_temp.x += sgn_2;
			c_temp.y -= sgn_2;
		}
		if (valid_carre_no_cout(c_temp) and verif_liberte_cellule(c_temp))
		{
			c = c_temp;
			squarecell::add_carre(c);
			return;
		}
	}
	c_temp.x = x;
	c_temp.y = y;
	if ((vx+vy)/2 != 0)
	{
		c_temp.x += sgn_1;
		c_temp.y += sgn_1;
		if (valid_carre_no_cout(c_temp) and verif_liberte_cellule(c_temp))
		{
			c = c_temp;
			squarecell::add_carre(c);
			return;
		}
	}
	squarecell::add_carre(c);
}

bool Fourmi::get_end_of_life()
{
	return end_of_life;
}

void Collector::get_out(squarecell::Carre fourmiliere)
{
	int dx_plus(fourmiliere.x + fourmiliere.size - c.x);
	int dx_moins(fourmiliere.x - c.x);
	int dy_plus(fourmiliere.y + fourmiliere.size - c.y);
	int dy_moins(fourmiliere.y - c.y);
	int mini(min({abs(dx_plus),abs(dx_moins),abs(dy_plus),abs(dy_moins)}));
	if (abs(dx_plus) == mini)
	{
		squarecell::Carre out = {c.x + dx_plus + 2, c.y, 1, 1};
		if (squarecell::valid_carre_no_cout(out))
		{
			move_collector(out);
			return;
		}
	}
	if (abs(dx_moins) == mini)
	{
		squarecell::Carre out = {c.x + dx_moins - 3, c.y, 1, 1};
		if (squarecell::valid_carre_no_cout(out))
		{
			move_collector(out);
			return;
		}
	}
	if (abs(dy_plus) == mini)
	{
		squarecell::Carre out = {c.x, c.y + dy_plus + 2, 1, 1};
		if (squarecell::valid_carre_no_cout(out))
		{
			move_collector(out);
			return;
		}
	}
	squarecell::Carre out = {64, 64, 1, 1};
	if (squarecell::valid_carre_no_cout({c.x, c.y + dy_moins - 3, 1, 1}))
	{
		out = {c.x, c.y + dy_moins - 3, 1, 1};
	}
	move_collector(out);
}

void Fourmi::end_life()
{
	end_of_life = true;
}

void Defensor::deplacement(squarecell::Carre fourmiliere)
{
	squarecell::remove_carre(c);
	squarecell::Carre goal(this->free_defensor_spot(fourmiliere));
	this->move_to_goal(goal);
	squarecell::add_carre(c);
}

squarecell::Carre Defensor::free_defensor_spot(squarecell::Carre fourmiliere) const
{
	squarecell::Carre goal = {fourmiliere.x + 2, fourmiliere.y + 2, sizeD, true};
	
	if (!(fourmiliere.x + 2 == c.x or fourmiliere.y + 2 == c.y or
		fourmiliere.x + fourmiliere.size - 3 == c.x or
		fourmiliere.y + fourmiliere.size - 3 == c.y))
	{
		for (unsigned int i(0); i < fourmiliere.size - 2; ++i)
		{
			++goal.y;
			if (squarecell::valid_carre_no_cout(goal) and
				squarecell::verif_liberte_cellule(goal))	return goal;
		}
		for (unsigned int i(0); i < fourmiliere.size - 2; ++i)
		{
			++goal.x;
			if (squarecell::valid_carre_no_cout(goal) and
				squarecell::verif_liberte_cellule(goal)) return goal;
		}
		for (unsigned int i(0); i < fourmiliere.size - 2; ++i)
		{
			--goal.y;
			if (squarecell::valid_carre_no_cout(goal) and
				squarecell::verif_liberte_cellule(goal)) return goal;
		}
		for (unsigned int i(0); i < fourmiliere.size - 3; ++i)
		{
			--goal.x;
			if (squarecell::valid_carre_no_cout(goal) and
				squarecell::verif_liberte_cellule(goal)) return goal;
		}
	}
	return goal;
}

void Defensor::move_to_goal(squarecell::Carre goal)
{
	squarecell::Carre temp_x(c);
	squarecell::Carre temp_y(c);
	int dx(goal.x - c.x);
	int sgn_x;
	if (dx != 0) sgn_x = abs(dx)/dx;
	int dy(goal.y - c.y);
	int sgn_y;
	if (dy != 0) sgn_y = abs(dy)/dy;
	
	temp_x.x += sgn_x;
	temp_y.y += sgn_y;
	if (dx != 0 and verif_liberte_cellule(temp_x))
	{
		c.x += sgn_x;
		return;
	} else if (dy != 0 and verif_liberte_cellule(temp_y)) {
		c.y += sgn_y;
		return;
	}
}

bool Defensor::contact_defensor(squarecell::Carre collector) const
{
	squarecell::Carre defense_range(c);
	defense_range.size += 2;
	
	if (superpo_carre(defense_range, collector))
	{
		return true;
	}
	return false;
}

squarecell::Carre Predator::move_up_r(bool xdirection)
{
	squarecell::Carre c_temp={c.x,c.y,1,true};
	if(xdirection)
	{
		c_temp.x+=2;
		c_temp.y+=1;
	} else {
		c_temp.x+=1;
		c_temp.y+=2;
	}
	return c_temp;
}

squarecell::Carre Predator::move_up_l(bool xdirection)
{
	squarecell::Carre c_temp={c.x,c.y,1,true};
	if(xdirection)
	{
		c_temp.x-=2;
		c_temp.y+=1;
	} else {
		c_temp.x-=1;
		c_temp.y+=2;
	}
	return c_temp;
}

squarecell::Carre Predator::move_down_r(bool xdirection)
{
	squarecell::Carre c_temp={c.x,c.y,1,true};
	if(xdirection)
	{
		c_temp.x+=2;
		c_temp.y-=1;
	} else {
		c_temp.x+=1;
		c_temp.y-=2;
	}
	return c_temp;
}

squarecell::Carre Predator::move_down_l(bool xdirection)
{
	squarecell::Carre c_temp={c.x,c.y,1,true};
	if(xdirection)
	{
		c_temp.x-=2;
		c_temp.y-=1;
	} else {
		c_temp.x-=1;
		c_temp.y-=2;
	}
	return c_temp;
}

squarecell::Carre Predator::choose_path_up_r(squarecell::Carre &f)
{
	int dx(c.x>f.x ? c.x-f.x : f.x-c.x);
	int dy(c.y>f.y ? c.y-f.y : f.y-c.y);
	bool xdirection(true);
	squarecell::Carre c_tmp;
	
	if(dx>dy)
	{
		c_tmp = move_up_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		
	} else {
		c_tmp = move_up_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
	}
	return c;
}

squarecell::Carre Predator::choose_path_up_l(squarecell::Carre &f)
{
	int dx(c.x>f.x ? c.x-f.x : f.x-c.x);
	int dy(c.y>f.y ? c.y-f.y : f.y-c.y);
	bool xdirection(true);
	squarecell::Carre c_tmp;
	
	if(dx>dy)
	{
		c_tmp = move_up_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		
	} else {
		c_tmp = move_up_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
	}
	return c;
}

squarecell::Carre Predator::choose_path_down_r(squarecell::Carre &f)
{
	int dx(c.x>f.x ? c.x-f.x : f.x-c.x);
	int dy(c.y>f.y ? c.y-f.y : f.y-c.y);
	bool xdirection(true);
	squarecell::Carre c_tmp;
	
	if(dx>dy)
	{
		c_tmp = move_down_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		
	} else {
		c_tmp = move_down_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
	}
	return c;
}

squarecell::Carre Predator::choose_path_down_l(squarecell::Carre &f)
{
	int dx(c.x>f.x ? c.x-f.x : f.x-c.x);
	int dy(c.y>f.y ? c.y-f.y : f.y-c.y);
	bool xdirection(true);
	squarecell::Carre c_tmp;
	
	if(dx>dy)
	{
		c_tmp = move_down_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_up_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		
	} else {
		c_tmp = move_down_l(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_l(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_r(!xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
		c_tmp = move_down_r(xdirection);
		if ((valid_carre_no_cout(c_tmp)) and
			(verif_liberte_cellule(c_tmp)))		return c_tmp;
	}
	return c;
}

squarecell::Carre Predator::deplacement(squarecell::Carre target)
{
	squarecell::Carre tmp(target);
	if ((target.x>=c.x) and (target.y>=c.y))	return choose_path_up_r(target);
	
	else if ((target.x<=c.x) and (target.y>=c.y))	return choose_path_up_l(target);
	
	else if ((target.x>=c.x) and (target.y<=c.y))	return choose_path_down_r(target);
	
	else if ((target.x<=c.x) and (target.y<=c.y))	return choose_path_down_l(target);
	
	return tmp;
}

squarecell::Carre Predator::deplacement_in(squarecell::Carre target,
											squarecell::Carre fourmiliere)
{
	squarecell::Carre tmp(target);
	if ((target.x>=c.x) and (target.y>=c.y)) 
	{
		tmp=choose_path_up_r(target);
		
		if (squarecell::inside_fourmiliere(fourmiliere,tmp))	return tmp;
	} else if ((target.x<=c.x) and (target.y>=c.y)) {
		tmp=choose_path_up_l(target);
		
		if (squarecell::inside_fourmiliere(fourmiliere,tmp))	return tmp;
	} else if ((target.x>=c.x) and (target.y<=c.y)) {
		tmp=choose_path_down_r(target);
		
		if (squarecell::inside_fourmiliere(fourmiliere,tmp))	return tmp;
	} else if ((target.x<=c.x) and (target.y<=c.y)) {
		tmp=choose_path_down_l(target);
		
		if (squarecell::inside_fourmiliere(fourmiliere,tmp))	return tmp;
	}
	return c;
}

void Predator::move_predator(squarecell::Carre p)
{
	squarecell::remove_carre(c);
	c=p;
	squarecell::add_carre(c);
}

double min(double a, double b)
{
	if(a<b)	return a;
	return b;
}