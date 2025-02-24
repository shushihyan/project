#include <iostream>

#include <pthread.h>

#include <semaphore.h>

#include <unistd.h>



const int N = 5;

sem_t sticks[N];



void* philosopher(void* arg) {

    int id = *((int*)arg);

    int left = id;

    int right = (id + 1) % N;



    while (true) {

        std::cout << "philosopher " << id << " thinks։" << std::endl;

        sleep(1 + (id % 3));

        if (id == 0) {

            sem_wait(&sticks[right]);

            sem_wait(&sticks[left]);

        } else {

            sem_wait(&sticks[left]);

            sem_wait(&sticks[right]);

        }

        std::cout << "philosopher" << id << " eats։" << std::endl;

        sleep(1 + (id % 2));

        sem_post(&sticks[left]);

        sem_post(&sticks[right]);



        std::cout << "philosopher " << id << " freed the sticks:" << std::endl;

    }



    return nullptr;

}



int main() {

    for (int i = 0; i < N; ++i) {

        sem_init(&sticks[i], 0, 1);

    }

    pthread_t philosophers[N];

    int ids[N];

    for (int i = 0; i < N; ++i) {

        ids[i] = i;

        pthread_create(&philosophers[i], nullptr, philosopher, &ids[i]);

    }

    for (int i = 0; i < N; ++i) {

        pthread_join(philosophers[i], nullptr);

    }



    for (int i = 0; i < N; ++i) {

        sem_destroy(&sticks[i]);

    }



    return 0;

}