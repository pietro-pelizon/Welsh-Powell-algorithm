#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Representa um nó na lista encadeada de vizinhos
typedef struct st_no_adj {

    // Índice do vizinho no vetor principal
    int info_node;

    // Ponteiro para o próximo vizinho da lista de adjacência
    struct st_no_adj *prox;
} no_adj;

// Representa o vértice em si
typedef struct st_vertice {

    // Cor atribuída ao vértice
    char cor;

    // Número de conexões que o vértice faz
    int grau;

    // Ponteiro para o início da lista de adjacência
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

void add_aresta(grafo *g, const int u, const int v) {

    no_adj *adj1 = malloc (sizeof(no_adj));
    if (adj1 == NULL) return;

    adj1 -> info_node = v;
    adj1 -> prox = g -> vertices[u].inicio;
    g -> vertices[u].inicio = adj1;
    g -> vertices[u].grau++;

    no_adj *adj2 = malloc (sizeof(no_adj));
    if (adj2 == NULL) return;

    adj2 -> info_node = u;
    adj2 -> prox = g -> vertices[v].inicio;
    g -> vertices[v].inicio = adj2;
    g -> vertices[v].grau++;

}

bool pode_pintar(const grafo *g, const int u, const char cor_desejada) {
    if (g == NULL) return false;

    no_adj *atual = g -> vertices[u].inicio;

    while (atual != NULL) {
        int id_do_vizinho = atual -> info_node;
        char cor_vizinho = g -> vertices[id_do_vizinho].cor;

        if (cor_vizinho != 0 && cor_vizinho == cor_desejada) {
            return false;
        }

        atual = atual -> prox;
    }

    return true;
}

int get_num_vertices(grafo *g) {
    return g -> num_vertices;
}

// Dados para a ordenação de vértices por ordem decrescente de grau
typedef struct {
    int id_original;
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
int Welsh_Powell(grafo *g) {

    int n = get_num_vertices(g);
    elemento_ordenacao *lista = malloc(n * sizeof(elemento_ordenacao));
    for (int i = 0; i < n; i++) {
        lista[i].id_original = i;
        lista[i].grau = g -> vertices[i].grau;
    }

    qsort(lista, n, sizeof(elemento_ordenacao), comparar_graus);

    int cor_index = 0;
    int vertices_coloridos = 0;

    while (vertices_coloridos < n) {
        char cor_atual = lista_cores[cor_index];

        for (int i = 0; i < n; i++) {
            int u = lista[i].id_original;

            if (g -> vertices[u].cor != 0) continue;

            if (pode_pintar(g, u, cor_atual)) {
                g -> vertices[u].cor = cor_atual;
                vertices_coloridos++;
            }
        }
        cor_index++;
    }

    free(lista);

    return cor_index;
}


void imprimir_resultado(grafo *g) {
    printf("Resultado da coloração:\n");
    for (int i = 0; i < g -> num_vertices; i++) {
        printf("Vertice %d (Grau %d) -> Cor: %c\n",
               i, g -> vertices[i].grau, g -> vertices[i].cor);
    }
}

double tempo_usado(double fim, double inicio) {
    return ((double) (fim - inicio)) / CLOCKS_PER_SEC;
}

void print_info_coloracao(grafo *g, int numero_cromatico) {
    printf("Número cromático do grafo com %i vértices: %i\n", g -> num_vertices, numero_cromatico);
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

int main() {
    clock_t inicio_welsh_powell, fim_welsh_powell;
    clock_t inicio_programa, fim_programa;
    double tempo_usado_cpu_programa;
    double tempo_usado_cpu_welsh_powell;


    inicio_programa = clock();

    int n = 100000;

    grafo *g = init_grafo(n);

    int n_v = g -> num_vertices;

    printf("Gerando grafo com %d vertices...\n", n);

    for (int i = 0; i < n; i++) {
        add_aresta(g, i, (i + 1) % n_v);
        add_aresta(g, i, (i + 2) % n_v);
        add_aresta(g, i, (i + 3) % n_v);
    }


    inicio_welsh_powell = clock();
    int numero_cromatico = Welsh_Powell(g);
    fim_welsh_powell = clock();

    tempo_usado_cpu_welsh_powell = tempo_usado((double) fim_welsh_powell, (double) inicio_welsh_powell);

    printf("Tempo de execução do algoritmo Welsh & Powell: %lf\n", tempo_usado_cpu_welsh_powell);

    // imprimir_resultado(g);
    print_info_coloracao(g, numero_cromatico);

    liberar_grafo(g);

    fim_programa = clock();

    tempo_usado_cpu_programa = ((double) (fim_programa - inicio_programa)) / CLOCKS_PER_SEC;

    printf("tempo de execução do programa todo: %lf\n", tempo_usado_cpu_programa);
    return 0;
}








