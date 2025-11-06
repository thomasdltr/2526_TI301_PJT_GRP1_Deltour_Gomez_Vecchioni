#include <stdio.h>
#include "graph.h"

int main(void) {
    // Chemin du fichier de données.
    // Si CLion exécute dans cmake-build-debug, mets "../data/exemple1.txt"
    // Sinon, règle le "Working directory" sur le dossier projet et utilise "data/exemple1.txt".
    const char *path = "../data/exemple1.txt"; // adapte si besoin

    AdjList G = readGraph(path);

    printf("=== Affichage de la liste d'adjacence ===\n");
    adj_print(&G);

    adj_free(&G);
    return 0;
}
