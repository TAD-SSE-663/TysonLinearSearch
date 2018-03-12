#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>

long locationOf(long, const long*, long);
long lrand();

int main() {
    int my_rank, comm_sz;
    long array_size = 100000000;
    long* array;
    long* local;
    long target_location;
    long target;
    int numTrials = 100;
    struct timeval startTime;
    struct timeval endTime;
    long local_size;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    local = (long *) malloc(array_size / comm_sz * sizeof(long));
    local_size = array_size / comm_sz;

    if (my_rank == 0) {
        array = (long *) malloc(array_size * sizeof(long));
        srand(100);

        for (long i = 0; i < array_size; i++) {
            array[i] = i;
        }


        gettimeofday(&startTime, NULL);

        MPI_Scatter(array, local_size, MPI_LONG, local, local_size, MPI_LONG, 0, MPI_COMM_WORLD);
    } else {
        MPI_Scatter(array, local_size, MPI_LONG, local, local_size, MPI_LONG, 0, MPI_COMM_WORLD);
    }

    for (int i = 0; i < numTrials; i++) {
        if (my_rank == 0){
            target = lrand() % array_size;
            MPI_Bcast(&target, 1, MPI_LONG, 0, MPI_COMM_WORLD);
            target_location = -1;
            MPI_Bcast(&target_location, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        } else {
            MPI_Bcast(&target, 1, MPI_LONG, 0, MPI_COMM_WORLD);
            MPI_Bcast(&target_location, 1, MPI_LONG, 0, MPI_COMM_WORLD);
        }
        target_location = locationOf(target, local, local_size);
        if (target_location != -1) target_location += my_rank * local_size;

        if (my_rank != 0 && target_location != -1){
            MPI_Send(&target_location, 1, MPI_LONG, 0, 0, MPI_COMM_WORLD);
        }
        if (my_rank == 0 && target_location == -1){
            MPI_Recv(&target_location, 1, MPI_LONG, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (my_rank == 0){
        gettimeofday(&endTime, NULL);

        double time_taken = (double) endTime.tv_sec + (double) endTime.tv_usec / 1000000 -
                            ((double) startTime.tv_sec + (double) startTime.tv_usec / 1000000);

        printf("The time taken was %f seconds\n", time_taken);
    }

    MPI_Finalize();
    return 0;
}

long locationOf(long target, const long* array, long size){
    for(long i =0; i < size; i++){
        if(*(array + i) == target){
            return i;
        }
    }

    return -1;
}

long lrand(){
    return (((long) rand()) << (sizeof(int) * 8) | rand());
}