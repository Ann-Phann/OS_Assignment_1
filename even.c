#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

/*
 * Signal handler for `SIGHUP`,  prints out "Ouch!"
 */
void handle_sighup(int _) {
    printf("Ouch!\n");
}

/*
 * Signal handler for `SIGINT`, prints out "Yeah!"
 */
void handle_sigint(int _) {
    printf("Yeah!\n");
}

int main(int argc, char**argv) {
    if (argc < 2) {
        fprintf(stderr, "Must provide a number as arguement");
        return 1;
    }

    int num = atoi(argv[1]);

    // Setup signal handlers for SIGHUP and SIGINT
    signal(SIGHUP, handle_sighup);
    signal(SIGINT, handle_sigint);

    // Print n even numbers from 0 and sleep 5s
    for (int i = 0; i < num; i++) {
        printf("%d\n", i*2);
        fflush(stdout);
        sleep(5);
    }
    return 0;
}