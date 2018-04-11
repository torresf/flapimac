#ifndef STRUCT_H
	#define STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>
#include "display.h"

typedef struct point {
    float x, y; // Coordonnées du point
} Point;

typedef struct element {
	int type; // Type de l'élément (0: Player, 1: Obstacle, 2: Enemy, 3: Bonus, 4: Missile)
	float x, y; // Coordonnées de l'élément
	float speed_x, speed_y; // Vitesse de l'élément
	int nb_bonus; // nombre de bonus récupérés
	int shooting_range; // portée de tir
	int shooting_rate; // portée de tir
	struct element* next; // Elément suivant
	struct element* missiles; // Liste chainée de missiles
} Element, *ElementList;

/* Fonction qui alloue la mémoire nécessaire pour un Element, initialise ses champs avec les valeurs x,y,r,g,b passées en paramètre et renvoie le pointeur vers cet espace mémoire */
Element* allocElement(int type, float x, float y, float speed_x, float speed_y, int shooting_range, int shooting_rate);

/* Fonction qui ajoute l'adresse d'un Element passé en paramètre à une liste chainée passée en paramètre sans valeur de retour */
void addElementToList(Element* element, ElementList* list);

/* Fonction qui supprime un élément d'une liste */
void removeElementFromList(Element* element, ElementList* list);

/* Vérifie si il n'y a pas de collisions entre les élémentsde deux listes différentes, si oui supprime l'élément de la 2ème liste */
int checkIntersections(ElementList list1, ElementList* list2);

/* Renvoie 1 si les deux éléments entrent en collision, 0 sinon */
int collided(Element a, Element b);

/* Fonction qui dessine les Elements de la liste passée en paramètre sans valeur de retour */
void drawElements(ElementList list);

/* Fonction qui vide la mémoire de tous les Elements de la liste passée en paramètre sans valeur de retour */
void deleteElements(ElementList* list);

#endif