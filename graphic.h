#ifndef GRAPHIC_H_INCLUDED
#define GRAPHIC_H_INCLUDED

#include <cmath>
#include <cairomm/context.h>
#include <gtkmm/drawingarea.h>

enum Couleur{ROUGE, VERT, BLEU, JAUNE, MAGENTA, CYAN};

void graphic_set_context(const Cairo::RefPtr<Cairo::Context>& cr);
void draw_grid();
void couleur(unsigned int indice);
void couleur_pale(unsigned int indice);
void dessin_losange(unsigned x, unsigned y);
void dessin_uniforme(unsigned x, unsigned y, unsigned side, unsigned indice);
void dessin_vide(unsigned x, unsigned y, unsigned side, unsigned indice);
void dessin_diagonale(unsigned x, unsigned y, unsigned indice);
void dessin_grille(unsigned x, unsigned y, unsigned indice);

#endif