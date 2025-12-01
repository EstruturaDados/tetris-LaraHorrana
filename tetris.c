#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para a geracao de pecas aleatorias

// ====================================================================
// DEFINICOES GLOBAIS
// ====================================================================

// Capacidades fixas conforme os requisitos
#define TAMANHO_FILA 5    // Fila Circular (peças futuras)
#define TAMANHO_PILHA 3   // Pilha (peças de reserva)

// ====================================================================
// 1. ESTRUTURA DE DADOS
// ====================================================================

// Struct para representar a Peça (tipo + id)
typedef struct {
    char nome;          // Tipo da peça ('I', 'O', 'T', 'L', etc.)
    int id;             // Identificador unico
} Peca;

// ====================================================================
// 2. VARIÁVEIS GLOBAIS PARA AS ESTRUTURAS
// (Pratica comum em projetos simples do primeiro ano para simplificar a logica)
// ====================================================================

// --- ESTRUTURA DA FILA CIRCULAR ---
Peca fila[TAMANHO_FILA];    // Array para armazenar a Fila
int frente = 0;             // Indice da frente (proxima a ser removida/jogada)
int tras = -1;              // Indice do tras (ultima inserida)
int contadorFila = 0;       // Numero atual de elementos na fila

// --- ESTRUTURA DA PILHA LINEAR ---
Peca pilha[TAMANHO_PILHA];  // Array para armazenar a Pilha
int topo = -1;              // Indice do topo (-1 indica pilha vazia)

// --- CONTADOR GLOBAL DE IDS ---
int G_proximoId = 0;        // Garante que cada peca tenha um ID unico

// ====================================================================
// 3. FUNÇÕES AUXILIARES DE GERAÇÃO E VISUALIZAÇÃO
// ====================================================================

// Requisito: Gerar nova peca automaticamente.
// Gera uma peca com tipo aleatorio e ID unico.
Peca gerarPeca() {
    Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L'}; // Peças padrao do Tetris
    int indexAleatorio;
    
    // Gera um numero aleatorio entre 0 e 3 para selecionar o tipo
    indexAleatorio = rand() % 4; 

    novaPeca.nome = tipos[indexAleatorio];
    novaPeca.id = G_proximoId++; // Atribui o ID e incrementa o contador global

    return novaPeca;
}

// Requisito: Exibir o estado da Pilha apos cada acao.
void visualizarPilha() {
    printf("Pilha de Reserva (Topo -> Base): ");
    if (topo == -1) {
        printf("Vazia.");
    } else {
        // Percorre a pilha do topo para a base (LIFO)
        for (int i = topo; i >= 0; i--) {
            printf("[%c %d] ", pilha[i].nome, pilha[i].id);
        }
    }
    printf("\n");
}

// Requisito: Exibir o estado da Fila apos cada acao.
void visualizarFila() {
    printf("Fila de Pecas (Frente -> Tras): ");
    if (contadorFila == 0) {
        printf("Vazia.");
    } else {
        // Usa a logica circular para percorrer o array do elemento 'frente' ate o 'tras'
        for (int i = 0; i < contadorFila; i++) {
            // Formula para calcular a posicao atual na fila circular
            int indice = (frente + i) % TAMANHO_FILA; 
            printf("[%c %d] ", fila[indice].nome, fila[indice].id);
        }
    }
    printf("\n");
}

// Requisito: Exibir o estado atual (Fila e Pilha)
void exibirEstadoAtual() {
    printf("\n==================================\n");
    visualizarFila();
    visualizarPilha();
    printf("==================================\n");
}


// ====================================================================
// 4. FUNÇÕES DA FILA CIRCULAR (FIFO) - ENQUEUE E DEQUEUE
// ====================================================================

// Requisito: Inserir nova peça ao final da fila (enqueue).
int enqueue(Peca novaPeca) {
    if (contadorFila == TAMANHO_FILA) {
        // Overflow: a fila esta cheia
        return 0; 
    }
    
    // Move o ponteiro 'tras' para a proxima posicao (logica circular)
    tras = (tras + 1) % TAMANHO_FILA; 
    fila[tras] = novaPeca;
    contadorFila++;
    
    return 1; // Sucesso
}

// Requisito: Jogar/Remover a peca da frente da fila (dequeue).
Peca dequeue() {
    Peca pecaRemovida = {'\0', -1}; // Inicializa uma peca vazia para retorno de erro
    
    if (contadorFila == 0) {
        // Underflow: a fila esta vazia
        printf("[ERRO] Fila de peças vazia! Nao ha o que jogar.\n");
        return pecaRemovida;
    }
    
    pecaRemovida = fila[frente];
    
    // Move o ponteiro 'frente' para a proxima posicao (logica circular)
    frente = (frente + 1) % TAMANHO_FILA; 
    contadorFila--;
    
    // Se a fila esvaziou, resetamos o 'tras' para o estado inicial
    if (contadorFila == 0) {
         tras = -1;
         frente = 0;
    }

    return pecaRemovida; // Sucesso
}


// ====================================================================
// 5. FUNÇÕES DA PILHA LINEAR (LIFO) - PUSH E POP
// ====================================================================

// Função Push (empilhar)
int push(Peca peca) {
    if (topo == TAMANHO_PILHA - 1) {
        // Overflow: a pilha esta cheia
        return 0; 
    }

    topo++;
    pilha[topo] = peca;
    return 1; // Sucesso
}

// Função Pop (desempilhar)
Peca pop() {
    Peca pecaRemovida = {'\0', -1};
    
    if (topo == -1) {
        // Underflow: a pilha esta vazia
        printf("[ERRO] Pilha de reserva vazia! Nao ha o que usar.\n");
        return pecaRemovida;
    }

    pecaRemovida = pilha[topo];
    topo--;
    return pecaRemovida; // Sucesso
}

// ====================================================================
// 6. LÓGICA DO JOGO E MENU
// ====================================================================

// Implementa a acao de jogar a peca da frente da fila.
void jogarPeca() {
    Peca pecaJogada = dequeue();
    
    if (pecaJogada.id != -1) {
        printf("[ACAO] Peça '%c %d' JOGADA da Fila.\n", pecaJogada.nome, pecaJogada.id);
        
        // Requisito: A cada acao, uma nova peca é gerada e inserida na fila.
        // A fila deve ser mantida cheia (se possivel).
        if (enqueue(gerarPeca())) {
            printf("[INFO] Nova peca gerada e adicionada ao final da Fila.\n");
        }
    }
    exibirEstadoAtual();
}

// Requisito: Mover a peca da frente da fila para o topo da pilha.
void reservarPeca() {
    // 1. Verificar se a Pilha tem espaco
    if (topo == TAMANHO_PILHA - 1) {
        printf("[ALERTA] Pilha de reserva esta CHEIA. Nao e possivel reservar.\n");
        exibirEstadoAtual();
        return;
    }
    
    // 2. Tenta remover da Fila
    Peca pecaParaReserva = dequeue();
    if (pecaParaReserva.id == -1) {
        // A fila estava vazia, erro ja foi tratado no dequeue
        exibirEstadoAtual(); 
        return;
    }

    // 3. Tenta adicionar na Pilha
    push(pecaParaReserva);
    printf("[ACAO] Peça '%c %d' RESERVADA (movida da Fila para o Topo da Pilha).\n", pecaParaReserva.nome, pecaParaReserva.id);
    
    // 4. Gera nova peca para manter a Fila cheia
    if (enqueue(gerarPeca())) {
         printf("[INFO] Nova peca gerada e adicionada ao final da Fila.\n");
    }
    exibirEstadoAtual();
}

// Requisito: Remover a peca do topo da pilha, simulando seu uso.
void usarPecaReservada() {
    Peca pecaUsada = pop();
    
    if (pecaUsada.id != -1) {
        printf("[ACAO] Peça '%c %d' USADA (removida do Topo da Pilha).\n", pecaUsada.nome, pecaUsada.id);
        
        // A acao de "usar" a peca nao afeta a fila, entao nao gera nova peca.
        // Requisito Aventureiro/Mestre: "A cada ação, uma nova peça é gerada e adicionada ao final da fila, mantendo-a sempre cheia."
        // O requisito é confuso se deve gerar peca no pop. Vamos seguir o 'jogar' e 'reservar'. 
        // Vamos assumir que "Usar uma peça reservada" *não* conta como ação de jogo para gerar uma peça nova, por simplificação.
    }
    exibirEstadoAtual();
}


// ====================================================================
// 7. FUNÇÕES DE TROCA ESTRATÉGICA (NÍVEL MESTRE)
// ====================================================================

// Requisito: Trocar peça atual (frente da fila) com o topo da pilha.
void trocarPecaAtual() {
    if (contadorFila == 0) {
        printf("[ERRO] Fila vazia. Nao e possivel trocar a peca da frente.\n");
        exibirEstadoAtual();
        return;
    }
    if (topo == -1) {
        printf("[ERRO] Pilha vazia. Nao ha peca para trocar.\n");
        exibirEstadoAtual();
        return;
    }
    
    // Troca simples de valores
    Peca temp = fila[frente];      // Peca da fila (frente)
    fila[frente] = pilha[topo];    // Peca da pilha vai para a frente da fila
    pilha[topo] = temp;            // Peca da fila vai para o topo da pilha
    
    printf("[ACAO] TROCA SIMPLES realizada: Frente da Fila <-> Topo da Pilha.\n");
    exibirEstadoAtual();
}

// Requisito: Troca múltipla (3 primeiras da fila com 3 da pilha).
void trocarMultipla() {
    if (contadorFila < 3) {
        printf("[ERRO] Fila tem menos de 3 pecas (%d). Troca multipla cancelada.\n", contadorFila);
        exibirEstadoAtual();
        return;
    }
    if (topo < 2) { // Pilha precisa ter indices 0, 1 e 2 (3 peças)
        printf("[ERRO] Pilha tem menos de 3 pecas (%d). Troca multipla cancelada.\n", topo + 1);
        exibirEstadoAtual();
        return;
    }
    
    printf("[ACAO] TROCA MULTIPLA em andamento...\n");
    
    // Logica da Troca Multipla:
    // Trocar as pecas da Fila (indices: frente, (frente+1)%T, (frente+2)%T)
    // com as pecas da Pilha (indices: topo, topo-1, topo-2)
    
    for (int i = 0; i < 3; i++) {
        int indiceFila = (frente + i) % TAMANHO_FILA; // Indice circular na fila
        int indicePilha = topo - i;                   // Indice simples na pilha (do topo para baixo)
        
        // Troca de valores usando uma Peca temporaria
        Peca temp = fila[indiceFila];
        fila[indiceFila] = pilha[indicePilha];
        pilha[indicePilha] = temp;
    }

    printf("[SUCESSO] TROCA MULTIPLA realizada: 3 primeiras da Fila <-> 3 da Pilha.\n");
    exibirEstadoAtual();
}


// ====================================================================
// 8. FUNÇÃO MAIN E MENU PRINCIPAL
// ====================================================================

// Funcao que inicializa a Fila com 5 pecas
void inicializarFila() {
    printf("[SETUP] Inicializando a Fila de peças...\n");
    // Garante que a fila comece cheia, como no requisito
    for (int i = 0; i < TAMANHO_FILA; i++) {
        enqueue(gerarPeca());
    }
    printf("[SETUP] Fila inicializada com %d peças.\n", contadorFila);
}

void exibirMenu() {
    printf("\n--- MENU DE ACOES TETRIS STACK ---\n");
    printf("1. Jogar peça (Dequeue)\n");             // Nivel Novato
    printf("2. Reservar peça (Fila -> Pilha)\n");   // Nivel Aventureiro
    printf("3. Usar peça reservada (Pop)\n");       // Nivel Aventureiro
    printf("4. Trocar peça da frente da Fila <-> Topo da Pilha\n"); // Nivel Mestre
    printf("5. Trocar as 3 primeiras da Fila <-> 3 da Pilha\n");    // Nivel Mestre
    printf("0. Sair\n");
    printf("------------------------------------\n");
    printf("Escolha uma opcao: ");
}

int main() {
    int opcao;
    
    srand(time(NULL)); // Inicializa a semente para numeros aleatorios

    printf("==================================================\n");
    printf("   GERENCIADOR DE PEÇAS TETRIS STACK (FINAL) \n");
    printf("==================================================\n");

    inicializarFila();
    exibirEstadoAtual();

    do {
        exibirMenu();
        if (scanf("%d", &opcao) != 1) {
            // Limpa o buffer de entrada em caso de input invalido
            while(getchar() != '\n'); 
            opcao = -1; 
        }

        switch (opcao) {
            case 1: jogarPeca(); break;
            case 2: reservarPeca(); break;
            case 3: usarPecaReservada(); break;
            case 4: trocarPecaAtual(); break;
            case 5: trocarMultipla(); break;
            case 0:
                printf("\nEncerrando o gerenciador de peças. Adeus!\n");
                break;
            default:
                printf("\n[ERRO] Opcao invalida. Tente novamente.\n");
        }
        
    } while (opcao != 0);

    return 0;
}