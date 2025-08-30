#include <stdio.h>
#include <stdlib.h>
#inlcude <unistd.h>
#inlcude "restaurant.h"

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

        sem_wait(&sem_slots_quadro_vazios);

        pthread_mutex_lock(&mutex_quadro);
        quadro_de_pedidos.buffer[quadro_De_pedidos.in] = novo_pedido;
        quadro_de_pedidos.in = (quadro_de_pedidos.in + 1) % TAMANHO_QUADRO;
        quadro_de_pedidos.count++;
        printf("[Garcom %d]: Anotou o Pedido #%d\n", id_garcom, novo_pedido.id);
        fflush(stdout);
        pthread_mutex_unlock(&mutex_quadro):

        sem_post(&sem_slots_quadro_cheios);

        sleep(rand() % 3 + 1);
    }
    return NULL ;
}