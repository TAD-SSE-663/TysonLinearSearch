#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>

void* search(void* rank);
void printLocation();
long lrand();

long thread_count;
long array_size = 100000000;
long range_per_thread;
long* array;
long target;
long target_location;

int main(int argc, char* argv[]) {
    array = (long *) malloc(array_size *sizeof(long));
    long thread;
    pthread_t* thread_handles;
    struct timeval startTime;
    struct timeval endTime;
    int numTrials = 100;
    srand(100);

    thread_count = strtol(argv[1], NULL, 10);

    range_per_thread = array_size / thread_count;

    thread_handles = malloc (thread_count * sizeof(pthread_t));

    for (long i = 0; i < array_size; i++){
        *(array + i) = i;
    }

    gettimeofday(&startTime, NULL);

    for(int i = 0; i < numTrials; i++){
        target = lrand() % array_size;

        for (thread = 0; thread < thread_count; thread++){
            pthread_create(&thread_handles[thread], NULL, search, (void*) thread);
        }

        for (thread = 0; thread < thread_count; thread++){
            pthread_join(thread_handles[thread], NULL);
        }

        target_location = -1;
    }

    gettimeofday(&endTime, NULL);

    double time_taken = (double) endTime.tv_sec + (double) endTime.tv_usec / 1000000 - ((double) startTime.tv_sec + (double) startTime.tv_usec / 1000000);

    printf("The time taken was %f seconds\n", time_taken);

    printf("Thread count: %ld", thread_count);

    free(thread_handles);
    return 0;
}

void* search(void* rank){
    for(long i = (long) rank * range_per_thread; (i < ((long) rank + 1) * (range_per_thread) && target_location == -1); i++){
        if(*(array + i) == target){
            target_location = i;
        }
    }

    return -1;
}

void printLocation(){
    printf("The location of %ld was %ld\n", target, target_location);
}

long lrand(){
    return (((long) rand()) << (sizeof(int) * 8) | rand());
}