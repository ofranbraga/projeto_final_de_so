#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> // Para funções de mutex (pthread_mutex_lock, pthread_mutex_unlock)
#include <semaphore.h> // Para funções de semáforo (sem_wait, sem_post)
#include "restaurante.h"

void* rotina_chefe_montador(void* arg){
    while(1){
        pthread_mutex_lock(&mutex_servidos);
        if (pedidos_servidos >= TOTAL_PEDIDOS){
            pthread_mutex_unlock(&mutex_servidos);
            break;
        }
        pthread_mutex_unlock(&mutex_servidos);

        sem_wait(&sem_prato_pronto); // Espera por um prato completo
 
        pthread_mutex_lock(&mutex_balcao);
        int id_pedido_pronto = -1;
        int index_prato = -1;
        for (int i = 0; i < TOTAL_PEDIDOS; i++){
            if (balcao_itens_prontos[i].em_uso && balcao_itens_prontos[i].count == 2){
                id_pedido_pronto = balcao_itens_prontos[i].id_pedido;
                index_prato = i;
                break;
            }
        }
        if (id_pedido_pronto != -1){
            printf("\033[34m[Chefe Montador]: Itens do Pedido #%d prontos! Montando... \033[0m\n", id_pedido_pronto);
            fflush(stdout);

            balcao_itens_prontos[index_prato].em_uso = 0;
            pthread_mutex_unlock(&mutex_balcao);

            sleep(2);

            pthread_mutex_lock(&mutex_servidos);
            pedidos_servidos++;
            printf("\033[35m[SISTEMA]: Pedido #%d SERVIDO! Total servidos: %d/%d\033[0m\n", id_pedido_pronto, pedidos_servidos, TOTAL_PEDIDOS);
            fflush(stdout);
            pthread_mutex_unlock(&mutex_servidos);

            sem_post(&sem_pratos_limpos);
            sem_post(&sem_pratos_limpos); // Libera os dois "pratos" usados pelos cozinheiros
        } else{
            pthread_mutex_unlock(&mutex_balcao);
        }
    }

    // O trabalho do chefe acabou. Para garantir que o programa termine corretamente,
    // precisamos "acordar" todas as outras threads que possam estar bloqueadas
    // em um semáforo, esperando por um trabalho que nunca chegará.
    int i;
    int total_cozinheiros = NUM_COZINHEIROS_BIFE + NUM_COZINHEIROS_SALADA;
    for (i = 0; i < total_cozinheiros; i++) sem_post(&sem_slots_quadro_cheios);
    for (i = 0; i < NUM_CHEFS_MONTAGEM; i++) sem_post(&sem_prato_pronto);

    return NULL;
}