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
	ElementList tmp_shooter = *shooter;
	while (tmp_shooter) {
		ElementList tmp = tmp_shooter->missiles;
		while (tmp != NULL) {
			// Destruction du missile lorsqu'il dépasse la portée du joueur
			if (tmp_shooter->type  == 2)
			{
				if (tmp->x < tmp_shooter->x + tmp_shooter->shooting_range) {
					ElementList tmp2 = tmp;
					removeElementFromList(tmp2, &(tmp_shooter->missiles));
				} else {
					tmp->x += tmp->speed_x;
				}
			} else {
				if (tmp->x > tmp_shooter->x + tmp_shooter->shooting_range) {
					ElementList tmp2 = tmp;
					removeElementFromList(tmp2, &(tmp_shooter->missiles));
				} else {
					tmp->x += tmp->speed_x;
				}
			}
			tmp = tmp->next;
		}
		tmp_shooter = tmp_shooter->next;
	}
}

/* Fonctions relatives mouvements verticaux des éléments (bonus / ennemis) */
void moveVertical(World world, ElementList* list) {
	ElementList tmp = *list;
	while (tmp) {
		int collision = 0;
		ElementList tmp2 = world.obstacleList;
		ElementList tmp3 = world.brokableObstacleList;
		while (tmp2){
			if (checkSquareSquareCollision(*tmp, *tmp2) == 1)
				collision = 1;
			tmp2 = tmp2->next;
		}
		while (tmp3){
			if (checkSquareSquareCollision(*tmp, *tmp3) == 1)
				collision = 1;
			tmp3 = tmp3->next;
		}
		if (tmp->y >= NB_UNITS_Y - 1 || tmp->y <= 0 || collision == 1) {
			tmp->speed_y = -tmp->speed_y;
		}
		tmp->y += tmp->speed_y;
		tmp = tmp->next;
	}
}

/* Fonctions de tir des ennemis */
void enemyShooting(ElementList* enemyList, GLuint malus) {
	Element* tmp = *enemyList;
	while (tmp) {
		if (tmp->loaded >= tmp->shooting_rate) {
			// Création d'un élement missile et ajout à la liste
			float missile_size = 0.4;
			addElementToList(allocElement(4, tmp->x - tmp->width/2, tmp->y + (tmp->height-missile_size)/2, missile_size, missile_size, -0.2, 0, 0, 0, 0, malus), &(tmp->missiles));
			tmp->loaded = 0;
		}
		tmp->loaded += 1;
		tmp = tmp->next;
	}
}