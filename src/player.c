#include "player.h"

/* Déplacement du joueur vers le haut */
void moveUp(ElementList* player) {
	if ((*player)->speed < MAX_SPEED)
		(*player)->speed += ACC;
	else
		(*player)->speed = MAX_SPEED;

}

/* Déplacement du joueur vers le bas */
void moveDown(ElementList* player) {
    if ((*player)->speed > - MAX_SPEED)
		(*player)->speed -= ACC;
	else
		(*player)->speed = -MAX_SPEED;
}

void movePlayer(ElementList* player) {
	(*player)->y += (*player)->speed;
}

void checkPlayerPos(ElementList* player) {
	if ((*player)->y > NB_UNITS_Y - 1) {
		(*player)->y = NB_UNITS_Y - 1;
		(*player)->speed = 0;
	} else if ((*player)->y < 0) {
		(*player)->y = 0;
		(*player)->speed = 0;
	}
}
