#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* cancellable_thread(void* arg) {
    int count = 0;
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    while (1) {
        printf("Thread iteration %d\n", ++count);
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

    pthread_create(&thread, NULL, cancellable_thread, NULL);
    sleep(delay);

    pthread_cancel(thread);
    void* res;
    pthread_join(thread, &res);

    if (res == PTHREAD_CANCELED) {
        printf("Thread was canceled.\n");
    } else {
        printf("Thread finished normally.\n");
    }

    return 0;
}
