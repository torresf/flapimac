#ifndef PLAYER_H
    #define PLAYER_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "struct.h"

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

#endif