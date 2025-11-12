#include "hasse.h"

// Fonction pour retirer les liens transitifs dans le diagramme de Hasse
// (optionnelle, pour simplifier le graphe)
void removeTransitiveLinks(t_link_array *p_link_array) {
    if (!p_link_array || p_link_array->size <= 2) return;

    int n = p_link_array->size;
    int to_remove[n];
    for (int i = 0; i < n; ++i) to_remove[i] = 0;

    // Si on a A->B et B->C, on supprime A->C
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            if (p_link_array->data[i].to == p_link_array->data[j].from) {
                int inter = p_link_array->data[j].to;
                for (int k = 0; k < n; ++k) {
                    if (p_link_array->data[k].from == p_link_array->data[i].from &&
                        p_link_array->data[k].to == inter) {
                        to_remove[k] = 1;
                        }
                }
            }
        }
    }

    // Compactage
    int new_size = 0;
    for (int i = 0; i < n; ++i) {
        if (!to_remove[i]) {
            p_link_array->data[new_size++] = p_link_array->data[i];
        }
    }
    p_link_array->size = new_size;
}
