#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graph.h"
#include "tarjan.h"


// =====================
//   Création / Free
// =====================

// Crée une matrice n×n initialisée à 0
float **matrix_create(int n);

// Libère une matrice
void matrix_free(float **M, int n);

// Copie une matrice source → destination
void matrix_copy(float **dest, float **src, int n);

// =====================
//   Opérations
// =====================

// Convertit un graphe en matrice de probabilités (n×n)
float **matrix_from_graph(const AdjList *G);

// Multiplication matricielle R = A × B
void matrix_mult(float **A, float **B, float **R, int n);

// Différence absolue entre deux matrices
float matrix_diff(float **A, float **B, int n);

// Affichage pour debug
void matrix_print(float **M, int n);
// =========================================================
// Sous-matrice correspondant à une composante fortement connexe
// =========================================================
float **subMatrix(float **matrix, const TarjanPartition *part, int compo_index, int *out_n);

#endif
