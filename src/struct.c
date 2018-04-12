#include "struct.h"

/* Fonctions liées aux Elements (joueurs, obstacles, ennemis, bonus) */

/* Fonction qui alloue la mémoire nécessaire pour un Element, initialise ses champs avec les valeurs x,y,r,g,b passées en paramètre et renvoie le Elementeur vers cet espace mémoire */
Element* allocElement(int type, float x, float y, float speed_x, float speed_y, int shooting_range, int shooting_rate) {
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
	element->speed_x = speed_x;
	element->speed_y = speed_y;
	element->nb_bonus = 0;
	element->shooting_range = shooting_range;
	element->shooting_rate = shooting_rate;
	element->next = NULL;
	element->missiles = NULL;
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

/* Vérifie si il n'y a pas de collisions entre les élémentsde deux listes différentes, si oui supprime l'élément de la 2ème liste */
int checkIntersections(ElementList list1, ElementList* list2) {
	ElementList tmp1 = list1;
	ElementList tmp2 = *list2;
	while (tmp2) {
		while (list1) {
			if (collided(*list1, *tmp2) == 1) {
				removeElementFromList(tmp2, list2);
				return 1;
			}
			list1 = list1->next;
		}
		list1 = tmp1;
		tmp2 = tmp2->next;
	}
	return 0;
}

/* Renvoie 1 si les deux éléments entrent en collision, 0 sinon */
int collided(Element a, Element b) {
	return (abs(a.x - b.x) * 2 < 2) && (abs(a.y - b.y) * 2 < 2);
}

/* Fonction qui vide la mémoire de tous les Elements de la liste passée en paramètre sans valeur de retour */
void deleteElements(ElementList* list){
	while (*list) {
		ElementList next = (*list)->next;
		free(*list);
		*list = next;
	}
}
