#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h> // Para funções de mutex (pthread_mutex_lock, pthread_mutex_unlock)
#include <semaphore.h> // Para funções de semáforo (sem_wait, sem_post)
#include "restaurante.h"

void* rotina_garcom(void* arg){
    int id_garcom = *(int*)arg;
    free(arg);

    while (1){
        pthread_mutex_lock(&mutex_quadro);
        if (pedidos_gerados >= TOTAL_PEDIDOS){
            pthread_mutex_unlock(&mutex_quadro);
            break;
        }
        pedidos_gerados++;
        Pedido novo_pedido = { .id = pedidos_gerados};
        pthread_mutex_unlock(&mutex_quadro);

        // Um pedido requer 2 itens (bife e salada), então esperamos por 2 slots na fila
        // e criamos 2 "tarefas" para o mesmo pedido.
        sem_wait(&sem_slots_quadro_vazios);
        sem_wait(&sem_slots_quadro_vazios);

        pthread_mutex_lock(&mutex_quadro);
        quadro_de_pedidos.buffer[quadro_de_pedidos.in] = novo_pedido;
        quadro_de_pedidos.in = (quadro_de_pedidos.in + 1) % TAMANHO_QUADRO;
        quadro_de_pedidos.buffer[quadro_de_pedidos.in] = novo_pedido;
        quadro_de_pedidos.in = (quadro_de_pedidos.in + 1) % TAMANHO_QUADRO;
        quadro_de_pedidos.count += 2;
        printf("[Garcom %d]: Anotou o Pedido #%d\n", id_garcom, novo_pedido.id);
        fflush(stdout);
        pthread_mutex_unlock(&mutex_quadro);

        sem_post(&sem_slots_quadro_cheios);
        sem_post(&sem_slots_quadro_cheios);

        sleep(rand() % 3 + 1);
    }
    return NULL ;
}