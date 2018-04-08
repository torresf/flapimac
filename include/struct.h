#ifndef STRUCT_H
	#define STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "display.h"

typedef struct element {
	int type; // Type de l'élément (0: Player, 1: Obstacle, 2: Enemy, 3: Bonus)
	float x, y; // Coordonnées de l'élément
	float speed; // Vitesse de l'élément
	struct element* next; // Elément suivant
} Element, *ElementList;

/* Fonction qui alloue la mémoire nécessaire pour un Element, initialise ses champs avec les valeurs x,y,r,g,b passées en paramètre et renvoie le pointeur vers cet espace mémoire */
Element* allocElement(int type, float x, float y);

/* Fonction qui ajoute l'adresse d'un Element passé en paramètre à une liste chainée passée en paramètre sans valeur de retour */
void addElementToList(Element* element, ElementList* list);

/* Fonction qui supprime un élément d'une liste */
void removeElementFromList(Element* element, ElementList* list);

/* Vérifie si il n'y a pas de collisions entre un élément et une liste d'éléments, supprime l'élément de la 2ème liste */
int checkIntersections(ElementList list1, ElementList* list2);

/* Renvoie 1 si les deux éléments entrent en collision, 0 sinon */
int collided(Element a, Element b);

/* Fonction qui dessine les Elements de la liste passée en paramètre sans valeur de retour */
void drawElements(ElementList list);

/* Fonction qui vide la mémoire de tous les Elements de la liste passée en paramètre sans valeur de retour */
void deleteElements(ElementList* list);

#endif