#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Representa um nó na lista encadeada de adjacência
typedef struct st_no_adj {
    int info_adjacente;
    struct st_no_adj *prox;
} no_adj;

// Representa o vértice em si
typedef struct st_vertice {
    char cor;
    int grau;
    no_adj *inicio;
} vertice;

// Estrutura principal do grafo
typedef struct st_grafo {
    int num_vertices;
    vertice *vertices;
} grafo;


grafo *init_grafo(int n) {
    if (n <= 0) return NULL;

    grafo *g = malloc (sizeof(grafo));
    if (g == NULL) return NULL;


    g -> num_vertices = n;

    g -> vertices = malloc (n * sizeof(vertice));
    if (g -> vertices == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g -> vertices[i].grau = 0;
        g -> vertices[i].cor = 0;
        g -> vertices[i].inicio = NULL;
    }

    return g;
}

void insere_no_inicio(vertice *v, int info_novo_adjacente) {
    no_adj *novo = malloc(sizeof(no_adj));
    if (novo) {
        novo -> info_adjacente = info_novo_adjacente;
        novo -> prox = v -> inicio;
        v -> inicio = novo;
        v -> grau++;
    }
}

void add_aresta(const grafo *g, int u, int v) {
    insere_no_inicio(&g -> vertices[u], v);
    insere_no_inicio(&g -> vertices[v], u);
}

void liberar_grafo(grafo *g) {
    if (g == NULL) return;

    for (int i = 0; i < g -> num_vertices; i++) {

        no_adj *atual = g -> vertices[i].inicio;

        while (atual != NULL) {
            no_adj *aux = atual;
            atual = atual -> prox;
            free(aux);
        }
    }

    free(g -> vertices);
    free(g);
}

bool pode_pintar(const grafo *g, const int u, const char cor_atual) {
    if (g == NULL) return false;

    no_adj *atual = g -> vertices[u].inicio;

    while (atual != NULL) {
        int info_atual_adjacente = atual -> info_adjacente;
        char cor_adjacente = g -> vertices[info_atual_adjacente].cor;

        if (cor_adjacente != 0 && cor_adjacente == cor_atual) {
            return false;
        }

        atual = atual -> prox;
    }

    return true;
}

int get_num_vertices(const grafo *g) {
    return g -> num_vertices;
}

// Dados para a ordenação de vértices por ordem decrescente de grau
typedef struct {
    int id_atual;
    int grau;
} elemento_ordenacao;

// Função para comparar os graus em ordem decrescente
int comparar_graus(const void *a, const void *b) {
    elemento_ordenacao *e1 = (elemento_ordenacao*)a;
    elemento_ordenacao *e2 = (elemento_ordenacao*)b;
    return e2 -> grau - e1 -> grau;
}

// Lista global de cores disponíveis
char lista_cores[] = {'A', 'B', 'C', 'D', 'E', 'F'};

// Algoritmo Welsh & Powell para coloração de grafos (heurística)
int Welsh_Powell(const grafo *g) {
    if (g == NULL || g -> vertices == NULL) return -1;

    int n = get_num_vertices(g);
    elemento_ordenacao *lista_ordenacao = malloc(n * sizeof(elemento_ordenacao));

    for (int i = 0; i < n; i++) {
        lista_ordenacao[i].id_atual = i;
        lista_ordenacao[i].grau = g -> vertices[i].grau;
    }

    qsort(lista_ordenacao, n, sizeof(elemento_ordenacao), comparar_graus);

    int cor_atual_index = 0;
    int vertices_coloridos = 0;

    while (vertices_coloridos < n) {

        char cor_atual = lista_cores[cor_atual_index];

        for (int i = 0; i < n; i++) {
            int u = lista_ordenacao[i].id_atual;

            if (g -> vertices[u].cor != 0) continue;

            if (pode_pintar(g, u, cor_atual)) {
                g -> vertices[u].cor = cor_atual;
                vertices_coloridos++;
            }
        }

        cor_atual_index++;
    }

    free(lista_ordenacao);

    return cor_atual_index;
}

void print_resultado(const grafo *g) {
    if (g == NULL) return;

    printf("\nResultado da coloração:\n");
    for (int i = 0; i < g -> num_vertices; i++) {
        printf("Vertice %d (Grau %d) -> Cor: %c\n",
            i, g -> vertices[i].grau, g -> vertices[i].cor);
    }
}

void gerar_imagem_grafo(grafo *g);

int main() {
    int numero_de_vertices = 12;
    grafo *g = init_grafo(numero_de_vertices);

    printf("Gerando grafo com %d vertices...\n", numero_de_vertices);

    add_aresta(g, 0, 1);
    add_aresta(g, 0, 2);
    add_aresta(g, 0, 3);
    add_aresta(g, 0, 4);
    add_aresta(g, 4, 5);
    add_aresta(g, 5, 6);
    add_aresta(g, 6, 7);
    add_aresta(g, 6, 8);
    add_aresta(g, 8, 9);
    add_aresta(g, 4, 7);


    printf("Executando algoritmo de coloração...\n");
    int numero_cromatico = Welsh_Powell(g);
    print_resultado(g);
    printf("\nNúmero cromático do grafo com %i vértices: %i\n", numero_de_vertices, numero_cromatico);

    gerar_imagem_grafo(g);

    liberar_grafo(g);
    return 0;
}



void gerar_imagem_grafo(grafo *g) {
    if (g == NULL) return;

    FILE *txt = fopen("dados_grafo.txt", "w");
    if (txt == NULL) {
        printf("Erro ao criar arquivo de dados para o Python.\n");
        return;
    }

    for (int i = 0; i < g -> num_vertices; i++) {
        char c = (g -> vertices[i].cor == 0) ? '0' : g->vertices[i].cor;
        fprintf(txt, "%d %c\n", i, c);
    }

    fprintf(txt, "ARESTAS\n");
    for (int i = 0; i < g -> num_vertices; i++) {
        no_adj *atual = g -> vertices[i].inicio;
        while (atual != NULL) {
            fprintf(txt, "%d %d\n", i, atual -> info_adjacente);
            atual = atual->prox;
        }
    }

    fclose(txt);

    printf("\n[C] Dados exportados. Chamando script Python...\n");

    system("python plot_grafo.py");
}
