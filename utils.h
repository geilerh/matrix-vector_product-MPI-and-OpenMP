#ifndef _UTILS_
#define _UTILS_


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>


#define _RAND_MAX 10


int** get_random_matrix(const int, const int); 
int* get_random_vector(const int n);
int* matrix_vector_product(int** matrix, int* vector, const int m, const int n);
void print_matrix2D(int** A, const int m, const int n);
void print_matrix1D(int* A, const int m, const int n);
void print_vector(int* v, const int n);


#endif