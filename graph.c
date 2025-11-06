#include "graph.h"

/* ------------ outils listes ------------ */

Cell* make_cell(int dest, float prob) {
    Cell *c = (Cell*)malloc(sizeof(Cell));
    if (!c) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    c->dest = dest;
    c->prob = prob;
    c->next = NULL;
    return c;
}

List make_list(void) {
    List L;
    L.head = NULL;
    return L;
}

void list_push_front(List *L, int dest, float prob) {
    Cell *c = make_cell(dest, prob);
    c->next = L->head;
    L->head = c;
}

void list_print(const List *L) {
    const Cell *cur = L->head;
    printf("[head @]");
    while (cur) {
        printf(" -> (%d, %.2f)", cur->dest, cur->prob);
        cur = cur->next;
    }
    printf("\n");
}

/* ----------- liste d’adjacence ---------- */

AdjList adj_create(int n) {
    AdjList G;
    G.n = n;
    // on alloue n+1 pour indexer de 1 à n
    G.arr = (List*)malloc((n + 1) * sizeof(List));
    if (!G.arr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= n; ++i) G.arr[i] = make_list();
    return G;
}

void adj_add_edge(AdjList *G, int u, int v, float p) {
    if (u < 1 || u > G->n || v < 1 || v > G->n) {
        fprintf(stderr, "Edge out of bounds: %d -> %d\n", u, v);
        exit(EXIT_FAILURE);
    }
    // on ajoute en tête: ordre affiché = inverse d’insertion (comme dans l’exemple du sujet)
    list_push_front(&G->arr[u], v, p);
}

void adj_print(const AdjList *G) {
    for (int u = 1; u <= G->n; ++u) {
        printf("Liste pour le sommet %d:", u);
        list_print(&G->arr[u]);
    }
}

void adj_free(AdjList *G) {
    if (!G || !G->arr) return;
    for (int u = 1; u <= G->n; ++u) {
        Cell *cur = G->arr[u].head;
        while (cur) {
            Cell *nxt = cur->next;
            free(cur);
            cur = nxt;
        }
    }
    free(G->arr);
    G->arr = NULL;
    G->n = 0;
}

/* --------------- lecture fichier --------------- */

AdjList readGraph(const char *filename) {
    FILE *file = fopen(filename, "rt"); // read-only, text
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

    AdjList G = adj_create(nbvert);

    int depart, arrivee;
    float proba;

    // pour chaque ligne: depart arrivee proba
    while (fscanf(file, "%d %d %f", &depart, &arrivee, &proba) == 3) {
        adj_add_edge(&G, depart, arrivee, proba);
    }

    fclose(file);
    return G;
}
