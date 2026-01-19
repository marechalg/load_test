#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

int main(int argc, char* argsv[]) {
    if (argc > 1) {
        int choix;

        do {
            printf("----------------\n");
            printf("1. Départ\n");
            printf("2. Signaler un passage\n");
            printf("0. Quitter la course\n\n");
            scanf("%d", &choix);

            switch (choix) {
                case 1:
                    kill(atoi(argsv[1]), SIGUSR1);
                    break;
                case 2:
                    kill(atoi(argsv[1]), SIGUSR2);
                    break;
                default:
                    kill(atoi(argsv[1]), SIGTERM);
                    break;
            }
        } while (choix != 0);
    } else {
        printf("Aucun PID indiqué pour le programme chrono\n");
    }

    return EXIT_SUCCESS;
}