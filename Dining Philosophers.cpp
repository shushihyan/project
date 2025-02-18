#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

#define NUM_PHILOSOPHERS 5

const char* sem_names[NUM_PHILOSOPHERS] = {"/stick0", "/stick1", "/stick2", "/stick3", "/stick4"};

void philosopher(int philosopher_id) {
    sem_t* left_stick = sem_open(sem_names[philosopher_id], 0);
    sem_t* right_stick = sem_open(sem_names[(philosopher_id + 1) % NUM_PHILOSOPHERS], 0);

    if (left_stick == SEM_FAILED || right_stick == SEM_FAILED) {
        perror("Semaphore open failed");
        exit(1);
    }

    while (1) {
        printf("Philosopher %d is thinking...\n", philosopher_id);
        usleep(rand() % 1000000);

        if (philosopher_id == 0) {
            sem_wait(right_stick);
            sem_wait(left_stick);
        } else {
            sem_wait(left_stick);
            sem_wait(right_stick);
        }

        printf("Philosopher %d is eating...\n", philosopher_id);
        usleep(rand() % 1000000);

        sem_post(left_stick);
        sem_post(right_stick);
    }

    sem_close(left_stick);
    sem_close(right_stick);
}

int main() {
    pid_t pids[NUM_PHILOSOPHERS];

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_unlink(sem_names[i]);
        sem_t* sem = sem_open(sem_names[i], O_CREAT, 0644, 1);
        if (sem == SEM_FAILED) {
            perror("Semaphore initialization failed");
            exit(1);
        }
        sem_close(sem);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if ((pids[i] = fork()) == 0) {
            philosopher(i);
            exit(0);
        }
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        wait(NULL);
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_unlink(sem_names[i]);
    }

    return 0;
}
