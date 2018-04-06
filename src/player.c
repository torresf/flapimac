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

/* Ralentissement du joueur */
void slowDown(ElementList* player) {
	if ((*player)->speed < 0.001 && (*player)->speed > - 0.001)
		(*player)->speed = 0;
	else
		(*player)->speed = (*player)->speed / 1.2;
}

/* Vérifie que le joueur est dans le fenêtre, sinon le replace dans la fenêtre */
void movePlayer(ElementList* player) {
	(*player)->y += (*player)->speed;
}

/* Vérifie que le joueur est dans le fenêtre, sinon le replace dans la fenêtre */
void checkPlayerPos(ElementList* player) {
	if ((*player)->y > NB_UNITS_Y - 1) {
		(*player)->y = NB_UNITS_Y - 1;
		(*player)->speed = 0;
	}
	if ((*player)->y < 0) {
		(*player)->y = 0;
		(*player)->speed = 0;
	}
}
