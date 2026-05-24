#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Compile: gcc -o pageRank pageRank.c
// Usage: ./pageRank <file_name>

void normalize_matriz(float **matriz, int n);
void normalize_rank(float rank[], int n);
int equals(float rank[], float last_rank[], int n);
void calculate_pagerank(float **link_matriz, float rank[], float last_rank[], int n);

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Uso: %s <arquivo>\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", argv[1]);
        return 1;
    }

    int n;

    // Lê o tamanho da matriz
    fscanf(file, "%d", &n);

    float **link_matriz = malloc(n * sizeof(float*));

    for (int i = 0; i < n; i++) {
        link_matriz[i] = malloc(n * sizeof(float));
    }

    // Leitura da matriz
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%f", &link_matriz[i][j]);
        }
    }

    fclose(file);

    normalize_matriz(link_matriz, n);

    // // Vetores de rank
    float *last_rank = calloc(n, sizeof(float));
    float *rank = calloc(n, sizeof(float));

    normalize_rank(rank, n);

    calculate_pagerank(link_matriz, rank, last_rank, n);

    printf("Ranks finais:\n");

    for (int i = 0; i < n; i++) {
        printf("Página %d: %.2f\n", i, rank[i]);
    }

    // Liberação de memória
    for (int i = 0; i < n; i++) {
        free(link_matriz[i]);
    }

    free(link_matriz);
    free(rank);
    free(last_rank);

    return 0;
}

void normalize_matriz(float **matriz, int n) {

    for (int j = 0; j < n; j++) {

        float count = 0;

        for (int i = 0; i < n; i++) {
            count += matriz[i][j];
        }

        // Evita divisão por zero
        if (count == 0)
            continue;

        for (int i = 0; i < n; i++) {
            matriz[i][j] = matriz[i][j] / count;
        }
    }
}

void normalize_rank(float rank[], int n) {

    for (int i = 0; i < n; i++) {
        rank[i] = 1.0f / n;
    }
}

int equals(float rank[], float last_rank[], int n) {

    for (int i = 0; i < n; i++) {
        if (fabs(rank[i] - last_rank[i]) > 0.001f)
            return 0;
    }

    return 1;
}

void calculate_pagerank(float **link_matriz,
                        float rank[],
                        float last_rank[],
                        int n) {

    while (equals(rank, last_rank, n) == 0) {

        for (int i = 0; i < n; i++) {
            last_rank[i] = rank[i];
            rank[i] = 0;
        }

        // Cálculo do PageRank
        // ****A paralelização tem que ser feita aqui!!!****
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                rank[i] += last_rank[j] * link_matriz[i][j];
            }
        }
    }
}