#ifndef WORLD_H
	#define WORLD_H

#include "display.h"
#include "struct.h"
#include "player.h"

#define MAX_SIZE 2000

/* Dimensions de la fenêtre */
static unsigned int WINDOW_WIDTH = 600;
static unsigned int WINDOW_HEIGHT = 400;

/* Nombre de bits par pixel de la fenêtre */
static const unsigned int BIT_PER_PIXEL = 32;

/* Nombre minimal de millisecondes separant le rendu de deux images */
static const Uint32 FRAMERATE_MILLISECONDS = 1000 / 60;

typedef struct world{
	int score; /* Numero du tour */
	ElementList player, obstacleList, enemyList, bonusList; /* Listes des joueurs, obstacles, ennemis, bonus */
} World;

/* Fonction qui nous sert à redimensionner notre repère dans la fenêtre de travail */
void resizeViewport();

/* Initialise les champs et listes chainées du monde */
void initWorld(World *world);

/* Libère la mémoire associée au monde */
void drawWorld(World world);

/* Libère la mémoire associée au monde */
void deleteWorld(World *world);

/* Chargement du niveau */
void loadLevel(World *world);

#endif