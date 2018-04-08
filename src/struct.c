#include "struct.h"

/* Fonctions liées aux Elements (joueurs, obstacles, ennemis, bonus) */

/* Fonction qui alloue la mémoire nécessaire pour un Element, initialise ses champs avec les valeurs x,y,r,g,b passées en paramètre et renvoie le Elementeur vers cet espace mémoire */
Element* allocElement(int type, float x, float y){
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
	element->speed = 0;
	element->next = NULL;
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

/* Fonction qui ajoute l'adresse d'un Element passé en paramètre à une liste chainée passée en paramètre sans valeur de retour */
void removeElementFromList(Element* element, ElementList* list) {
	if (*list) {
		if (element == *list) {
			Element *tmp = *list;
			*list = (*list)->next;
			free(tmp);
		} else{
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

/* Vérifie si il n'y a pas de collisions entre un élément et une liste d'éléments, supprime l'élément de la 2ème liste */
int checkIntersections(ElementList list1, ElementList* list2) {
	ElementList tmp = *list2;
	while (tmp) {
		while (list1) {
			if (collided(*list1, *tmp) == 1) {
				removeElementFromList(tmp, list2);
				return 1;
			}
			list1 = list1->next;
		}
		tmp = tmp->next;
	}
	return 0;
}

/* Renvoie 1 si les deux éléments entrent en collision, 0 sinon */
int collided(Element a, Element b) {
	return (abs(a.x - b.x) * 2 < 2) && (abs(a.y - b.y) * 2 < 2);
}

/* Fonction qui dessine les Elements de la liste passée en paramètre sans valeur de retour */
void drawElements(ElementList list) {
	int r, g, b;
	r = 0;
	g = 0;
	b = 0;
	while (list) {
		switch (list->type){
			case 0:
				b = 255;
				r = 255;
				g = 255;
				break;
			case 1:
				break;
			case 2:
				r = 255;
				break;
			case 3:
				g = 255;
				break;
			default:
				break;
		}
		glColor3ub(r, g, b);
		glPushMatrix();
			glTranslatef(list->x, list->y, 0);
			drawSquare(1);
		glPopMatrix();
		list = list->next;
	}
}

/* Fonction qui vide la mémoire de tous les Elements de la liste passée en paramètre sans valeur de retour */
void deleteElements(ElementList* list){
	while (*list) {
		ElementList next = (*list)->next;
		free(*list);
		*list = next;
	}
}
