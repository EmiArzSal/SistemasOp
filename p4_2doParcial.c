#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

// Declaración de semáforos y variables compartidas
sem_t sem_estudiante, sem_certificado, sem_pago;
int certificado_listo = 0;

// Función que representa el proceso de un estudiante
void *estudiante(void *id) {
    int estudiante_id = *(int *)id;

    // El estudiante llega a la secretaría
    printf("Estudiante %d llega a la secretaría.\n", estudiante_id);

    // El estudiante solicita un certificado de notas
    printf("Estudiante %d solicita un certificado de notas.\n", estudiante_id);
    sem_post(&sem_estudiante);
    sem_wait(&sem_certificado);

    // El estudiante paga y recibe la carta de pago
    printf("Estudiante %d paga el importe correspondiente y recibe la carta de pago.\n", estudiante_id);
    // Simular el tiempo de pago
    sleep(2);
    printf("Estudiante %d tiene la carta de pago sellada.\n", estudiante_id);
    sem_post(&sem_pago);
    while (!certificado_listo) {
        // Esperar a que el certificado esté listo
    }

    // El estudiante recoge su certificado de notas
    printf("Estudiante %d recoge su certificado de notas.\n", estudiante_id);
    certificado_listo = 0;
    printf("Estudiante %d se retira de la secretaría.\n", estudiante_id);

    pthread_exit(NULL);
}

// Función que representa el proceso de un encargado
void *encargado(void *id) {
    while (1) {
        sem_wait(&sem_estudiante);
        printf("Encargado %d atiende a un estudiante.\n", *(int *)id);

        // Simular si el estudiante necesita un certificado o carta de pago
        if (*(int *)id == 1) {
            printf("Encargado %d entrega una carta de pago.\n", *(int *)id);
            sem_post(&sem_certificado);
        } else {
            printf("Encargado %d entrega un certificado de notas.\n", *(int *)id);
            certificado_listo = 1;
        }

        sem_wait(&sem_pago);
        printf("Encargado %d recibe el pago.\n", *(int *)id);
    }
}

int main() {
    pthread_t estudiantes[5];
    pthread_t encargados[2];
    int id_estudiantes[5] = {1, 2, 3, 4, 5};
    int id_encargados[2] = {1, 2};

    // Inicialización de semáforos
    sem_init(&sem_estudiante, 0, 0);
    sem_init(&sem_certificado, 0, 0);
    sem_init(&sem_pago, 0, 0);

    // Crear hilos para encargados
    for (int i = 0; i < 2; i++) {
        pthread_create(&encargados[i], NULL, encargado, (void *)&id_encargados[i]);
    }

    // Crear hilos para estudiantes
    for (int i = 0; i < 5; i++) {
        pthread_create(&estudiantes[i], NULL, estudiante, (void *)&id_estudiantes[i]);
    }

    // Esperar a que los hilos de estudiantes terminen
    for (int i = 0; i < 5; i++) {
        pthread_join(estudiantes[i], NULL);
    }

    return 0;
}