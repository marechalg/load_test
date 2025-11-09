#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    pid_t id;
    time_t depart;
    time_t dernier_passage;
    int tours;
    time_t t1;
    time_t t2;
    time_t t3;
} Coureur;

void sig(int, siginfo_t *info, void *context);

Coureur coureurs[100];
int nb = 0;
int remaining = 0;

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = sig;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    sigaction(SIGINT, &sa, NULL);

    printf("Chrono démarré (PID : %d)\n", getpid());
    printf("--------------------------------\n\n");

    while (true) {
        pause();
    }

    return EXIT_SUCCESS;
}

Coureur initCoureur(pid_t id) {
    Coureur coureur;
    coureur.id = id;
    coureur.depart = time(NULL);
    coureur.dernier_passage = coureur.depart;
    coureur.t1 = 0;
    coureur.t2 = 0;
    coureur.t3 = 0;
    coureur.tours = 0;
    nb++;
    remaining++;
    return coureur;
}

Coureur* findCoureur(pid_t id) {
    Coureur *coureur = NULL;
    for (int i = 0; i < nb; i++) {
        if (coureurs[i].id == id) {
            coureur = &coureurs[i];
            break;
        }
    }
    return coureur;
}

int numCoureur(Coureur coureur) {
    int num = -1;
    for (int i = 0; i < nb; i++) {
        if (coureurs[i].id == coureur.id) {
            num = i;
            break;
        }
    }
    return num;
}

void passageCoureur(Coureur *coureur) {
    if (coureur != NULL) {
        time_t maintenant = time(NULL);
        time_t nouvelle_duree = maintenant - coureur->dernier_passage;
        
        coureur->t1 = coureur->t2;
        coureur->t2 = coureur->t3;
        coureur->t3 = nouvelle_duree;
        
        coureur->dernier_passage = maintenant;
        coureur->tours++;
    } else {
        printf("Coureur non parti\n\n");
    }
}

float moyenne(Coureur *coureur) {
    if (coureur->tours == 0) {
        return 0;
    }
    float somme = 0;
    int count = 0;
    if (coureur->t3) {
        somme += coureur->t3;
        count++;
    }
    if (coureur->t2 && coureur->tours >= 2) {
        somme += coureur->t2;
        count++;
    }
    if (coureur->t1 && coureur->tours >= 3) {
        somme += coureur->t1;
        count++;
    }
    return count > 0 ? somme / count : 0;
}

void affichageStats(Coureur *coureur) {
    printf("-------- N°%d --------\n", numCoureur(*coureur));
    printf("Tours : %d\n", coureur->tours);
    printf("Moyenne 3 derniers tours : %.2f\n", moyenne(coureur));
    printf("\n");
}

void sig(int sig, siginfo_t *info, void *context) {
    pid_t pid = info->si_pid;
    Coureur *coureur = NULL;

    switch (sig) {
        case SIGUSR1:
            printf("Départ d'un coureur\n\n");
            if (findCoureur(pid) == NULL) {
                coureurs[nb] = initCoureur(pid);
            } else {
                printf("Coureur déjà enregistré\n\n");
            }
            break;
        case SIGUSR2:
            coureur = findCoureur(pid);
            passageCoureur(coureur);
            affichageStats(coureur);
            break;
        case SIGTERM:
            coureur = findCoureur(pid);
            printf("N°%d a fini la course\n\n", numCoureur(*coureur));
            if (!(--remaining)) {
                printf("COURSE TERMINÉE : Affichage des Statistiques\n");
                for (int i = 0; i < nb; i++) {
                    affichageStats(&coureurs[i]);
                }
                exit(EXIT_SUCCESS);
            }
            break;
        case SIGINT:
            printf("COURSE TERMINÉE : Affichage des Statistiques\n");
            for (int i = 0; i < nb; i++) {
                affichageStats(&coureurs[i]);
            }
            break;
    }
}