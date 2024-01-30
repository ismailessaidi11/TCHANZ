//graphic.cc
//Frederic Piguet : 100%
//Ismail Essaidi : 0%

#include "graphic.h"

namespace
{
	constexpr short unsigned g_dim(7);
	constexpr short unsigned g_max(pow(2,g_dim));
	constexpr int taille_dessin(500);

	constexpr double delta((double)taille_dessin/((double)g_max));
}

static const Cairo::RefPtr<Cairo::Context>* ptcr(nullptr);


void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr)
{
	ptcr = &cr;
}

void draw_grid()
{
	(*ptcr)->set_source_rgb(1., 1., 1.);

	(*ptcr)->set_line_width(delta);
	(*ptcr)->move_to(0., delta/2.);
	(*ptcr)->line_to(taille_dessin, delta/2.);
	(*ptcr)->move_to(0., taille_dessin - delta/2.);
	(*ptcr)->line_to(taille_dessin, taille_dessin - delta/2.);
	(*ptcr)->move_to(delta/2.,delta/2.);
	(*ptcr)->line_to(delta/2., taille_dessin - delta/2.);
	(*ptcr)->move_to(taille_dessin - delta/2., delta/2.);
	(*ptcr)->line_to(taille_dessin - delta/2., taille_dessin - delta/2.);

	(*ptcr)->stroke();

	(*ptcr)->set_source_rgb(0.4, 0.4, 0.4);
	(*ptcr)->set_line_width(0.3);

	for (int i(0); i <= g_max; ++i)
	{
		double x(delta*i);

		(*ptcr)->move_to(x,0.);
		(*ptcr)->line_to(x, taille_dessin);

		(*ptcr)->move_to(0., x);
		(*ptcr)->line_to(taille_dessin, x);

		(*ptcr)->stroke();
	}
}

void couleur(unsigned int indice)
{
	unsigned int couleur(indice%6);

	switch(couleur)
	{
		case ROUGE:
			(*ptcr)->set_source_rgb(1., 0., 0.);
			break;
		case VERT:
			(*ptcr)->set_source_rgb(0., 1., 0.);
			break;
		case BLEU:
			(*ptcr)->set_source_rgb(0., 0., 1.);
			break;
		case JAUNE:
			(*ptcr)->set_source_rgb(1., 1., 0.);
			break;
		case MAGENTA:
			(*ptcr)->set_source_rgb(1., 0., 1.);
			break;
		case CYAN:
			(*ptcr)->set_source_rgb(0., 1., 1.);
			break;
	}
}

void couleur_pale(unsigned int indice)
{
	unsigned int couleur(indice%6);

	switch(couleur)
	{
		case ROUGE:
			(*ptcr)->set_source_rgb(1., 0.7, 0.7);
			break;
		case VERT:
			(*ptcr)->set_source_rgb(0.7, 1., 0.7);
			break;
		case BLEU:
			(*ptcr)->set_source_rgb(0.7, 0.7, 1.);
			break;
		case JAUNE:
			(*ptcr)->set_source_rgb(1., 1., 0.8);
			break;
		case MAGENTA:
			(*ptcr)->set_source_rgb(1., 0.8, 1.);
			break;
		case CYAN:
			(*ptcr)->set_source_rgb(0.8, 1., 1.);
			break;
	}
}

void dessin_losange(unsigned x, unsigned y)
{
	(*ptcr)->set_source_rgb(1., 1., 1.);
	(*ptcr)->set_line_width(sqrt((delta*delta)/2));

	(*ptcr)->move_to(x*delta + delta/4., y*delta + delta/4.);
	(*ptcr)->line_to(x*delta + 3*delta/4., y*delta + 3*delta/4.);

	(*ptcr)->stroke();
}

void dessin_uniforme(unsigned x, unsigned y, unsigned side, unsigned indice)
{
	couleur(indice);
	(*ptcr)->set_line_width(delta);

	for (unsigned int i(x-side/2); i <= x+side/2; ++i)
	{
		for (unsigned int j(y-side/2); j <= y+side/2; ++j)
		{
			(*ptcr)->move_to(i*delta, j*delta + delta/2.);
			(*ptcr)->line_to((i+1)*delta, j*delta + delta/2.);

			(*ptcr)->stroke();
		}
	}
}

void dessin_vide(unsigned x, unsigned y, unsigned side, unsigned indice)
{
	couleur(indice);
	(*ptcr)->set_line_width(delta/3.);

	(*ptcr)->move_to(x*delta+delta/2., y*delta+delta/2.);
	(*ptcr)->line_to((x+side)*delta-delta/2., y*delta+delta/2.);
	(*ptcr)->move_to(x*delta+delta/2., (y+side)*delta-delta/2.);
	(*ptcr)->line_to((x+side)*delta-delta/2., (y+side)*delta-delta/2.);
	(*ptcr)->move_to(x*delta+delta/2., y*delta+delta/3.);
	(*ptcr)->line_to(x*delta+delta/2., (y+side)*delta-delta/3.);
	(*ptcr)->move_to((x+side)*delta-delta/2., y*delta+delta/3.);
	(*ptcr)->line_to((x+side)*delta-delta/2., (y+side)*delta-delta/3.);

	(*ptcr)->stroke();
}

void dessin_diagonale(unsigned x, unsigned y, unsigned indice)
{
	couleur(indice);
	(*ptcr)->set_line_width(delta);

	(*ptcr)->move_to(x*delta, y*delta + delta/2.);
	(*ptcr)->line_to((x+1)*delta, y*delta + delta/2.);
	(*ptcr)->move_to((x-1)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->line_to((x)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->move_to((x+1)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->line_to((x+2)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->move_to((x-1)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->line_to((x)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->move_to((x+1)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->line_to((x+2)*delta, (y-1)*delta + delta/2.);

	(*ptcr)->stroke();

	couleur_pale(indice);

	(*ptcr)->move_to(x*delta, (y+1)*delta + delta/2.);
	(*ptcr)->line_to((x+1)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->move_to(x*delta, (y-1)*delta + delta/2.);
	(*ptcr)->line_to((x+1)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->move_to((x-1)*delta, y*delta + delta/2.);
	(*ptcr)->line_to(x*delta, y*delta + delta/2.);
	(*ptcr)->move_to((x+1)*delta, y*delta + delta/2.);
	(*ptcr)->line_to((x+2)*delta, y*delta + delta/2.);

	(*ptcr)->stroke();
}

void dessin_grille(unsigned x, unsigned y, unsigned indice)
{
	couleur_pale(indice);
	(*ptcr)->set_line_width(delta);

	(*ptcr)->move_to((x-1)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->line_to((x)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->move_to((x+1)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->line_to((x+2)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->move_to((x-1)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->line_to((x)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->move_to((x+1)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->line_to((x+2)*delta, (y-1)*delta + delta/2.);

	(*ptcr)->stroke();

	couleur(indice);

	(*ptcr)->move_to(x*delta, y*delta + delta/2.);
	(*ptcr)->line_to((x+1)*delta, y*delta + delta/2.);
	(*ptcr)->move_to(x*delta, (y+1)*delta + delta/2.);
	(*ptcr)->line_to((x+1)*delta, (y+1)*delta + delta/2.);
	(*ptcr)->move_to(x*delta, (y-1)*delta + delta/2.);
	(*ptcr)->line_to((x+1)*delta, (y-1)*delta + delta/2.);
	(*ptcr)->move_to((x-1)*delta, y*delta + delta/2.);
	(*ptcr)->line_to(x*delta, y*delta + delta/2.);
	(*ptcr)->move_to((x+1)*delta, y*delta + delta/2.);
	(*ptcr)->line_to((x+2)*delta, y*delta + delta/2.);

	(*ptcr)->stroke();
}