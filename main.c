#include <stdio.h>
#include "graph.h"
#include "tarjan.h"
#include "caracteristiques.h"
#include "matrix.h"

int main(void) {

    const char *path = "../data/exemple_scc1.txt";

    printf("=========================================\n");
    printf("        PROJET - Graphes de Markov        \n");
    printf("=========================================\n\n");

    //--------------------------------------------------
    // PARTIE 1 : Graphes de Markov
    //--------------------------------------------------
    printf("=== Partie 1 : Analyse du graphe ===\n");

    AdjList G = readGraph(path);

    printf("\n1) Liste d'adjacence :\n");
    adj_print(&G);

    printf("\n2) Vérification Markov :\n");
    if (adj_is_markov(&G))
        printf("→ Le graphe est un graphe de Markov.\n");
    else
        printf("→ Le graphe n'est PAS un graphe de Markov.\n");

    printf("\n3) Export Mermaid du graphe...\n");
    adj_to_mermaid(&G, "graph_mermaid.txt");
    printf("→ Fichier 'graph_mermaid.txt' généré.\n");

    //--------------------------------------------------
    // PARTIE 2 : Composantes fortement connexes
    //--------------------------------------------------
    printf("\n=== Partie 2 : Composantes fortement connexes ===\n");

    TarjanPartition P = tarjan_run(&G);
    partition_print(&P);

    printf("\n4) Diagramme de Hasse :\n");

    t_link_array L;
    build_class_links(&G, &P, &L);
    print_class_links(&L);

    removeTransitiveLinks(&L);
    hasse_to_mermaid(&P, &L, "hasse_mermaid.txt");
    printf("→ Fichier 'hasse_mermaid.txt' généré.\n");

    printf("\n5) Caractéristiques du graphe :\n");
    printGraphCharacteristics(&P, &L);

    //--------------------------------------------------
    // PARTIE 3 : Matrices de transition
    //--------------------------------------------------
    printf("\n=== Partie 3 : Matrices du graphe ===\n");

    // Construire la matrice M
    float **M = matrix_from_graph(&G);
    printf("\nMatrice M :\n");
    matrix_print(M, G.n);

    // M³
    float **M2 = matrix_create(G.n);
    float **M3 = matrix_create(G.n);

    matrix_mult(M, M, M2, G.n);
    matrix_mult(M2, M, M3, G.n);

    printf("M^3 :\n");
    matrix_print(M3, G.n);

    // M⁷
    float **tmp = matrix_create(G.n);
    matrix_copy(tmp, M3, G.n);

    for (int i = 4; i <= 7; i++) {
        float **next = matrix_create(G.n);
        matrix_mult(tmp, M, next, G.n);
        matrix_free(tmp, G.n);
        tmp = next;
    }

    printf("M^7 :\n");
    matrix_print(tmp, G.n);

    // Convergence
    printf("\n=== Test de convergence ===\n");

    float **A = matrix_create(G.n);
    float **B = matrix_create(G.n);
    matrix_copy(A, M, G.n);

    int n_iter = 0;

    while (1) {
        matrix_mult(A, M, B, G.n);
        float d = matrix_diff(A, B, G.n);

        if (d < 0.01f) {
            printf("→ Convergence atteinte après %d itérations (diff = %.4f)\n", n_iter, d);
            break;
        }

        matrix_copy(A, B, G.n);
        n_iter++;

        if (n_iter > 1000) {
            printf("→ Pas de convergence trouvée.\n");
            break;
        }
    }

    printf("\nM^n (limite) :\n");
    matrix_print(B, G.n);

    //--------------------------------------------------
    // LIBÉRATION MÉMOIRE
    //--------------------------------------------------

    free(L.data);
    partition_free(&P);
    adj_free(&G);

    matrix_free(M, G.n);
    matrix_free(M2, G.n);
    matrix_free(M3, G.n);
    matrix_free(tmp, G.n);
    matrix_free(A, G.n);
    matrix_free(B, G.n);

    printf("\n=== Fin du programme ===\n");

    return 0;
}
