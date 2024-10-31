#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

pthread_key_t message_key;

void* thread_function(void* arg) {
    int thread_id = *(int*)arg;
    int lines = rand() % 5 + 1;

    char *message = (char*)malloc(50 * sizeof(char));
    snprintf(message, 50, "Thread %d:", thread_id);
    pthread_setspecific(message_key, message);

    for (int i = 0; i < lines; i++) {
        int random_number = rand() % 100 + 1;
        printf("%s Random number: %d\n", (char*)pthread_getspecific(message_key), random_number);
        sleep(1);
    }

    free(message);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <number_of_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    srand(time(NULL));
    pthread_key_create(&message_key, NULL);

    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        if (pthread_create(&threads[i], NULL, thread_function, &thread_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_key_delete(message_key);
    return 0;
}
