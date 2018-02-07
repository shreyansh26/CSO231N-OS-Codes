#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

int MAX;

#define MAX_THREAD 4
 
int matA[100][100];
int matB[100][100];
int matC[100][100];
int step_i = 0;
 
void* multiply(void* arg) {
    int core = step_i++;
    int i, j, k;
    // Each thread computes 1/4th of matrix multiplication
    for (i = core * MAX / 4; i < (core + 1) * MAX / 4; i++) 
        for (j = 0; j < MAX; j++) 
            for (k = 0; k < MAX; k++) 
                matC[i][j] += matA[i][k] * matB[k][j];
}
 
int main(int argc, char *argv[]) {
    clock_t begin = clock();
    if (argc != 2) {
        printf("format is:./a.out <intgervalue>\n");
        return -1;
    }
    
    MAX = atoi(argv[1]);
    int i, j, k;
    srand(time(NULL)); 
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }
 
    
    // printf("\nMatrix A\n");
    // for (i = 0; i < MAX; i++) {
    //     for (j = 0; j < MAX; j++) 
    //         printf("%d ", matA[i][j]);
    //     printf("\n");
    // }
 

    // printf("\nMatrix B\n");
    // for (i = 0; i < MAX; i++) {
    //     for (j = 0; j < MAX; j++) 
    //         printf("%d ", matB[i][j]);
    //     printf("\n");
    // }
 
    pthread_t threads[MAX_THREAD];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
 
    for (i = 0; i < MAX_THREAD; i++) {
        int* p;
        pthread_create(&threads[i], &attr, multiply, (void*)(p));
    }
 
    for (i = 0; i < MAX_THREAD; i++) 
        pthread_join(threads[i], NULL);    
 
    printf("\nMultiplication of A and B\n");
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) 
            printf("%d ", matC[i][j]);      
        printf("\n");
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time: %lf\n", time_spent);
    return 0;
}