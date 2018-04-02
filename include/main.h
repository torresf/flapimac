#ifndef MAIN_H
	#define MAIN_H

#define MAX_SIZE 2000

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 600;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct element {
    int type, x, y; // Type de primitive (GL_POINTS, GL_LINES, GL_TRIANGLES)
    struct Element* next;  // Next Element
} Element, *ElementList;

/* Fonction qui nous sert à redimensionner notre repère dans la fenêtre de travail */
void resizeViewport();

#endif