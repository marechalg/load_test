#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#include "include/all.h"

#define TARGET_MAX_SIZE 1024

char *MENU[4] = { "Quit", "New DDOS attack", "View last attack", "Export last attack" };
enum menu_t {
    QUIT,
    DDOS,
    VIEW,
    EXPORT
};

pid_t pingId;

enum menu_t menu();
void handler(enum menu_t);
void waitUser();
void sig(int, siginfo_t*, void*);

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sig;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGCHLD, &sa, NULL);
    
    enum menu_t choice;
    do {
        system("clear");
        choice = menu();
        handler(choice);
    } while (choice != QUIT);

    return EXIT_SUCCESS;
}

enum menu_t menu() {
    int choice;
    printf("-------- DDoS Attack --------\n");
    printf("%d. %s\n", DDOS, MENU[DDOS]);
    printf("%d. %s\n", VIEW, MENU[VIEW]);
    printf("%d. %s\n", EXPORT, MENU[EXPORT]);
    printf("%d. %s\n", QUIT, MENU[QUIT]);
    printf("\n");
    scanf("%d", &choice);
    printf("-----------------------------\n");
    return (enum menu_t)choice;
}

void handler(enum menu_t choice) {
    switch (choice) {
        case DDOS:
            int threads, connections, time = -1;
            char target[TARGET_MAX_SIZE];
            system("clear");
            printf("-------- DDoS Attack --------\n");

            // Verify target
            while (true) {
                printf("Target : "); scanf("%s", target);
                if (strncmp(target, "http://", 7) == 0 || strncmp(target, "https://", 8) == 0) {
                    printf("Reaching %s...\n", target);
                    if (reachable(target)) {
                        printf("Successfully reached %s\n", target);
                        break;
                    } else {
                        printf("Failed : %s unreachable\n", target);
                    }
                } else {
                    printf("Invalid target (%s) : must be http / https server\n", target);
                }
            }
            printf("\n");

            // Verify threads
            while (true) {
                printf("Number of threads : "); scanf("%d", &threads);
                if (threads <= 0) {
                    printf("Invalid number of threads (%d) : minimum 1 thread\n", threads);
                } else break;
            }
            printf("\n");

            // Verify connections
            while (true) {
                printf("Number of connections : "); scanf("%d", &connections);
                if (connections <= 0) {
                    printf("Invalid number of connections (%d) : minimum 1 connection\n", connections);
                } else break;
            }
            printf("\n");

            // Verify time
            while (true) {
                printf("Time of connections (seconds) : "); scanf("%d", &time);
                if (time <= 0) {
                    printf("Invalid time (%d) : minimum 1s\n", time);
                } else break;
            }
            printf("\n");

            ddos(target, threads, connections, time);
            waitUser();
            break;

        case VIEW:
            system("clear");
            printf("-------- Last Attack --------\n");
            waitUser();
            break;

        case EXPORT:
            system("clear");
            waitUser();
            break;

        case QUIT:
            return;

        default:
            printf("Unknown choice\n");
            break;
    }
}

void waitUser() {
    while (getchar() != '\n');
    getchar();
}

void sig(int signal, siginfo_t *info, void *context) {
    switch (signal) {
        case SIGCHLD:
            break;
    }
}