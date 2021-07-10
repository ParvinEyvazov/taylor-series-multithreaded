#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

double sum;
int operation;
int num_threads;

// function for a thread to calculate operations
void *sumer(void *arg)
{
    double curr_sum = 0;
    int interval, start, end;
    int thread_id;

    thread_id = *(int *)(arg);
    interval = (int) operation / num_threads;
    start = thread_id * interval;
    end = (thread_id + 1) * interval;

    // if this thread is the latest one, take all remaining part
    if(thread_id +1 >= num_threads){
        end = operation;
    }

    // add operations
    for (int i = start; i < end; i++)
    {
            curr_sum += (1.0/(4.0*(i+1)-3.0) - 1.0/(4.0*(i+1)-1.0));
    }

    // add result to general total sum
    sum += curr_sum;
}

int main()
{
    sum = 0.0;
    operation = 0;
    int start_thread = 0;
    int end_thread = 0;
    double latest = 0.0;

    // get values from user
    printf("Enter the operation numbers: ");
    scanf("%d", &operation);

    printf("From thread no: ");
    scanf("%d", &start_thread);

    printf("From %d to thread no: ",start_thread);
    scanf("%d", &end_thread);

    // time values initialize
    struct timeval total_operation_start, total_operation_stop;
    struct timeval one_thread_start, one_thread_stop;

    pthread_t *threads;

    FILE *fPtr;
    fPtr = fopen("stat.txt","w");

    // get total started time
    gettimeofday(&total_operation_start, NULL);


    // for each thread in the range (for ex: in range 1-60 threads)
    for(int current_thread = start_thread; current_thread <= end_thread; current_thread++){

        // get started time for 1 thread
        gettimeofday(&one_thread_start, NULL);

        num_threads = current_thread;
        threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

        for (int t = 0; t < num_threads; t++)
        {
            int *thread_id;
            thread_id = (int *)malloc(sizeof(int));
            *thread_id = t;
            pthread_create(&threads[t], NULL, sumer, (void *)thread_id);
        }

        for (int i = 0; i < num_threads; ++i)
        {
            pthread_join(threads[i], NULL);
        }

        latest = sum * 4.0;
        sum = 0.0;
        printf("for %d thread \t->done\n", current_thread);
        printf("--- --- --- ---\n");

        // get stopped time for 1 thread
        gettimeofday(&one_thread_stop, NULL);

        fprintf(fPtr, "for thread %d \n", current_thread);
        fprintf(fPtr, "\t\t operation time is: %3.5f seconds\n", ((one_thread_stop.tv_sec - one_thread_start.tv_sec) * 1000000 + one_thread_stop.tv_usec - one_thread_start.tv_usec) / 1000000.0);
    }

    // get total stopped time
    gettimeofday(&total_operation_stop, NULL);
    fprintf(fPtr, "\n pi: %.16f \n", latest);
    fprintf(fPtr, " Total time: %3.5f seconds\n", ((total_operation_stop.tv_sec - total_operation_start.tv_sec) * 1000000 + total_operation_stop.tv_usec - total_operation_start.tv_usec) / 1000000.0);


    return 0;
}
