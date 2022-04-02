
CC          = gcc
OMPFLAGS	= -fopenmp
CFLAGS		= -Wall $(OMPFLAGS)

PROCS       = 2
THREADS		= 2

ROWS        = 4
COLUMNS     = 4

main:
	mpicc -std=c99 $(CFLAGS) utils.c main.c -o main.o
run:
	@echo "Will run using $(PROCS) processes and $(THREADS) threads per process:"; export OMP_NUM_THREADS=$(THREADS); mpiexec -n $(PROCS) main.o $(ROWS) $(COLUMNS)
msj:
	@echo "***Random matrix of $(ROWS) x $(COLUMNS) and vector of $(COLUMNS)***\n";
clean:
	rm -f *% *~ core *.o $(PROGRAMS) $(PROGRAM)
