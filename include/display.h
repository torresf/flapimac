#ifndef DISPLAY_H
	#define DISPLAY_H

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "struct.h"

/* Nombres de cases visibles */
static const unsigned int NB_UNITS_X = 30;
static const unsigned int NB_UNITS_Y = 20;

/* Dessine un carré */
void drawLandmark();

/* Dessine un carré "canonique" */
void drawSquare(int full);

/* Fonction qui dessine les Elements de la liste passée en paramètre sans valeur de retour */
void drawElements(ElementList list);

#endif