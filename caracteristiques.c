#include "caracteristiques.h"

void printGraphCharacteristics(const TarjanPartition *P, const t_link_array *L) {

    printf("\n=== Etape 3 : Caracteristiques du graphe ===\n");

    int nbClasses = P->size;

    // Tableau indiquant si une classe a un lien sortant (=> transitoire)
    int *hasOutgoing = calloc(nbClasses, sizeof(int));

    // Parcours des liens pour marquer les classes qui pointent vers d'autres
    for (int i = 0; i < L->size; ++i) {
        int from = L->data[i].from - 1;   // index de la classe source
        if (from >= 0 && from < nbClasses)
            hasOutgoing[from] = 1;        // cette classe a un lien sortant
    }

    // Un graphe est irréductible s'il n’a qu’une seule classe
    int irreductible = (nbClasses == 1);

    printf("\nNombre de classes : %d\n", nbClasses);

    // Parcours de chaque classe
    for (int i = 0; i < nbClasses; ++i) {

        TarjanClass *C = &P->classes[i];

        // Si elle a un lien sortant → transitoire, sinon → persistante
        if (hasOutgoing[i]) {
            printf("Classe %s : transitoire (etats ", C->name);
        } else {
            printf("Classe %s : persistante (etats ", C->name);
        }

        // Affichage des membres de la classe
        for (int j = 0; j < C->size; ++j) {
            printf("%d", C->members[j]);
            if (j < C->size - 1) printf(",");
        }
        printf(")\n");

        // Si une classe persistante contient un seul état → état absorbant
        if (!hasOutgoing[i] && C->size == 1) {
            printf(" → L etat %d est absorbant.\n", C->members[0]);
        }
    }

    // Conclusion sur l’irréductibilité du graphe
    if (irreductible)
        printf("\nLe graphe de Markov est irreductible (une seule classe).\n");
    else
        printf("\nLe graphe de Markov n est pas irreductible.\n");

    free(hasOutgoing);
}
