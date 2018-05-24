#ifndef PLAYER_H
    #define PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "struct.h"
#include "display.h"

#define MAX_SPEED 0.25
#define ACC 0.03

/* Déplacement du joueur vers le haut */
void moveUp(ElementList* player);

/* Déplacement du joueur vers le bas */
void moveDown(ElementList* player);

/* Ralentissement du joueur */
void slowDown(ElementList* player);

/* Déplacement du joueur en fonction de sa vitesse */
void movePlayer(ElementList* player);

/* Vérifie que le joueur est dans le fenêtre, sinon le replace dans la fenêtre */
void checkPlayerPos(ElementList* player);

/* Défini les propriétés du joueur en fonction du nombre de bonus récupérés */
void checkBonus(ElementList* player);


/* Fonctions relatives aux missiles */
void moveMissiles(ElementList* shooter);

/* Fonctions de déplacement vertical des éléments */
void moveVertical(World world, ElementList* list);

/* Fonctions de tir des ennemis */
void enemyShooting(ElementList* ennemyList, GLuint malus);

#endif