#ifndef TARJAN_H
#define TARJAN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "graph.h"   // pour AdjList (Partie 1)
#include "hasse.h"


// ---------- 1) Infos par sommet pour Tarjan ----------
typedef struct {
    int id;        // identifiant = numéro du sommet dans le graphe
    int index;     // "numéro" temporaire (initialement -1)
    int lowlink;   // "numéro accessible" (initialement -1)
    int on_stack;  // booléen (0/1) : est-il dans la pile ?
} TarjanVertex;

// crée le tableau [0..G->n] initialisé (index/lowlink = -1, on_stack = 0)
TarjanVertex* tarjan_init_vertices(const AdjList *G);
// libère le malloc du tableau de TarjanVertex (pointeur retourné par init)
void tarjan_free_vertices(TarjanVertex *arr);


// ---------- 2) Pile d'entiers (stocke des indices de sommets) ----------
typedef struct {
    int *data;
    int top;        // index du prochain emplacement libre
    int capacity;
} IntStack;

IntStack stack_create(int capacity);     // capacity initiale (ex: G->n)
void     stack_push(IntStack *S, int v);
int      stack_pop(IntStack *S);         // précondition: non vide
int      stack_peek(const IntStack *S);  // précondition: non vide
int      stack_empty(const IntStack *S); // 1 si vide, 0 sinon
void     stack_free(IntStack *S);


// ---------- 3) Classe (composante fortement connexe) ----------
typedef struct {
    char name[8];  // "C1", "C2", ...
    int  *members; // tableau dynamique d'identifiants de sommets
    int   size;
    int   capacity;
} TarjanClass;

TarjanClass class_create(const char *name);
void        class_add_member(TarjanClass *C, int vertex_id);
void        class_free(TarjanClass *C);
void        class_print(const TarjanClass *C);


// ---------- 4) Partition = ensemble de classes ----------
typedef struct {
    TarjanClass *classes; // tableau dynamique de classes
    int size;
    int capacity;
} TarjanPartition;

TarjanPartition partition_create(void);
void            partition_add_class(TarjanPartition *P, TarjanClass cls);
void            partition_free(TarjanPartition *P);
void            partition_print(const TarjanPartition *P);

// ---- Algorithme de Tarjan : renvoie la partition (SCC) ----
TarjanPartition tarjan_run(const AdjList *G);

TarjanPartition tarjan_run(const AdjList *G);

// ======================= Hasse (diagramme entre classes) =======================

// Associe chaque sommet à la classe à laquelle il appartient
int* build_vertex_to_class(const TarjanPartition *P, int n);

// Crée la liste des liens entre classes à partir du graphe et de la partition
void build_class_links(const AdjList *G, const TarjanPartition *P, t_link_array *links);

// Affiche les liens (debug)
void print_class_links(const t_link_array *links);

// Exporte le diagramme de Hasse au format Mermaid
void hasse_to_mermaid(const TarjanPartition *P, const t_link_array *links, const char *filename);



#endif // TARJAN_H

