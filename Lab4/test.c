#include <mpich/mpi.h>
#include <stdio.h>

int main (int argc, char* argv[])
{
  int rank, size;
  int count = 0;
  MPI_Init (&argc, &argv);      /* starts MPI*/
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);        /* get current process id*/
  MPI_Comm_size (MPI_COMM_WORLD, &size);        /* get number of processes*/
  printf( "Hello world from process %d of %d\n", rank, size );
  count ++;
  MPI_Finalize();
  printf("count: %d\n", count);
  return 0;
}