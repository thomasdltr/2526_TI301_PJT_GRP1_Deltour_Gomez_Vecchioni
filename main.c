#include <stdio.h>
#include "graph.h"

int main(void) {
    const char *path = "../data/exemple1.txt"; // adapte si besoin

    AdjList G = readGraph(path);

    printf("=== Étape 1 : Affichage de la liste d'adjacence ===\n");
    adj_print(&G);

    printf("\n=== Étape 2 : Vérification du graphe de Markov ===\n");
    if (adj_is_markov(&G)) {
        printf("Le graphe est un graphe de Markov.\n");
    } else {
        printf("Le graphe n'est PAS un graphe de Markov.\n");
    }

    printf("\n=== Étape 3 : Génération du fichier Mermaid ===\n");
    adj_to_mermaid(&G, "graph_mermaid.txt");
    printf("Fichier 'graph_mermaid.txt' généré avec succès !\n");

    adj_free(&G);
    return 0;
}
