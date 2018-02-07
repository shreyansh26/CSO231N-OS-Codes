#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>

#define MAX 2

#define MAX_THREAD 4
 
int matA[MAX][MAX];
int matB[MAX][MAX];
int matC[MAX][MAX];
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
 
int main() {

    int i, j, k;
    srand(time(NULL)); 
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) {
            matA[i][j] = rand() % 10;
            matB[i][j] = rand() % 10;
        }
    }
 
    
    printf("\nMatrix A\n");
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) 
            printf("%d ", matA[i][j]);
        printf("\n");
    }
 

    printf("\nMatrix B\n");
    for (i = 0; i < MAX; i++) {
        for (j = 0; j < MAX; j++) 
            printf("%d ", matB[i][j]);
        printf("\n");
    }
 
    pthread_t threads[MAX_THREAD];
    pthread_attr_t attr;

    int chk = pthread_attr_init(&attr);
    printf("%d\n", chk);
 
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
    return 0;
}