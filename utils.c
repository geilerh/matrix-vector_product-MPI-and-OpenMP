#include "utils.h"

int* matrix_vector_product(int** matrix, int* vector, const int m, const int n)
{
    int *product = malloc(sizeof(int) * m),
        sum      = 0,
        i        = 0,
        j        = 0;

    assert(product != NULL);

    for (i = 0; i < m; i++)
    {
        for (sum = 0, j = 0; j < n; j++)
            sum += matrix[i][j] * vector[j];
        product[i] = sum;
    }

    return product;
}


int** get_random_matrix(const int m, const int n)
{
    int **matrix = malloc(sizeof(int*) * m),
          i      = 0;

    assert(matrix != NULL);

    for (i = 0; i < m; i++)
        matrix[i] = get_random_vector(n);
    
    return matrix;
}

int* get_random_vector(const int n)
{

    int* vector = malloc(sizeof(int) * n),
         i      = 0;

    assert(vector != NULL);

    for (i = 0; i < n; i++) 
        vector[i] = rand() % _RAND_MAX;    
    
    return vector;
}

void print_vector(int* v, const int n)
{
    int i;
    printf("vector:\n");
    for (i = 0; i < n; i++)
        printf("row [%d] : [%d]\n", i, v[i]);
    printf("\n");    
}

void print_matrix1D(int* A, const int m, const int n)
{   
    int i, j;
    printf("matriz:\n"); 
    for (i = 0; i < m; i++) {
        printf("row [%d] : [", i);
        for (j = 0; j < n; j++) {
            printf("%d", A[ (i * n) + j]);
            if (j + 1 < n) printf(" ");
        }
        printf("]\n");
    }
    printf("\n");
}                 

void print_matrix2D(int** A, const int m, const int n)
{
    int i, j;
    printf("matriz:\n"); 
    for (i = 0; i < m; i++) {
        printf("row [%d] : [", i);
        for (j = 0; j < n; j++) {
            printf("%d", A[i][j]);
            if (j + 1 < n) printf(" ");
        }
        printf("]\n");
    }
    printf("\n");
}                 