#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

// Declaración de semáforos y variables compartidas
sem_t sem_estudiante, sem_certificado, sem_pago;
int certificado_listo = 0;
clock_t tiempo_inicial;

// Función que representa el proceso de un estudiante
void *estudiante(void *id) {
    int estudiante_id = *(int *)id; // Identificador del estudiante

    // El estudiante llega a la secretaría
    printf("Estudiante %d llega a la secretaría.\n", estudiante_id); // Simular el tiempo de llegada
    printf("Tiempo transcurrido: %.2f segundos\n", ((double)(clock() - tiempo_inicial)) / CLOCKS_PER_SEC);


    // El estudiante solicita un certificado de notas
    printf("Estudiante %d solicita un certificado de notas.\n", estudiante_id); // Simular el tiempo de espera
    sem_post(&sem_estudiante); // Despertar a un encargado
    sem_wait(&sem_certificado); // Esperar a que el encargado entregue el certificado

    // El estudiante paga y recibe la carta de pago
    printf("Estudiante %d paga el importe correspondiente y recibe la carta de pago.\n", estudiante_id);
    // Simular el tiempo de pago
    sleep(2); 
    printf("Estudiante %d tiene la carta de pago sellada.\n", estudiante_id); // Simular el tiempo de espera
    sem_post(&sem_pago); // Despertar a un encargado
    while (!certificado_listo) { 
        // Esperar a que el certificado esté listo
    }

    // El estudiante recoge su certificado de notas
    printf("Estudiante %d recoge su certificado de notas.\n", estudiante_id); // Simular el tiempo de espera
    certificado_listo = 0; // Reiniciar la variable
    printf("Estudiante %d se retira de la secretaría.\n", estudiante_id); // Simular el tiempo de salida

    pthread_exit(NULL); // Terminar el hilo
}

// Función que representa el proceso de un encargado
void *encargado(void *id) { // Identificador del encargado (1 o 2)
    while (1) { // Ciclo infinito para que el encargado siempre esté disponible
        sem_wait(&sem_estudiante); // Esperar a que un estudiante solicite un certificado
        printf("Encargado %d atiende a un estudiante.\n", *(int *)id);// Simular el tiempo de atención y entrega

        // Simular si el estudiante necesita un certificado o carta de pago
        if (*(int *)id == 1) { // Encargado 1 entrega carta de pago
            printf("Encargado %d entrega una carta de pago.\n", *(int *)id); // Simular el tiempo de entrega de la carta
            sem_post(&sem_certificado); // Despertar al estudiante para que pague
        } else { // Encargado 2 entrega certificado de notas
            printf("Encargado %d entrega un certificado de notas.\n", *(int *)id); // Simular el tiempo de entrega del certificado
            certificado_listo = 1; // Indicar que el certificado está listo
        }

        sem_wait(&sem_pago); // Esperar a que el estudiante pague
        printf("Encargado %d recibe el pago.\n", *(int *)id); // Simular el tiempo de pago
    }
}

int main() { // Función principal
    pthread_t estudiantes[5]; // Arreglo de hilos para estudiantes
    pthread_t encargados[2]; // Arreglo de hilos para encargados
    int id_estudiantes[5] = {1, 2, 3, 4, 5}; // Identificadores de los estudiantes
    int id_encargados[2] = {1, 2}; // Identificadores de los encargados
    tiempo_inicial = clock();

    // Inicialización de semáforos
    sem_init(&sem_estudiante, 0, 0); // Inicializar el semáforo del estudiante en 0
    sem_init(&sem_certificado, 0, 0); // Inicializar el semáforo del certificado en 0
    sem_init(&sem_pago, 0, 0); // Inicializar el semáforo del pago en 0

    // Crear hilos para encargados
    for (int i = 0; i < 2; i++) { // Crear 2 hilos para encargados
        pthread_create(&encargados[i], NULL, encargado, (void *)&id_encargados[i]); // Crear el hilo del encargado
    }

    // Crear hilos para estudiantes
    for (int i = 0; i < 5; i++) { // Crear 5 hilos para estudiantes 
        pthread_create(&estudiantes[i], NULL, estudiante, (void *)&id_estudiantes[i]); // Crear el hilo del estudiante
    }

    // Esperar a que los hilos de estudiantes terminen su ejecución
    for (int i = 0; i < 5; i++) {
        pthread_join(estudiantes[i], NULL); 
    }

    return 0;
}