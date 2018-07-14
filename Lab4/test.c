#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpich/mpi.h>
#include <time.h>

int main(int argc, char * argv[])
{
int rank;
int buffer_size;
int * buffer;
char file[128];
FILE * fp;
MPI_Status status;
sleep(30);
MPI_Init(&argc, &argv);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
if (rank == 0) {
   srand((int)time(0));
    buffer_size = (rand() + 1)%65536;
    buffer = (int *)malloc(buffer_size*sizeof(int));
    printf("rank %d: send a buffer with size %d\n", rank, buffer_size);
    MPI_Send(buffer, buffer_size, MPI_INT, 1, 99, MPI_COMM_WORLD);
}
else {
    MPI_Probe(0, 99, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_INT, &buffer_size);
    buffer = (int *)calloc(buffer_size, sizeof(int));
    printf("rank %d: receive a buffer with size %d\n", rank, buffer_size);
    MPI_Recv(buffer, buffer_size, MPI_INT, 0, 99, MPI_COMM_WORLD, &status);
}
MPI_Finalize();
sprintf(file, "data_%d.dat", rank);
fp = (FILE *)fopen(file, "wb");
fwrite(buffer, sizeof(int), buffer_size, fp);
free(buffer);
return 0;
}