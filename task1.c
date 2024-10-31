#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int *global_array;
int threads_count;
int completed_threads = 0;
pthread_mutex_t lock;

void* child_thread(void* arg) {
    int thread_num = *(int*)arg;
    sleep(thread_num);

    int rand_num = rand() % 10 + 1;
    pthread_mutex_lock(&lock);
    global_array[thread_num] = rand_num;
    completed_threads++;
    pthread_mutex_unlock(&lock);

    sleep(rand_num);
    return NULL;
}

void* monitor_thread(void* arg) {
    while (1) {
        sleep(1);
        pthread_mutex_lock(&lock);
        printf("Current array values: ");
        for (int i = 0; i < threads_count; i++) {
            printf("%d ", global_array[i]);
        }
        printf("\n");
        if (completed_threads == threads_count) {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    threads_count = atoi(argv[1]);
    global_array = (int*)malloc(threads_count * sizeof(int));
    pthread_t threads[threads_count], monitor;
    int thread_nums[threads_count];
    srand(time(NULL));

    pthread_mutex_init(&lock, NULL);
    for (int i = 0; i < threads_count; i++) {
        thread_nums[i] = i;
        if (pthread_create(&threads[i], NULL, child_thread, &thread_nums[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    pthread_create(&monitor, NULL, monitor_thread, NULL);

    for (int i = 0; i < threads_count; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_join(monitor, NULL);
    pthread_mutex_destroy(&lock);
    free(global_array);

    printf("All threads completed.\n");
    return 0;
}
