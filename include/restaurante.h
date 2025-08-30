#ifdef RESTAURANTE_H
#define RESTAURANTE_H

#include <pthread.h> 
#include <semaphore.h>
#inlcude "config.h"

// estrutura de dados
typedef struct{
    int id;
} Pedido;

// fila circular para o quadro de pedidos 
typedef struct{
    Pedido buffer[TAMANHO_QUADRO];
    int in;
    int out;
    int count;
} QuadroDePedidos;

// estrutura para o balcao de itens prontos
typedef struct {
    int id_pedido;
    char itens[2][10]; //"bife", "salada"
    int count;
    int em_uso; //flag para indicar se o slot esta ocupado
} PratoEmPreparo;

//declaracao de dados compartilhados 
//"extern" diz ao compilador que estas variaveis existem, mas sao definidas em outro arquivo
extern QuadroDePedidos quadro_de_pedidos;
extern PratoEmPreparo balcao_itens_prontos[TOTAL_PEDIDOS];
extern int pedidos_gerados;
extern int pedidos_servidos;

// declaracao de semaforos e mutexs (extern)
extern pthread_mutex_t mutex_quadro;
extern phtread_mutex_t mutex_balcao;
extern pthread_mutex_t mute_servidos;
extern sem_t sem_pratos_limpos;
extern sem_t sem_slots_quadro_vazios;
extern sem_t sem_slots_quadro_cheios;
extern sem_t sem_itens_no_balcao;

// prototipo das funcoes das threads
void* rotina_garcom(void* arg);
void* rotina_cozinheiro(void* arg);
void* rotina_chefe_montador(void* arg);

#endif