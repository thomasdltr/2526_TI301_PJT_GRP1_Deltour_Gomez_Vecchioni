#include <stdio.h>
#include "graph.h"

int main(void) {
    const char *path = "../data/exemple3.txt"; // adapte si besoin

    AdjList G = readGraph(path);

    printf("=== Etape 1 : Affichage de la liste d'adjacence ===\n");
    adj_print(&G);

    printf( "\n=== Etape 2 : Verification du graphe de Markov ===\n");
    if (adj_is_markov(&G)) {
        printf("Le graphe est un graphe de Markov.\n");
    } else {
        printf("Le graphe n'est PAS un graphe de Markov.\n");
    }

    printf("\n=== Etape 3 : Generation du fichier Mermaid ===\n");
    adj_to_mermaid(&G, "graph_mermaid.txt");
    printf("Fichier 'graph_mermaid.txt' genere avec succes !\n");

    adj_free(&G);
    return 0;
}

