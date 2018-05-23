#include "struct.h"

/* Fonctions liées aux Elements (joueurs, obstacles, ennemis, bonus) */

/* Fonction qui alloue la mémoire nécessaire pour un Element, initialise ses champs avec les valeurs x, y, speed_x, speed_y, shooting_range, shooting_rate passées en paramètre et renvoie le pointeur vers cet espace mémoire Element */
Element* allocElement(int type, float x, float y, float width, float height, float speed_x, float speed_y, int shooting_range, int shooting_rate, GLuint texture) {
	Element* element;
	element = malloc(sizeof(Element));
	/* Message d'erreur si l'allocation n'a pas fonctionné */
	if (element == NULL)
	{
		printf("Erreur d'allocation de mémoire\n");
		exit(EXIT_FAILURE);
	}
	element->type = type;
	element->x = x;
	element->y = y;
	element->height = height;
	element->width = width;
	element->speed_x = speed_x;
	element->speed_y = speed_y;
	element->nb_bonus = 0;
	element->shooting_range = shooting_range;
	element->shooting_rate = shooting_rate;
	element->loaded = 0;
	element->next = NULL;
	element->missiles = NULL;
	element->texture = texture;
	return element;
}

/* Fonction qui ajoute l'adresse d'un Element passé en paramètre à une liste chainée passée en paramètre sans valeur de retour */
void addElementToList(Element* element, ElementList* list) {
	ElementList tmpList;
	tmpList = *list;
	if (tmpList == NULL) {
		tmpList = element;
	} else {
		element->next = tmpList;
		tmpList = element;
	}
	*list = tmpList;
}

/* Fonction qui supprime un Element passé en paramètre d'une liste chainée passée en paramètre sans valeur de retour */
void removeElementFromList(Element* element, ElementList* list) {
	if (*list) {
		if (element == *list) {
			Element *tmp = *list;
			*list = (*list)->next;
			free(tmp);
		} else {
			Element *tmp = (*list)->next;
			Element *prev = *list;
			while (element != tmp) {
				prev = tmp;
				tmp = tmp->next;
			}
			prev->next = tmp->next;
			free(tmp);
		}
	}
}

/* Vérifie si il n'y a pas de collisions entre les éléments de deux listes différentes, si oui : si action == 0, on supprime l'élément de la 2ème liste | si action == 1, on supprime les éléments des deux listes */
int checkIntersections(ElementList* list1, ElementList* list2, int doubleRemove) {
	Element* tmp1 = *list1;
	Element* tmp2 = *list2;
	while (tmp2) {
		while (tmp1) {
			if (checkSquareSquareCollision(*tmp1, *tmp2) == 1) {
				removeElementFromList(tmp2, list2);
				if (doubleRemove == 1){
					removeElementFromList(tmp1, list1);
				}
				return 1;
			}
			tmp1 = tmp1->next;
		}
		tmp1 = *list1;
		tmp2 = tmp2->next;
	}
	return 0;
}

/* Vérifie si il n'y a pas de collisions entre les éléments de deux listes différentes, si oui : si action == 0, on supprime l'élément de la 2ème liste | si action == 1, on supprime les éléments des deux listes */
int checkMissilesIntersections(World* world) {
	ElementList tmp = world->enemyList;
	while (tmp) {
		if (checkIntersections(&(world->player), &(tmp->missiles), 0)) { 
			return 1;
		}
		tmp = tmp->next;
	}
	return 0;
}

/* Renvoie 1 si les deux éléments entrent en collision, 0 sinon */
int checkSquareSquareCollision(Element a, Element b) {
    // Collision axe x
    int collisionX = 0;
    if (a.x + a.width >= b.x && b.x + b.width >= a.x)
    	collisionX = 1;
    // Collision axe y
    int collisionY = 0;
    if (a.y + a.height >= b.y && b.y + b.height >= a.y)
        collisionY = 1;
    // Collision seulement si les deux axes
    return collisionX * collisionY;
} 

/* Fonction qui vide la mémoire de tous les éléments de la liste passée en paramètre sans valeur de retour. Supprime d'abord les missiles de la listes si il y en a */
void deleteElements(ElementList* list) {
	if (*list != NULL) {
		if ((*list)->missiles != NULL)
			deleteElements(&((*list)->missiles));
		while (*list) {
			ElementList next = (*list)->next;
			free(*list);
			*list = next;
		}
	}
}