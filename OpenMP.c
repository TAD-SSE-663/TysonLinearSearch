#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

long locationOf(long, const long*, long);
long lrand();
int thread_count;

int main(int argc, char* argv[]) {
    long array_size = 100000000;
    long* array;
    array = (long *) malloc(array_size *sizeof(long));
    struct timeval startTime;
    struct timeval endTime;
    int numTrials = 100;
    srand(100);
    long target_location;
    long target;

    thread_count = strtol(argv[1], NULL, 10);

    for (long i = 0; i < array_size; i++){
        array[i] = i;
    }

    gettimeofday(&startTime, NULL);

    for(int i = 0; i < numTrials; i++){
        target = lrand() % array_size;
        printf("Location of %ld was %ld\n", target, locationOf(target, array, array_size));
    }

    gettimeofday(&endTime, NULL);

    double time_taken = (double) endTime.tv_sec + (double) endTime.tv_usec / 1000000 - ((double) startTime.tv_sec + (double) startTime.tv_usec / 1000000);

    printf("The time taken was %f seconds\n", time_taken);

    return 0;
}

long locationOf(long target, const long* array, long size){
    long target_location = -1;
#   pragma omp parallel for num_threads(thread_count)
    for(long i =0; i < size; i++){
        if(*(array + i) == target){
            target_location = i;
        }
    }

    return target_location;
}

long lrand(){
    return (((long) rand()) << (sizeof(int) * 8) | rand());
}