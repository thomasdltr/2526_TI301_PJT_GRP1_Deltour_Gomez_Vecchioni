#include "hasse.h"

/*
   Supprime les liens transitifs du diagramme de Hasse
   → But : garder uniquement les relations directes entre classes.

   Si on a :
       A -> B
       B -> C
       A -> C   (transitif)
   Alors on supprime A -> C pour avoir un vrai diagramme de Hasse.
*/
void removeTransitiveLinks(t_link_array *p_link_array) {
    if (!p_link_array || p_link_array->size <= 2) return;

    int n = p_link_array->size;
    int to_remove[n];
    for (int i = 0; i < n; ++i) to_remove[i] = 0;   // marqueurs de suppression

    /*
       Triple boucle :
       - i = lien A -> B
       - j = lien B -> C
       - k = lien A -> C (si trouvé → à supprimer)
    */
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;

            // Condition : (A->B) et (B->C)
            if (p_link_array->data[i].to == p_link_array->data[j].from) {

                int inter = p_link_array->data[j].to; // C dans A->C
                for (int k = 0; k < n; ++k) {

                    // détection d’un lien A->C → inutile
                    if (p_link_array->data[k].from == p_link_array->data[i].from &&
                        p_link_array->data[k].to == inter) {

                        to_remove[k] = 1; // marquage pour suppression
                        }
                }
            }
        }
    }

    /* Compactage : on réécrit le tableau sans les liens marqués */
    int new_size = 0;
    for (int i = 0; i < n; ++i) {
        if (!to_remove[i]) {
            p_link_array->data[new_size++] = p_link_array->data[i];
        }
    }

    p_link_array->size = new_size; // mise à jour du nombre final de liens
}
