#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

pid_t pid[6];

void sig(int, siginfo_t*, void*);

int main() {
    struct sigaction sa;
    sa.sa_flags = SA_FLAGINFO;
    sa.sa_sigaction = sig;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGCHLD, &sa, NULL);
    sigaction(SIGUSR1, &sa, NULL);

    for (int i = 0; i < 5; i++) {
        pid[i] = fork();

        if (pid == 0) {
            execl("/usr/bin/xeyes", "xeyes", NULL);
        } else {
            int i = 1;
            while (true) {
                printf("Passage %d\n", i++);
                sleep(60);
            } 
        }
    }

    return EXIT_SUCCESS;
}

void sig(int sig, siginfo_t *info, void *context) {
    switch (sig) {
        pid_t cur_pid = info->si_pid;

        case SIGCHLD:
            wait(NULL);
            for (int i = 0; i < 5; i++) {
                if (pid[i] == cur_pid) {
                    pid[i] = fork();

                    if (pid[i] == 0) {
                        execl("/usr/bin/xeyes", "xeyes", NULL);
                    }
                }
            }
            break;
        case SIGUSR1:
            for (int i = 0; i < 5; i++) {
                kill(pid[i], SIGKILL);
            }
            exit(0);
            break;
    }
}