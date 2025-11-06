#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------- structures ------- */

typedef struct Cell {
    int dest;              // sommet d’arrivée
    float prob;            // probabilité
    struct Cell *next;     // maillon suivant
} Cell;

typedef struct {
    Cell *head;            // tête de la liste (arêtes sortantes d’un sommet)
} List;

typedef struct {
    int n;                 // nombre de sommets (1..n)
    List *arr;             // tableau de n listes (arr[1] ... arr[n])
} AdjList;

/* ------- API étape 1 ------- */

// création/gestion des listes
Cell*  make_cell(int dest, float prob);
List   make_list(void);
void   list_push_front(List *L, int dest, float prob);
void   list_print(const List *L);

// création/affichage de la liste d’adjacence
AdjList adj_create(int n);                 // crée n listes vides
void    adj_add_edge(AdjList *G, int u, int v, float p); // ajoute arête u->v (p)
void    adj_print(const AdjList *G);       // affichage format demandé
void    adj_free(AdjList *G);              // libération mémoire

// lecture depuis un fichier texte (format du sujet)
AdjList readGraph(const char *filename);

#endif // GRAPH_H
