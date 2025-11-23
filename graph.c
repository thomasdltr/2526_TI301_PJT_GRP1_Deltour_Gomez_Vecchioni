#include "graph.h"
#include <string.h>
#include <math.h>

/* Créé une cellule (un arc) dans la liste d’adjacence */
Cell* make_cell(int dest, float prob) {
    Cell *c = (Cell*)malloc(sizeof(Cell));
    if (!c) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    c->dest = dest;   // sommet d’arrivée
    c->prob = prob;   // probabilité de transition
    c->next = NULL;   // pointeur sur l’élément suivant
    return c;
}

/* Créé une liste vide */
List make_list(void) {
    List L;
    L.head = NULL;
    return L;
}

/* Ajoute une cellule en tête de liste (opération push-front classique) */
void list_push_front(List *L, int dest, float prob) {
    Cell *c = make_cell(dest, prob);
    c->next = L->head;
    L->head = c;
}

/* Affiche une liste d’adjacence d’un sommet */
void list_print(const List *L) {
    const Cell *cur = L->head;
    printf("[head @]");
    while (cur) {
        printf(" -> (%d, %.2f)", cur->dest, cur->prob);
        cur = cur->next;
    }
    printf("\n");
}

/* Création d’un graphe avec n sommets (indexés de 1 à n) */
AdjList adj_create(int n) {
    AdjList G;
    G.n = n;

    // allocation d’un tableau de listes (1 par sommet)
    G.arr = (List*)malloc((n + 1) * sizeof(List));
    if (!G.arr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // initialisation des listes
    for (int i = 0; i <= n; ++i)
        G.arr[i] = make_list();

    return G;
}

/* Ajout d’un arc u -> v avec probabilité p */
void adj_add_edge(AdjList *G, int u, int v, float p) {
    if (u < 1 || u > G->n || v < 1 || v > G->n) {
        fprintf(stderr, "Edge out of bounds: %d -> %d\n", u, v);
        exit(EXIT_FAILURE);
    }
    list_push_front(&G->arr[u], v, p);
}

/* Affiche la liste d’adjacence complète du graphe */
void adj_print(const AdjList *G) {
    for (int u = 1; u <= G->n; ++u) {
        printf("Liste pour le sommet %d:", u);
        list_print(&G->arr[u]);
    }
}

/* Libère toute la mémoire du graphe */
void adj_free(AdjList *G) {
    if (!G || !G->arr) return;

    for (int u = 1; u <= G->n; ++u) {
        Cell *cur = G->arr[u].head;
        while (cur) {
            Cell *nxt = cur->next;
            free(cur);  // libère chaque cellule
            cur = nxt;
        }
    }

    free(G->arr);   // libère le tableau de listes
    G->arr = NULL;
    G->n = 0;
}

/* Lit un fichier texte et construit le graphe correspondant */
AdjList readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt");
    if (file == NULL) {
        perror("Could not open file for reading");
        exit(EXIT_FAILURE);
    }

    int nbvert;
    if (fscanf(file, "%d", &nbvert) != 1) {
        perror("Could not read number of vertices");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // création du graphe
    AdjList G = adj_create(nbvert);

    int depart, arrivee;
    float proba;

    // lecture des arcs du fichier
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        adj_add_edge(&G, depart, arrivee, proba);
    }

    fclose(file);
    return G;
}

/* Vérifie si chaque ligne du graphe suit les règles d’une distribution de probas */
bool adj_is_markov(const AdjList *G) {
    bool is_ok = true;

    for (int u = 1; u <= G->n; ++u) {
        float sum = 0.0f;

        // somme des probabilités sortantes de u
        const Cell *cur = G->arr[u].head;
        while (cur) {
            sum += cur->prob;
            cur = cur->next;
        }

        // tolérance de ±1%
        if (sum < 0.99f || sum > 1.01f) {
            printf("Sommet %d : somme = %.2f (non valide)\n", u, sum);
            is_ok = false;
        }
    }

    return is_ok;
}

/* Convertit un entier en identifiant style A, B, C, AA, AB… (pour Mermaid) */
char *getId(int num) {
    static char buffer[8];
    int i = 0;
    char temp[8];
    num--; // passe en base 0

    // conversion en lettres
    while (num >= 0) {
        temp[i++] = 'A' + (num % 26);
        num = num / 26 - 1;
    }

    // on remet dans le bon sens
    for (int j = 0; j < i; ++j)
        buffer[j] = temp[i - j - 1];

    buffer[i] = '\0';
    return buffer;
}

/* Génère un fichier Mermaid pour visualiser le graphe */
void adj_to_mermaid(const AdjList *G, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) {
        perror("Could not open file for writing");
        exit(EXIT_FAILURE);
    }

    // configuration Mermaid
    fprintf(f, "---\n");
    fprintf(f, "config:\n");
    fprintf(f, "   layout: elk\n");
    fprintf(f, "   theme: neo\n");
    fprintf(f, "   look: neo\n");
    fprintf(f, "---\n\n");
    fprintf(f, "flowchart LR\n");

    // déclaration des sommets
    for (int u = 1; u <= G->n; ++u)
        fprintf(f, "%s((%d))\n", getId(u), u);

    fprintf(f, "\n");

    // déclaration des arcs
    for (int u = 1; u <= G->n; ++u) {
        const Cell *cur = G->arr[u].head;
        while (cur) {
            fprintf(f, "%s -->|%.2f|%s\n",
                    getId(u), cur->prob, getId(cur->dest));
            cur = cur->next;
        }
    }

    fclose(f);
}
