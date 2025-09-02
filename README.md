# Simulador de Restaurante Caótico

Este repositório contém o código-fonte de um simulador de restaurante multithread em C. O projeto modela o fluxo de trabalho de um restaurante, desde a anotação dos pedidos pelos garçons até a preparação pelos cozinheiros e a montagem final pelos chefs, utilizando conceitos de programação concorrente para gerenciar as interações entre os diferentes "funcionários" (threads).

A simulação implementa uma solução para o clássico problema do produtor-consumidor, onde diferentes entidades produzem e consomem recursos de buffers compartilhados de forma sincronizada e segura.

## Visão Geral do Simulador

O simulador é composto por quatro tipos principais de threads, cada uma representando uma função no restaurante:

* **Garçons:** Responsáveis por gerar novos pedidos e colocá-los em um quadro de pedidos compartilhado (buffer).
* **Cozinheiros (de Bife e de Salada):** Retiram os itens dos pedidos do quadro, preparam o item específico de sua especialidade (bife ou salada) e o colocam em um balcão de itens prontos.
* **Chefe Montador:** Aguarda até que todos os itens de um mesmo pedido (um bife e uma salada) estejam prontos no balcão. Quando o prato está completo, ele monta e serve, finalizando o pedido.

### Mecanismos de Sincronização

Para garantir que as threads acessem os recursos compartilhados (como o quadro de pedidos e o balcão) de forma segura e sem condições de corrida, o projeto utiliza os seguintes mecanismos da biblioteca POSIX Threads:

* **Mutexes:** Usados para garantir exclusão mútua ao acessar seções críticas do código, como a inserção e remoção de itens das estruturas de dados compartilhadas.
* **Semáforos:** Utilizados para controlar o acesso a recursos com capacidade limitada (como o número de pratos limpos) e para sincronizar as ações entre produtores e consumidores (por exemplo, garçons esperando por espaço no quadro e cozinheiros esperando por novos pedidos).

### Estrutura do Código

O projeto é organizado de forma modular, com a lógica de cada entidade e as configurações separadas em diferentes arquivos:

* `main.c`: Ponto de entrada da aplicação. Responsável por inicializar as estruturas de dados, os semáforos, os mutexes e por criar e gerenciar todas as threads.
* `garcom.c`: Contém a lógica da thread do garçom.
* `cozinheiro.c`: Contém a lógica da thread do cozinheiro.
* `chefe_montador.c`: Contém a lógica da thread do chefe montador.
* `restaurante.h`: Arquivo de cabeçalho que define as estruturas de dados compartilhadas (quadro de pedidos, balcão) e declara as variáveis globais, semáforos e protótipos de funções.
* `config.h`: Arquivo de configuração onde é possível ajustar os parâmetros da simulação, como o número de garçons, cozinheiros, o total de pedidos, etc.
* `Makefile`: Define as regras para compilar e executar o projeto de forma automatizada.

## Como Compilar e Executar

Para compilar e executar o simulador, você precisará ter o compilador `gcc` e o `make` instalados no seu sistema. O projeto utiliza a biblioteca `pthread`, que geralmente já está incluída em sistemas baseados em Linux.

Siga os passos abaixo:

1.  **Clone ou faça o download do repositório.**
    ```bash
    git clone https://github.com/ofranbraga/projeto_final_de_so.git
    ```

2.  **Abra um terminal na pasta raiz do projeto.**

3.  **Para compilar o projeto**, execute o seguinte comando. Ele irá compilar os arquivos-fonte e criar um executável chamado `restaurante_caotico`.

    ```bash
    make all
    ```

4.  **Para executar a simulação**, use o comando:

    
    ```bash
    ./restaurante_caotico
    ```

5.  **Para limpar os arquivos gerados** (o executável e os arquivos objeto), você pode usar o comando:

    ```bash
    make clean
    ```