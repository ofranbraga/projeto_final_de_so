#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "restaurante.h"

//definicao das variaveis globais 
// aqui as variaveis declaradas como 'extern' em restaurante.h sao finalmente definidas
QuadroDePedidos quadro_de_pedidos;
PratoDePreparo balcao_itens_prontos[TOTAL_PEDIDOS];
int pedidos_gerados = 0;
int pedidos_servidos = 0;

pthread_mutex_t mutex_quadro;
pthread_mutex_t mutex_balcao;
pthread_mutex_t mutex_servidos;
sem_t sem_pratos_limpos;
sem_t sem_slots_quadro_cheios;
sem_t sem_slots_quadro_cheios;
sem_t sem_itens_no_balcao;

//funcao principal
int main(){
    srand(time(NULL));

    //inicializa mutexes e semaforos
    pthread_mutex_init(&mutex_quadro, NULL);
    pthread_mutex_init(&mutex_balcao, NULL);
    pthread_mutex_init(&mutex_servidos, NULL);
    sem_init(&sem_pratos_limpos, 0, TOTAL_PRATOS);
    sem_init(&sem_slots_quadro_vazios, 0, TAMANHO_QUADRO);
    sem_init(&sem_slots_quadro_cheios, 0, 0);
    sem_init(&sem_itens_no_balcao, 0, 0);

    //inicializa estruturas de dados
    quadro_de_pedidos.in = 0;
    quadro_de_pedidos.out = 0;
    quadro_de_pedidos.count = 0;
    for (int i = 0; i < TOTAL_PEDIDOS; i++){
        balcao_itens_prontos[i].em_uso = 0;
    }
    
    printf("Restaurante aberto!\n");

    //vetores de threads
    for (int i = 0; i < NUM_GARCONS; i++){
        int* id = malloc(sizeof(int)); *id = i + 1;
        pthread_creat(&garcons[i], NULL, rotina_garcom, id);
    }
    for (int i = 0; i < NUM_COZINHEIROS_BIFE; i++){
        char* tipo = malloc(sizeof(char) * 10); strcpy(tipo, "Bife");
        pthread_create(&cozinheiros_bife[i], NULL, rotina_cozinheiro, tipo);
    }
    for (int i = 0; i < NUM_COZINHEIROS_SALADA; i++){
        char* tipo = malloc(sizeof(char) * 10); strcpy(tipo, "Salada");
        pthread_create(&cozinheiros_salada[i], NULL, rotina_cozinheiro, tipo);
    }
    for (int i = 0; i < NUM_CHEFS_MONTAGEM; i++){
        pthread_create(&chefes_montagem[i], NULL, rotina_chefe_montador, NULL);
    }

    //espera todas as threads terminarem
    for (int i = 0; i < NUM_GARCONS; i++) pthread_join(garcons[i], NULL);
    for (int i = 0; i < NUM_COZINHEIROS_BIFE; i++) pthread_join(cozinheiros_bife[i], NULL);
    for (int i = 0; i < NUM_COZINHEIROS_SALADA; i++) pthread_join(cozinheiros_salada[i], NULL);
    for (int i = 0; i < NUM_CHEFS_MONTAGEM; i++) pthread_join(chefes_montagem[i], NULL);

    //limpeza de recursos 
    ptrhead_mutex_destroy(&mutex_quadro);
    ptrhead_mutex_destroy(&mutex_balcao);
    ptrhead_mutex_destroy(&mutex_servidos);
    sem_destroy(&sem_pratos_limpos);
    sem_destroy(&sem_slots_quadro_vazios);
    sem_destroy(&sem_slots_quadro_cheios);
    sem_destroy(&sem_itens_no_balcao);

    printf("Restaurante Fechado! Todos os %d pedidos foram servidos.\n", TOTAL_PEDIDOS);

    return 0;
}