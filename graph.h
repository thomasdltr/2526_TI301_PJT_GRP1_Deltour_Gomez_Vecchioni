#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Maillon d’une liste
typedef struct Cell {
    int dest;              // sommet d’arrivée
    float prob;            // probabilité de transition
    struct Cell *next;     // maillon suivant
} Cell;

// Liste d’adjacence pour un sommet
typedef struct {
    Cell *head;            // tête de la liste
} List;

// Graphe orienté sous forme de liste d’adjacence
typedef struct {
    int n;                 // nombre de sommets
    List *arr;             // tableau de n listes
} AdjList;

// Création et manipulation des listes
Cell*  make_cell(int dest, float prob);
List   make_list(void);
void   list_push_front(List *L, int dest, float prob);
void   list_print(const List *L);

// Création et manipulation de la liste d’adjacence
AdjList adj_create(int n);                      // crée n listes vides
void    adj_add_edge(AdjList *G, int u, int v, float p); // ajoute arête
void    adj_print(const AdjList *G);            // affichage format demandé
void    adj_free(AdjList *G);                   // libération mémoire

// Lecture depuis un fichier texte (format du sujet)
AdjList readGraph(const char *filename);

// Vérifie que la somme des probabilités sortantes de chaque sommet ≈ 1
bool adj_is_markov(const AdjList *G);


// Convertit un numéro de sommet (1,2,3,...) en identifiant (A,B,C,...,AA,...)
char *getId(int num);

// Produit un fichier texte Mermaid pour visualiser le graphe
void adj_to_mermaid(const AdjList *G, const char *filename);

#endif // GRAPH_H
