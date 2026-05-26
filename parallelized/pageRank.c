#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

// Compile:
// gcc -fopenmp -o pageRank pageRank.c

// Run:
// OMP_NUM_THREADS=4 ./pageRank entrada.txt

#define EPSILON 0.001f

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

    double start = omp_get_wtime();

    calculate_pagerank(link_matriz, rank, last_rank, n);

    double end = omp_get_wtime();

    printf("\n===== RESULTADOS =====\n");
    printf("Threads utilizadas: %d\n", omp_get_max_threads());
    printf("Tempo de execucao: %f segundos\n", end - start);

    printf("\nRanks finais:\n");

    for (int i = 0; i < n; i++) {
        printf("Pagina %d: %.6f\n", i, rank[i]);
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

        // Evita divisão por zero casa a soma da coluna seja zero
        if (count == 0) {
            for (int i = 0; i < n; i++) {
                matriz[i][j] = 1.0f / n;
            }
            continue;
        }

        for (int i = 0; i < n; i++) {
            matriz[i][j] /= count;
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

        if (fabs(rank[i] - last_rank[i]) > EPSILON)
            return 0;
    }

    return 1;
}

void calculate_pagerank(float **link_matriz, float rank[], float last_rank[], int n) {

    int iterations = 0;
    float damping = 0.85f;

    while (!equals(rank, last_rank, n)) {

        // Copia ranks anteriores
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            last_rank[i] = rank[i];
            rank[i] = (1.0f - damping) / n;
        }

        // Cálculo paralelo do PageRank
        #pragma omp parallel for
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                rank[i] += damping * last_rank[j] * link_matriz[i][j];
            }
        }

        iterations++;
    }

    printf("Numero de iteracoes: %d\n", iterations);
}