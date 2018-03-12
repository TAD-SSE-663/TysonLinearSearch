#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

long locationOf(long, const long*, long);
void printLocation(long, const long*, long);
long lrand();

int main() {
    long array_size = 100000000;
    long* array;
    array = (long *) malloc(array_size *sizeof(long));
    struct timeval startTime;
    struct timeval endTime;
    int numTrials = 100;
    srand(100);

    for (long i = 0; i < array_size; i++){
        array[i] = i;
    }

    gettimeofday(&startTime, NULL);

    for(int i = 0; i < numTrials; i++){
        locationOf(lrand() % array_size, array, array_size);
    }

    gettimeofday(&endTime, NULL);

    double time_taken = (double) endTime.tv_sec + (double) endTime.tv_usec / 1000000 - ((double) startTime.tv_sec + (double) startTime.tv_usec / 1000000);

    printf("The time taken was %f seconds\n", time_taken);

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

void printLocation(long target, const long* array, long size){
    printf("The location of %ld was %ld\n", target, locationOf(target, array, size));
}

long lrand(){
    return (((long) rand()) << (sizeof(int) * 8) | rand());
}