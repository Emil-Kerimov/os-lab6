#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* pi_calculation_thread(void* arg) {
    int n = 100000;
    double pi = 0.0;
    int sign = 1;

    for (int i = 0; i < n; i++) {
        pthread_testcancel();
        pi += sign / (2.0 * i + 1);
        sign = -sign;
        if ((i + 1) % 10000 == 0) {
            printf("Current π estimate: %f\n", 4 * pi);
        }
    }
    pi *= 4;
    printf("Final π estimate: %f\n", pi);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <delay_seconds>\n", argv[0]);
        return 1;
    }

    int delay = atoi(argv[1]);
    pthread_t thread;

    pthread_create(&thread, NULL, pi_calculation_thread, NULL);
    sleep(delay);

    pthread_cancel(thread);
    pthread_join(thread, NULL);

    return 0;
}
