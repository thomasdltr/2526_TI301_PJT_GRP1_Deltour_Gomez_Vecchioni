#include "caracteristiques.h"

void printGraphCharacteristics(const TarjanPartition *P, const t_link_array *L) {
    printf("\n=== Étape 3 : Caractéristiques du graphe ===\n");

    int nbClasses = P->size;
    int *hasOutgoing = calloc(nbClasses, sizeof(int)); // 1 si la classe a un lien sortant

    // Parcours des liens pour repérer les classes transitoires
    for (int i = 0; i < L->size; ++i) {
        int from = L->data[i].from - 1;
        if (from >= 0 && from < nbClasses)
            hasOutgoing[from] = 1;
    }

    int irreductible = (nbClasses == 1);
    printf("\nNombre de classes : %d\n", nbClasses);

    for (int i = 0; i < nbClasses; ++i) {
        TarjanClass *C = &P->classes[i];
        if (hasOutgoing[i]) {
            printf("Classe %s : transitoire (états ", C->name);
        } else {
            printf("Classe %s : persistante (états ", C->name);
        }

        for (int j = 0; j < C->size; ++j) {
            printf("%d", C->members[j]);
            if (j < C->size - 1) printf(",");
        }
        printf(")\n");

        // Si elle est persistante et contient un seul état → absorbante
        if (!hasOutgoing[i] && C->size == 1) {
            printf(" → L’état %d est absorbant.\n", C->members[0]);
        }
    }

    if (irreductible)
        printf("\nLe graphe de Markov est irréductible (une seule classe).\n");
    else
        printf("\nLe graphe de Markov n’est pas irréductible.\n");

    free(hasOutgoing);
}
