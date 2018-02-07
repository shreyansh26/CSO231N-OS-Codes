#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int N;

void multiply(int mat1[][N], int mat2[][N], int res[][N]) {
    int i, j, k;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            res[i][j] = 0;
            for (k = 0; k < N; k++)
                res[i][j] += mat1[i][k]*mat2[k][j];
        }
    }
}
 
int main(int argc, char *argv[]) {
    clock_t begin = clock();
    if (argc != 2) {
        printf("format is:./a.out <intgervalue>\n");
        return -1;
    }
    N = atoi(argv[1]);
    int mat1[N][N];
    int mat2[N][N];
    int i, j;
    srand(time(NULL)); 
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            mat1[i][j] = rand() % 10;
            mat2[i][j] = rand() % 10;
        }
    }
 
    int res[N][N]; // To store result
    multiply(mat1, mat2, res);
 
    printf("Result matrix is \n");
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
           printf("%d ", res[i][j]);
        printf("\n");
    }
    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Time: %lf\n", time_spent);
    return 0;
}