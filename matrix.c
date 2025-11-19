#include "matrix.h"

// ===============================
// Création d'une matrice n×n
// ===============================

float **matrix_create(int n) {
    float **M = malloc(n * sizeof(float *));
    if (!M) {
        perror("malloc matrix");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        M[i] = calloc(n, sizeof(float)); // initialise à 0
        if (!M[i]) {
            perror("calloc matrix row");
            exit(EXIT_FAILURE);
        }
    }

    return M;
}

// ===============================
// Libération mémoire
// ===============================

void matrix_free(float **M, int n) {
    if (!M) return;

    for (int i = 0; i < n; i++)
        free(M[i]);

    free(M);
}

// ===============================
// Copier une matrice
// ===============================

void matrix_copy(float **dest, float **src, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            dest[i][j] = src[i][j];
}

// ===============================
// Convertir la liste d'adjacence
// en matrice n×n
// ===============================

float **matrix_from_graph(const AdjList *G) {
    int n = G->n;
    float **M = matrix_create(n);

    for (int u = 1; u <= n; u++) {
        for (Cell *c = G->arr[u].head; c != NULL; c = c->next) {
            int v = c->dest;
            M[u - 1][v - 1] = c->prob;   // indices 0-based
        }
    }

    return M;
}

// ===============================
// Multiplication matricielle
// R = A × B
// ===============================

void matrix_mult(float **A, float **B, float **R, int n) {
    // initialiser R à 0
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            R[i][j] = 0.0f;

    // multiplication
    for (int i = 0; i < n; i++) {
        for (int k = 0; k < n; k++) {
            for (int j = 0; j < n; j++) {
                R[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// ===============================
// Calcul de diff(M, N)
// Somme des |M_ij - N_ij|
// ===============================

float matrix_diff(float **A, float **B, int n) {
    float d = 0.0f;

    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            d += fabsf(A[i][j] - B[i][j]);

    return d;
}

// ===============================
// Affichage d'une matrice
// ===============================

void matrix_print(float **M, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.4f ", M[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
// =============================================================
// Sous-matrice correspondant à une composante fortement connexe
// =============================================================
float **subMatrix(float **matrix, const TarjanPartition *part, int compo_index, int *out_n)
{
    // On récupère la classe composante
    if (compo_index < 0 || compo_index >= part->size) return NULL;

    TarjanClass cls = part->classes[compo_index];
    int k = cls.size;    // nombre d’états dans la composante

    // Renvoie la taille de la sous-matrice
    *out_n = k;

    // Allocation k × k
    float **S = matrix_create(k);

    // Remplissage de la sous-matrice :
    // On garde uniquement les lignes et colonnes appartenant à la composante
    for (int i = 0; i < k; i++) {
        int original_i = cls.members[i];   // numéro réel du sommet

        for (int j = 0; j < k; j++) {
            int original_j = cls.members[j];

            S[i][j] = matrix[original_i - 1][original_j - 1];
        }
    }

    return S;
}
