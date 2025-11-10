#include <stdio.h>
#include "graph.h"
#include "tarjan.h"

int main(void) {
    const char *path = "../data/exemple_scc1.txt"; // adapte si besoin (../ ou data/)

    // === Étape 1 : Création et affichage du graphe ===
    AdjList G = readGraph(path);

    printf("=== Étape 1 : Affichage de la liste d'adjacence ===\n");
    adj_print(&G);

    // === Étape 2 : Vérification du graphe de Markov ===
    printf("\n=== Étape 2 : Vérification du graphe de Markov ===\n");
    if (adj_is_markov(&G)) {
        printf("Le graphe est un graphe de Markov.\n");
    } else {
        printf("Le graphe n'est PAS un graphe de Markov.\n");
    }

    // === Étape 3 : Génération du fichier Mermaid ===
    printf("\n=== Étape 3 : Génération du fichier Mermaid ===\n");
    adj_to_mermaid(&G, "graph_mermaid.txt");
    printf("Fichier 'graph_mermaid.txt' généré avec succès !\n");

    // === Partie 2 : Composantes fortement connexes (Tarjan) ===
    printf("\n=== Partie 2 : Composantes fortement connexes (Tarjan) ===\n");
    TarjanPartition P = tarjan_run(&G);
    partition_print(&P);          // ex: Composante C1: {1,2,3,4}

    // Libération mémoire
    partition_free(&P);
    adj_free(&G);

    return 0;
}


