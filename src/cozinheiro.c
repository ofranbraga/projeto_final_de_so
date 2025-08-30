#include <stdio.h>
#include <stlib.h>
#include <unistd.h>
#include <string.h>
#inlcude "restaurante.h"

void* rotina_cozinheiro(void* arg){
    char* tipo_cozinheiro = (char*)arg;

    while (1){
        pthread_mutex_lock(&mutex_servidos);
        if (pedidos_servidos >= TOTAL_PEDIDOS){
            pthread_mutex_unlock(&mutex_servidos);
            break;
        }
        pthread_mutex_unlock(&mutex_servidos);

        sem_wait(&sem_slots_quadro_cheios);
        sem_wait(&sem_pratos_limpos);

        pthread_mutex_lock(&mutex_servidos);
        Pedido p = quadro_de_pedidos.buffer[quadro_de_pedidos.out];
        quadro_de_pedidos.out = (quadro_de_pedidos.out + 1) % TAMANHO_QUADRO;
        quadro_de_pedidos.count--;
        pthread_mutex_unlock(&mutex_quadro);

        sem_post(&sem_slots_quadro_vazios);

        printf("[Cozinheiro de %s]: Pegou o Pedido #%d. Preparando...\n", tipo_cozinheiro, p.id);
        fflush(stdout);
        sleep(rand() % 4+2);
        printf("\033[32m[Cozinheiro de %s]: Terminou item para o Pedido #%d.\033[0m\n", tipo_cozinheiro, p.id);
        fflush(stdout);

        pthread_mutex_lock(&mutex_balcao);
        int i, found = 0;
        for (i = 0; i < TOTAL_PEDIDOS; i++){
            if (balcao_itens_prontos[i].em_uso && balcao_itens_prontos[i].id_pedido == p.id){
                strcpy (balcao_itens_prontos[i].itens[balcao_itens_prontos[i].count++], tipo_cozinheiro);
                found = 1;
                break;
            }
        }
        if (!found){
            for (i = 0; i < TOTAL_PEDIDOS; i++){
                if (!balcao_itens_prontos[i].em_uso){
                    balcao_itens_prontos[i].em_uso = 1;
                    balcao_itens_prontos[i].id_pedido = p.id;
                    balcao_itens_prontos[i].count = 0;
                    strcpy(balcao_itens_prontos[i].itens[balcao_itens_prontos[i].count++], tipo_cozinheiro);
                    break;
                }
            }
        }
        pthread_mutex_unlock(&mutex_balcao);

        sem_post(&sem_itens_no_balcao);
    }
    free(tipo_cozinheiro);
    return NULL;
}