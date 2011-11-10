#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
        int support;
        MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &support);
        if (support != MPI_THREAD_MULTIPLE)
                printf("FAILED! The MPI library does not support MPI_THREAD_MULTIPLE\n");
        else
                printf("SUCCESS! The MPI library has multithreaded support\n");
        MPI_Finalize();
        return 0;
}
