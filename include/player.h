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
void moveDown(ElementList* player);
void movePlayer(ElementList* player);
void checkPlayerPos(ElementList* player);

#endif