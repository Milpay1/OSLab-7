#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 20 // Define the size of the matrix as 20

int matA[MAX][MAX];
int matB[MAX][MAX];
int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

typedef struct {
    int startRow;
    int endRow;
} ThreadData;

void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* computeSum(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matSumResult[i][j] = matA[i][j] + matB[i][j];
        }
    }
    free(data); // Free the dynamically allocated memory
    pthread_exit(NULL);
}

void* computeDiff(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matDiffResult[i][j] = matA[i][j] - matB[i][j];
        }
    }
    free(data); // Free the dynamically allocated memory
    pthread_exit(NULL);
}

void* computeProduct(void* args) {
    ThreadData* data = (ThreadData*) args;
    for (int i = data->startRow; i < data->endRow; i++) {
        for (int j = 0; j < MAX; j++) {
            matProductResult[i][j] = 0; // Initialize the product value
            for (int k = 0; k < MAX; k++) {
                matProductResult[i][j] += matA[i][k] * matB[k][j];
            }
        }
    }
    free(data); // Free the dynamically allocated memory
    pthread_exit(NULL);
}

int main() {
    srand(time(0)); // Seed the random number generator

    // Fill the matrices with random values
    fillMatrix(matA);
    fillMatrix(matB);

    // Print the initial matrices
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // Create pthread_t objects
    pthread_t threads[10];
    int rowsPerThread = MAX / 10; // Divide rows evenly among threads

    // Perform addition
    for (int i = 0; i < 10; i++) {
        ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
        data->startRow = i * rowsPerThread;
        data->endRow = (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeSum, (void*) data);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Perform subtraction
    for (int i = 0; i < 10; i++) {
        ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
        data->startRow = i * rowsPerThread;
        data->endRow = (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeDiff, (void*) data);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Perform multiplication
    for (int i = 0; i < 10; i++) {
        ThreadData* data = (ThreadData*) malloc(sizeof(ThreadData));
        data->startRow = i * rowsPerThread;
        data->endRow = (i + 1) * rowsPerThread;
        pthread_create(&threads[i], NULL, computeProduct, (void*) data);
    }
    for (int i = 0; i < 10; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print results
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
