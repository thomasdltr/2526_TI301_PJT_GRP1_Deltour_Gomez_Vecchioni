#include "tarjan.h"

// ---------- TarjanVertex array ----------
TarjanVertex* tarjan_init_vertices(const AdjList *G) {
    if (!G || G->n <= 0) return NULL;
    // on alloue n+1 pour indexer de 1..n (on ignore l'indice 0)
    TarjanVertex *arr = (TarjanVertex*)malloc((G->n + 1) * sizeof(TarjanVertex));
    if (!arr) {
        perror("malloc TarjanVertex");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= G->n; ++i) {
        arr[i].id = i;
        arr[i].index = -1;
        arr[i].lowlink = -1;
        arr[i].on_stack = 0;
    }
    return arr;
}

void tarjan_free_vertices(TarjanVertex *arr) {
    free(arr);
}


// ---------- IntStack ----------
static void stack_grow(IntStack *S) {
    int newcap = (S->capacity < 4) ? 4 : (S->capacity * 2);
    int *nd = (int*)realloc(S->data, newcap * sizeof(int));
    if (!nd) {
        perror("realloc stack");
        exit(EXIT_FAILURE);
    }
    S->data = nd;
    S->capacity = newcap;
}

IntStack stack_create(int capacity) {
    IntStack S;
    if (capacity < 1) capacity = 4;
    S.data = (int*)malloc(capacity * sizeof(int));
    if (!S.data) {
        perror("malloc stack");
        exit(EXIT_FAILURE);
    }
    S.top = 0;
    S.capacity = capacity;
    return S;
}

void stack_push(IntStack *S, int v) {
    if (S->top >= S->capacity) stack_grow(S);
    S->data[S->top++] = v;
}

int stack_pop(IntStack *S) {
    if (S->top <= 0) {
        fprintf(stderr, "stack_pop on empty stack\n");
        exit(EXIT_FAILURE);
    }
    return S->data[--S->top];
}

int stack_peek(const IntStack *S) {
    if (S->top <= 0) {
        fprintf(stderr, "stack_peek on empty stack\n");
        exit(EXIT_FAILURE);
    }
    return S->data[S->top - 1];
}

int stack_empty(const IntStack *S) {
    return S->top == 0;
}

void stack_free(IntStack *S) {
    free(S->data);
    S->data = NULL;
    S->top = 0;
    S->capacity = 0;
}


// ---------- TarjanClass ----------
TarjanClass class_create(const char *name) {
    TarjanClass C;
    memset(&C, 0, sizeof(C));
    if (name) {
        // limite 7 chars + '\0' (ex: "C12345")
        strncpy(C.name, name, sizeof(C.name) - 1);
    } else {
        strcpy(C.name, "C?");
    }
    C.capacity = 0;
    C.members = NULL;
    C.size = 0;
    return C;
}

static void class_grow(TarjanClass *C) {
    int newcap = (C->capacity < 4) ? 4 : (C->capacity * 2);
    int *nm = (int*)realloc(C->members, newcap * sizeof(int));
    if (!nm) {
        perror("realloc class members");
        exit(EXIT_FAILURE);
    }
    C->members = nm;
    C->capacity = newcap;
}

void class_add_member(TarjanClass *C, int vertex_id) {
    if (C->size >= C->capacity) class_grow(C);
    C->members[C->size++] = vertex_id;
}

void class_free(TarjanClass *C) {
    free(C->members);
    C->members = NULL;
    C->size = 0;
    C->capacity = 0;
}

void class_print(const TarjanClass *C) {
    printf("Composante %s: {", C->name);
    for (int i = 0; i < C->size; ++i) {
        printf("%d", C->members[i]);
        if (i + 1 < C->size) printf(", ");
    }
    printf("}\n");
}


// ---------- TarjanPartition ----------
TarjanPartition partition_create(void) {
    TarjanPartition P;
    P.classes = NULL;
    P.size = 0;
    P.capacity = 0;
    return P;
}

static void partition_grow(TarjanPartition *P) {
    int newcap = (P->capacity < 4) ? 4 : (P->capacity * 2);
    TarjanClass *nc = (TarjanClass*)realloc(P->classes, newcap * sizeof(TarjanClass));
    if (!nc) {
        perror("realloc partition classes");
        exit(EXIT_FAILURE);
    }
    P->classes = nc;
    P->capacity = newcap;
}

void partition_add_class(TarjanPartition *P, TarjanClass cls) {
    if (P->size >= P->capacity) partition_grow(P);
    P->classes[P->size++] = cls; // copie "shallow" (les members restent possédés par cls)
}

void partition_free(TarjanPartition *P) {
    if (!P || !P->classes) {
        P->size = P->capacity = 0;
        return;
    }
    for (int i = 0; i < P->size; ++i) {
        class_free(&P->classes[i]);
    }
    free(P->classes);
    P->classes = NULL;
    P->size = 0;
    P->capacity = 0;
}

void partition_print(const TarjanPartition *P) {
    for (int i = 0; i < P->size; ++i) {
        class_print(&P->classes[i]);
    }

}


// ================== TARJAN (DFS) ==================
static void tarjan_dfs(int u,
                       const AdjList *G,
                       TarjanVertex *V,
                       IntStack *S,
                       int *pIndex,
                       TarjanPartition *P)
{
    V[u].index   = *pIndex;
    V[u].lowlink = *pIndex;
    (*pIndex)++;

    stack_push(S, u);
    V[u].on_stack = 1;

    // Parcourt tous les voisins v de u
    for (Cell *e = G->arr[u].head; e != NULL; e = e->next) {
        int v = e->dest;
        if (V[v].index == -1) {
            // non visité
            tarjan_dfs(v, G, V, S, pIndex, P);
            if (V[v].lowlink < V[u].lowlink) V[u].lowlink = V[v].lowlink;
        } else if (V[v].on_stack) {
            // arête vers un sommet dans la pile
            if (V[v].index < V[u].lowlink) V[u].lowlink = V[v].index;
        }
    }

    // u est racine d'une composante ?
    if (V[u].lowlink == V[u].index) {
        // on dépile jusqu'à retrouver u
        char name[8];
        snprintf(name, sizeof(name), "C%d", P->size + 1);
        TarjanClass C = class_create(name);

        while (!stack_empty(S)) {
            int w = stack_pop(S);
            V[w].on_stack = 0;
            class_add_member(&C, w);
            if (w == u) break;
        }
        partition_add_class(P, C);
    }
}

TarjanPartition tarjan_run(const AdjList *G)
{
    TarjanPartition P = partition_create();
    if (!G || G->n <= 0) return P;

    TarjanVertex *V = tarjan_init_vertices(G);
    IntStack S = stack_create(G->n);

    int index = 0;
    for (int u = 1; u <= G->n; ++u) {
        if (V[u].index == -1) {
            tarjan_dfs(u, G, V, &S, &index, &P);
        }
    }

    // Nettoyage des structures temporaires
    stack_free(&S);
    tarjan_free_vertices(V);
    return P; // (les classes/members sont possédés par P)
}

// ============================================================================
//  Hasse - Construction du diagramme entre classes (CFC)
// ============================================================================

// Vérifie si un lien (from->to) existe déjà
static int link_exists(const t_link_array *L, int from, int to) {
    for (int i = 0; i < L->size; ++i) {
        if (L->data[i].from == from && L->data[i].to == to) return 1;
    }
    return 0;
}

// map[v] = index de la classe dans la partition
int* build_vertex_to_class(const TarjanPartition *P, int n) {
    int *map = malloc((n + 1) * sizeof(int));
    if (!map) { perror("malloc map vertex->class"); exit(EXIT_FAILURE); }
    for (int i = 0; i <= n; ++i) map[i] = -1;

    for (int ci = 0; ci < P->size; ++ci) {
        const TarjanClass *C = &P->classes[ci];
        for (int k = 0; k < C->size; ++k) {
            int v = C->members[k];
            if (v >= 1 && v <= n) map[v] = ci;
        }
    }
    return map;
}

// Ajoute un lien dans la structure dynamique
static void push_link(t_link_array *L, int a, int b) {
    if (L->size >= L->capacity) {
        int nc = (L->capacity < 8) ? 8 : L->capacity * 2;
        L->data = realloc(L->data, nc * sizeof(t_link));
        if (!L->data) { perror("realloc links"); exit(EXIT_FAILURE); }
        L->capacity = nc;
    }
    L->data[L->size].from = a;
    L->data[L->size].to = b;
    L->size++;
}

// Crée tous les liens entre classes
void build_class_links(const AdjList *G, const TarjanPartition *P, t_link_array *links) {
    links->data = NULL;
    links->size = 0;
    links->capacity = 0;

    int *v2c = build_vertex_to_class(P, G->n);

    for (int u = 1; u <= G->n; ++u) {
        int ci = v2c[u];
        for (Cell *e = G->arr[u].head; e != NULL; e = e->next) {
            int v = e->dest;
            int cj = v2c[v];
            if (ci != cj && ci >= 0 && cj >= 0) {
                if (!link_exists(links, ci, cj))
                    push_link(links, ci, cj);
            }
        }
    }

    free(v2c);
}

// Affiche les liens entre classes
void print_class_links(const t_link_array *links) {
    for (int i = 0; i < links->size; ++i)
        printf("Lien C%d -> C%d\n", links->data[i].from + 1, links->data[i].to + 1);
}

// Écrit une classe au format {1,7,5}
static void write_class_label(FILE *f, const TarjanClass *C) {
    fprintf(f, "{");
    for (int i = 0; i < C->size; ++i) {
        fprintf(f, "%d", C->members[i]);
        if (i + 1 < C->size) fprintf(f, ",");
    }
    fprintf(f, "}");
}

// Exporte le diagramme de Hasse en format Mermaid
void hasse_to_mermaid(const TarjanPartition *P, const t_link_array *links, const char *filename) {
    FILE *f = fopen(filename, "wt");
    if (!f) { perror("open mermaid hasse"); exit(EXIT_FAILURE); }

    fprintf(f, "---\nconfig:\n  layout: elk\n  theme: neo\n  look: neo\n---\n\n");
    fprintf(f, "flowchart TB\n");

    // Nœuds (une box par classe)
    for (int i = 0; i < P->size; ++i) {
        fprintf(f, "C%d[\"", i + 1);
        write_class_label(f, &P->classes[i]);
        fprintf(f, "\"]\n");
    }

    fprintf(f, "\n");
    // Liens
    for (int i = 0; i < links->size; ++i) {
        fprintf(f, "C%d --> C%d\n", links->data[i].from + 1, links->data[i].to + 1);
    }

    fclose(f);
}




