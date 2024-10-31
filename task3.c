#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* uncancellable_thread(void* arg) {
    int iterations = *(int*)arg * 2;
    for (int i = 0; i < iterations; i++) {
        printf("Iteration %d\n", i + 1);
        sleep(1);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <delay_seconds>\n", argv[0]);
        return 1;
    }

    int delay = atoi(argv[1]);
    pthread_t thread;
    pthread_create(&thread, NULL, uncancellable_thread, &delay);
    sleep(delay);

    int cancel_status = pthread_cancel(thread);
    if (cancel_status == 0) {
        printf("Thread was canceled.\n");
    } else {
        printf("Thread finished execution.\n");
    }

    pthread_join(thread, NULL);
    return 0;
}
