#ifndef HASSE_H
#define HASSE_H

#include <stdio.h>
#include <stdlib.h>

// Structure d’un lien (entre deux classes)
typedef struct {
    int from; // classe de départ
    int to;   // classe d’arrivée
} t_link;

// Tableau dynamique de liens
typedef struct {
    t_link *data;
    int size;
    int capacity;
} t_link_array;

// Fonction (optionnelle) pour retirer les liens transitifs
void removeTransitiveLinks(t_link_array *p_link_array);

#endif // HASSE_H
