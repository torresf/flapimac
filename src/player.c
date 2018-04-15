#include "player.h"

/* Déplacement du joueur vers le haut */
void moveUp(ElementList* player) {
	if ((*player)->speed_y < MAX_SPEED)
		(*player)->speed_y += ACC;
	else
		(*player)->speed_y = MAX_SPEED;
}

/* Déplacement du joueur vers le bas */
void moveDown(ElementList* player) {
	if ((*player)->speed_y > - MAX_SPEED)
		(*player)->speed_y -= ACC;
	else
		(*player)->speed_y = - MAX_SPEED;
}

/* Ralentissement du joueur */
void slowDown(ElementList* player) {
	if ((*player)->speed_y < 0.001 && (*player)->speed_y > - 0.001)
		(*player)->speed_y = 0;
	else
		(*player)->speed_y = (*player)->speed_y / 1.2;
}

/* Vérifie que le joueur est dans le fenêtre, sinon le replace dans la fenêtre */
void movePlayer(ElementList* player) {
	(*player)->x += (*player)->speed_x;
	(*player)->y += (*player)->speed_y;
}

/* Vérifie que le joueur est dans le fenêtre, sinon le replace dans la fenêtre */
void checkPlayerPos(ElementList* player) {
	if ((*player)->y >= NB_UNITS_Y - 1) {
		(*player)->y = NB_UNITS_Y - 1;
		(*player)->speed_y = 0;
	}
	if ((*player)->y <= 0) {
		(*player)->y = 0;
		(*player)->speed_y = 0;
	}
}

/* Défini les propriétés du joueur en fonction du nombre de bonus récupérés */
void checkBonus(ElementList* player) {
	if ((*player)->nb_bonus == 1) {
		(*player)->shooting_range = 10;
	} else if ((*player)->nb_bonus == 2) {
		(*player)->shooting_rate = 10;
		(*player)->shooting_range = 20;
	}
}

/* Fonctions relatives aux missiles */
void moveMissiles(ElementList* shooter) {
	ElementList tmp = (*shooter)->missiles;
	while (tmp != NULL) {
		// Destruction du missile lorsqu'il dépasse la portée du joueur
		if ((*shooter)->type  == 2)
		{
			if (tmp->x < (*shooter)->x + (*shooter)->shooting_range) {
				ElementList tmp2 = tmp;
				removeElementFromList(tmp2, &((*shooter)->missiles));
			} else {
				tmp->x += tmp->speed_x;
			}
		} else {
			if (tmp->x > (*shooter)->x + (*shooter)->shooting_range) {
				ElementList tmp2 = tmp;
				removeElementFromList(tmp2, &((*shooter)->missiles));
			} else {
				tmp->x += tmp->speed_x;
			}
		}
		
		tmp = tmp->next;
	}
}

/* Fonctions relatives aux bonus */
void moveVertical(ElementList* list) {
	ElementList tmp = *list;
	while (tmp) {
		if (tmp->y >= NB_UNITS_Y - 1 || tmp->y <= 0) {
			tmp->speed_y = -tmp->speed_y;
		}
		tmp->y += tmp->speed_y;
		tmp = tmp->next;
	}
}