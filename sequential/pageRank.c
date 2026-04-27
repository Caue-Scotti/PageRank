#include <stdio.h>
#include <math.h>

void normalize_matriz (float matriz[][4]);
void normalize_rank (float rank[]);
int equals(float rank[], float last_rank[]);

int main (){

    float link_matriz[4][4] =  {{0, 1, 0, 0},
                                {1, 0, 0, 1},
                                {1, 0, 0, 1},
                                {1, 1, 1, 0}};

    normalize_matriz(link_matriz);

    // printf("Matriz: \n");
    // for (int i = 0; i < 4; i++){
    //         for (int j = 0; j < 4; j++){
    //             printf("%f ", link_matriz[i][j]);
    //         }
    //         printf("\n");
    //     }

    float last_rank[4] = {0};
    float rank[4] = {0};
    normalize_rank(rank);

    while (equals(rank, last_rank) == 0){
        for (int i = 0; i < 4; i++){
            last_rank[i] = rank[i];
            rank[i] = 0;
        }

        // printf("Last Rank: ");
        // for (int i = 0; i < 4; i++){
        //     printf("%f ", last_rank[i]);
        // }
        // printf("\n");


        // Cálculo do PageRank
        for (int i = 0; i < 4; i++){
            for (int j = 0; j < 4; j++){
                rank[i] += last_rank[j] * link_matriz[i][j];
            }
        }

        // printf("Rank: ");
        // for (int i = 0; i < 4; i++){
        //     printf("%f ", rank[i]);
        // }
        // printf("\n");
    }

    printf("Ranks finais:\n");
    for (int i = 0; i < 4; i++){
        printf("Página %d: %.3f\n", i, rank[i]);
    }

}

void normalize_matriz (float matriz[][4]){
    for (int j = 0; j < 4; j++){
        float count = 0;
        for (int i = 0; i < 4; i++){
            count += matriz[i][j];
        }
        for (int i = 0; i < 4; i++){
            matriz[i][j] = matriz[i][j]/count;
        }
    }
}

void normalize_rank (float rank[]){
    for (int i = 0; i < 4; i++){
        rank[i] = 1.0/4.0;
    }
}

int equals (float rank[], float last_rank[]){
    for (int i = 0; i < 4; i++){
        if (fabs(rank[i] - last_rank[i]) > 0.001f)
            return 0;
    }
    return 1;
}