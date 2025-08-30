#include <stdio.h>
#include <stlib.h>
#include <unistd.h>
#include <string.h>
#inlcude "restaurante.h"

void* rotina_chefe_montador(void* arg){
    while(1){
        pthread_mutex_lock(&mutex_servidos);
        if (pedidos_servidos >= TOTAL_PEDIDOS){
            pthread_mutex_unlock(&mutex_servidos);
            break;
        }
        pthread_mutex_unlock(&mutex_servidos);

        sem_wait(&sem_itens_no_balcao);
        sem_wait(&sem_itens_no_balcao);

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
            sem_post(&sem_pratos_limpos);
        } else{
            pthread_mutex_unlock(&mutex_balcao);
        }
    }
    return NULL;
}