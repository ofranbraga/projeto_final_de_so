#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h> // Para funções de mutex (pthread_mutex_lock, pthread_mutex_unlock)
#include <semaphore.h> // Para funções de semáforo (sem_wait, sem_post)
#include "restaurante.h"

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

        pthread_mutex_lock(&mutex_quadro);
        // Verificação de segurança: se a fila estiver vazia (o que pode acontecer
        // durante o encerramento, quando o chefe nos acorda), não fazemos nada.
        // Apenas devolvemos o prato que pegamos e voltamos ao início do loop
        // para que a condição de término seja checada.
        if (quadro_de_pedidos.count == 0) {
            pthread_mutex_unlock(&mutex_quadro);
            sem_post(&sem_pratos_limpos); // Devolve o prato que não será usado.
            continue;
        }
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
        int slot = -1;

        // Procura se já existe um prato para este pedido no balcão
        for (int i = 0; i < TOTAL_PEDIDOS; i++){
            if (balcao_itens_prontos[i].em_uso && balcao_itens_prontos[i].id_pedido == p.id){
                slot = i;
                break;
            }
        }

        // Se não encontrou, procura um slot vazio no balcão
        if (slot == -1) {
            for (int i = 0; i < TOTAL_PEDIDOS; i++){
                if (!balcao_itens_prontos[i].em_uso){
                    slot = i;
                    balcao_itens_prontos[slot].em_uso = 1;
                    balcao_itens_prontos[slot].id_pedido = p.id;
                    balcao_itens_prontos[slot].count = 0;
                    break;
                }
            }
        }

        // Adiciona o item ao prato e, se estiver completo, sinaliza o chefe
        strcpy(balcao_itens_prontos[slot].itens[balcao_itens_prontos[slot].count], tipo_cozinheiro);
        balcao_itens_prontos[slot].count++;

        if (balcao_itens_prontos[slot].count == 2) {
            sem_post(&sem_prato_pronto);
        }
        pthread_mutex_unlock(&mutex_balcao);
    }
    free(tipo_cozinheiro);
    return NULL;
}