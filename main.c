/*******************************************************************************
*                                                                              *
*   main.c  -   programa para calcular el producto matriz-vector con omp y mpi.*
*               Es decir, Ax=b, donde la matriz A se genera ale-               *
*               atoriamente dado los parametros m y n, donde m                 *
*               es el numero de filas y n el numero de columnas.               *                                                      *
*                                                                              *
*   Departamento de Electronica y Ciencias de la Computacion                   *
*   Pontificia Universidad Javeriana - CALI                                    *
*   Porgramacion Paralela                                                      *
*                                                                              *
*   Autor:  Geiler Orlando Hipia Meji'a                                        * 
*                                                                              *
*******************************************************************************/

#include "utils.h"
#include <mpi.h>
#include <omp.h>

#define ROOT_PROCESS   0


int main ( int argc, char *argv[] )
{
    if ( argc != 3 )
    {
        printf( "Usage: %s <elements_per_proc> <number_rows> <number_cols>\n", argv[0] );
        exit( -1 );
    }

    int my_rank,                /* rank de los procesos                 */
        p,                      /* numero de procesos                   */
        m,                      /* numero de filas de la matriz         */
        n,                      /* numero de columnas de la matriz      */
        num_elements_per_proc,  /* cantidad de elementos por proceso    */
        **matrix_2D = NULL,     /* matriz 2D del proceso principal (root)  */
        *matrix_1D = NULL,      /* matriz 1D del proceso principal (root)  */
        *submatrix = NULL,      /* submatriz de cada proceso            */
        *vector = NULL,         /* vector x                             */
        *subproduct = NULL,     /* subvector del producto matriz-vector */
        *product = NULL;        /* vector producto matriz-vector        */


    // inicializando el MPI runtime
    MPI_Init ( &argc, &argv );
    // determinando el rank de cada proceso
    MPI_Comm_rank ( MPI_COMM_WORLD, &my_rank );
    // determinando el numero de procesos
    MPI_Comm_size ( MPI_COMM_WORLD, &p );


    // verficando que el proceso actual sea el principal (root)
    if (my_rank == ROOT_PROCESS)
    {
        m = atoi( argv[1] ); // numero de filas
        n = atoi( argv[2] ); // numero de columnas

        // reservando memoria para matriz unidimensional
        // de tamano m x n
        matrix_1D = malloc(sizeof(int) * m * n);

        // verificando si no fue posible reservar memoria
        assert(matrix_1D != NULL);


        // obteniendo matriz 2D aleatoria
        matrix_2D = get_random_matrix(m, n);

        // Aplanando matriz 2D:
        // La matriz generada aleatoriamente tiene una dimension 2
        // en la implementacion presente las matrices se trabajan
        // con 1 dimension.
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                matrix_1D[ (i * n) + j] = matrix_2D[i][j];

        // imprimiendo matriz
        print_matrix1D(matrix_1D, m, n);
    }

    // Enviando a todos los procesos el numero de columnas que tiene la matriz
    MPI_Bcast( &n, 1, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD );
    // Enviando a todos los procesos el numero de filas que tiene la matriz
    MPI_Bcast( &m, 1, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD );

    // determinando el numero de elementos que tendra cada proceso
    num_elements_per_proc = (n * m) / p;
    // reservando memoria para la submatrix
    submatrix = (int*) malloc(sizeof(int) * num_elements_per_proc);
    // reservando memoria para el vector x
    vector = (int*) malloc(sizeof(int) * n);
    // verificando si no fue posible reservar memoria
    assert(submatrix != NULL && vector !=  NULL && product == NULL);

    // verficando que el proceso actual sea el principal (root)
    if (my_rank == ROOT_PROCESS)
    {
        // obteniendo un vector random de dimension n
        vector = get_random_vector(n);
        print_vector(vector, n);
        // reservando memoria para el producto matriz-vector
        product = (int*) malloc(sizeof(int) * m);
        assert(product != NULL);
    }

    // enviando el vector x a todos los procesos
    MPI_Bcast (vector, n, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD );

    // enviando las submatrices a cada proceso
    MPI_Scatter (matrix_1D, num_elements_per_proc, MPI_INT,
                    submatrix, num_elements_per_proc, MPI_INT,
                    ROOT_PROCESS, MPI_COMM_WORLD );


    // determinando la cantidad de filas de la submatrix
    int m_sub = num_elements_per_proc/n;
    // reservando memoria para la subvector del producto mnatriz-vector
    subproduct = malloc(sizeof(int) * m_sub);
    // verificando si no fue posible reservar memoria
    assert(subproduct != NULL);

    int sum = 0,
        i   = 0,
        j   = 0;

    // inicializando vector
    for (i = 0; i < m_sub; i++) subproduct[i] = 0;

    // realizando la operacion producto matriz-vector
    #pragma omp parallel for private(j) reduction(+: sum)
    for (i = 0; i < m_sub; i++)
    {
        for (sum = 0, j = 0; j < n; j++)
            sum += submatrix[ (i * n) + j] * vector[j];
        subproduct[i] += sum;
    }

    // Enviando el resultado del producto matriz-vector al proceso principal (rooot)
    MPI_Gather (subproduct, m_sub, MPI_INT, &product[my_rank * m_sub], m_sub,
                MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD );


    // verficando que el proceso actual sea el principal (root)
    if (my_rank == ROOT_PROCESS)
    {
        // imprimiendo el resultado del producto
        printf("resultado del producto matriz-vector paralelo (MPI + OpenMP)\n");
        print_vector(product, m);
        // imprimiendo el resultado del producto
        printf("resultado del producto matriz-vector secuencial\n");
        // calculando el producto de manera secuencial
        product = matrix_vector_product(matrix_2D, vector, m, n);
        print_vector(product, m);

        free(product);
        free(matrix_1D);
        free(matrix_2D);
    }

    // liberando memoria
    free(submatrix);
    free(vector);
    free(subproduct);
    // shutdwon MPI runtime
    MPI_Finalize ();
}
