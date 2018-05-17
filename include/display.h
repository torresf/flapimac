#ifndef DISPLAY_H
	#define DISPLAY_H

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
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

/* Créer et initialise la texture avec l'image dont l'URL est passé en paramètre */
GLuint createTexture(const char* theFileName);

/* Affiche la texture dont l'id est passé en paramètre */
void displayTexture(GLuint idTexture);

/* Supprime la texture */
void clearTexture(GLuint* idTexture);

/* Affiche les éléments du monde : Le joueur, les obstacles, les ennemis, les bonus */
void drawWorld(World world);

#endif