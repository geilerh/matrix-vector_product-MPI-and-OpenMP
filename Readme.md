main.c  - programa para calcular el producto matriz-vector secuencial  con omp, mpi y omp + mpi.                                     
          Producto matriz-vecotr, Ax=b, donde la matriz A se genera aleatoriamente dado los parametros m y n, donde m          
          es el numero de filas y n el numero de columnas.                                                                          
Departamento de Electronica y Ciencias de la Computacion                   
Pontificia Universidad Javeriana - CALI Porgramacio'n Paralela
Autor:  Geiler Orlando Hipia Meji'a

## Primeramente
    En el archivo Makefile estan previmente establecido las dimensiones de la matriz,\
    la cantidad de hilos y el nu'mero de procesos.
        PROCS       = <nu'mero de procesos>
        THREADS		= <nu'mero de hilos>
        ROWS        = <nu'mero de filas>
        COLUMNS     = <nu'mero de columnas>
## NOTA IMPORTANTE:
     La cantidad de elementos de la matriz debe ser divisible por el nu'mero de procesos y la cantiad de hilos debe ser menor o igual al numero de filas por cada proceso.
## Para compilar ingrese
    $ make
## Para ejecutar ingrese
    $ make run
## Para limpiar usa
    $ make clean
