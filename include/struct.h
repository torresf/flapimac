#ifndef STRUCT_H
    #define STRUCT_H

#include <stdlib.h>
#include <stdio.h>
#include <GL/gl.h>

typedef struct element {
    int type; // Type de l'élément 
    float x, y; // Coordonnées de l'élément 
    struct element* next;  // Next Element
} Element, *ElementList;

/* Fonction qui alloue la mémoire nécessaire pour un Element, initialise ses champs avec les valeurs x,y,r,g,b passées en paramètre et renvoie le Elementeur vers cet espace mémoire */
Element* allocElement(int type, float x, float y);

/* Fonction qui ajoute l'adresse d'un Element passé en paramètre à une liste chainée passée en paramètre sans valeur de retour */
void addElementToList(Element* element, ElementList* list);

/* Fonction qui dessine les Elements de la liste passée en paramètre sans valeur de retour */
void drawElements(ElementList list);

/* Fonction qui vide la mémoire de tous les Elements de la liste passée en paramètre sans valeur de retour */
void deleteElements(ElementList* list);

#endif