#include <stdio.h>
#include <stdlib.h>

//COMPILAÇÃO DE CÓDIGO DIFERENTES -- NÃO MEXER
#ifdef _WIN32
    #include <windows.h>
    void gotoxy(int x, int y) {
        COORD coord;
        coord.X = x;
        coord.Y = y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }
    void configura_terminal() {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD dwMode = 0;
        GetConsoleMode(hOut, &dwMode);
        SetConsoleMode(hOut, dwMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        system("chcp 437 > nul");
    }
    void espera(int ms) { Sleep(ms); }
#else
    #include <unistd.h>
    void gotoxy(int x, int y) { printf("\033[%d;%dH", y, x); }
    void configura_terminal() {}
    void espera(int ms) { usleep(ms * 1000); }
#endif

void limpa_tela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Tipo avião
typedef struct aviao {
    char nome[50];
    int id;
    char origem[50];
    char destino[50];
    int passageiros;
} Aviao;

typedef struct nodo_Fila {
    Aviao aviao; // Dados do avião
    struct nodo_Fila *prox; // Ponteiro para o próximo nodo
} *NODOF;

typedef struct { // Estrutura da Fila
    NODOF inicio; // Ponteiro para o início da fila
    NODOF fim; // Ponteiro para o fim da fila
    int tamanho; // Tamanho da fila
} Fila;

Fila fila_decolagem; // Variável global para a fila de decolagem
Fila fila_pouso; // Variável global para a fila de pouso
int contador_avioes = 1; // Contador de ID


// PROTOPIPOS
void inicializa_Fila(Fila *f);
NODOF Cria_Nodo();
int FilaVazia(Fila *f);
void inserir_Fila(Fila *f);
Aviao remover_Fila(Fila *f);
void exibe_Aviao(Aviao av);
void exibe_Fila(Fila f);
void primeiro_Fila(Fila f);
void autoriza_Decolagem();
void autoriza_Pouso();
void anima_Decolagem(Aviao av);
void anima_Pouso(Aviao av);

void inicializa_Fila(Fila *f) { // Inicializa a fila zerada
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

// Função que cria / aloca um novo nó
NODOF Cria_Nodo() {
    NODOF p;
    p = (NODOF) malloc(sizeof(struct nodo_Fila));
    if (!p) {
        printf("Problemas na Alocacao!!!");
        exit(0);
    }
    return p;
}

// Função para verificar se a fila está vazia
int FilaVazia(Fila *f) {
    return (f->inicio == NULL);
}

// Função para inserir NO FIM 
void inserir_Fila(Fila *f){ 
    NODOF novo;
    novo = Cria_Nodo();
    
    novo->aviao.id = contador_avioes; // Atribui um ID único ao avião
    printf("ID: %d\n", novo->aviao.id); // Exibe o ID do avião
    contador_avioes++; // Incrementa o contador para o próximo avião

    printf("Nome do aviao: ");
    scanf("%s", novo->aviao.nome);
    
    printf("Origem: ");
    scanf("%s", novo->aviao.origem);
    
    printf("Destino: ");
    scanf("%s", novo->aviao.destino);
    
    printf("Numero passageiros: ");
    scanf("%d", &novo->aviao.passageiros);

    novo->prox = NULL; // O próximo do novo nó é NULL, pois ele será o último da fila

    if(f->inicio == NULL){ // Se a fila estiver vazia
        f->inicio = novo; // O início da fila recebe o novo nó
        f->fim = novo; // O fim da fila recebe o novo nó
    }else{
        f->fim->prox = novo; // O próximo do fim da fila recebe o novo nó
        f->fim = novo; // O fim da fila recebe o novo nó
    }

    f->tamanho++; // Incrementa o tamanho da fila
    printf("\nAviao %s adicionado na fila!\n", novo->aviao.nome);
}

// Função para remover do inicio
Aviao remover_Fila(Fila *f){
    Aviao av;
    NODOF aux;

    if(FilaVazia(f)){ // Se a fila estiver vazia
        printf("\nFila Vazia!\n");
        av.id = -1;
        return av; // Retorna um avião vazio
    }

    aux = f->inicio; // Auxiliar recebe o início da fila
    av = aux->aviao; // Av recebe os dados do avião que será removido
    f->inicio = f->inicio->prox; // O início da fila recebe o próximo do início

    if(f->inicio == NULL)  // Se a fila ficar vazia após a remoção, o fim também deve ser NULL
        f->fim = NULL;

    free(aux); // Libera o nó removido  
    f->tamanho--; // Decrementa o tamanho da fila
    return av; // Retorna os dados do avião removido
    
}

// Função para exibir os dados de um avião
void exibe_Aviao(Aviao av) {
    printf("\nID: %d\n", av.id);
    printf("Nome: %s\n", av.nome);
    printf("Origem: %s\n", av.origem);
    printf("Destino: %s\n", av.destino);
    printf("Numero de passageiros: %d\n", av.passageiros);
}

// Função para exibir os dados de todos os aviões na fila
void exibe_Fila(Fila f) {
    if(FilaVazia(&f)){ // Se a fila estiver vazia
        printf("Fila Vazia!\n");
        return;
    }
    
    NODOF aux; // Auxiliar para percorrer a fila
    aux = f.inicio; // Auxiliar recebe o início da fila
    int posicao = 1; // Variável para exibir a posição do avião na fila

    while(aux != NULL){ // Enquanto o auxiliar não for NULL
        printf("\nPosicao: %d", posicao);
        exibe_Aviao(aux->aviao); // Exibe os dados do avião
        aux = aux->prox; // O auxiliar recebe o próximo nó
        posicao++; // Incrementa a posição para o próximo avião
        printf("------------------------\n");
    }
}

void primeiro_Fila(Fila f) {
    if(FilaVazia(&f)){ // Se a fila estiver vazia
        printf("Fila Vazia!\n");
        return;
    }
    printf("\nPrimeiro aviao da fila:\n");
    exibe_Aviao(f.inicio->aviao); // Exibe os dados do primeiro avião da fila
}

void autoriza_Decolagem() {
    if(FilaVazia(&fila_decolagem)){ // Se a fila de decolagem estiver vazia 
        printf("\nNenhum aviao esperando para decolar!\n");
        return;
    }
    printf("\nAviao autorizado a decolar:\n");
    primeiro_Fila(fila_decolagem); // Exibe os dados do primeiro avião da fila de decolagem
    printf("\nConfirmar decolagem? [1]Sim [0]Nao: ");
    
    int op; 
    scanf("%d", &op);

    if(op != 1){ // Se a opção for diferente de 1, cancela a decolagem
        printf("\nDecolagem cancelada!\n");
        return;  
    }

    limpa_tela(); // Limpa a tela antes de animar a decolagem
    Aviao av = remover_Fila(&fila_decolagem); // Remove o primeiro avião da fila de decolagem e armazena os dados em av
    anima_Decolagem(av);  // Anima a decolagem do avião
    limpa_tela(); // Limpa a tela após a animação da decolagem
}

void autoriza_Pouso() {
        if(FilaVazia(&fila_pouso)){
        printf("\nNenhum aviao esperando para pousar!\n");
        return;
    }
    printf("\nAviao autorizado a pousar:\n");
    primeiro_Fila(fila_pouso); // Exibe os dados do primeiro avião da fila de pouso
    printf("\nConfirmar pouso? [1]Sim [0]Nao: ");

    int op;
    scanf("%d", &op);
    if(op != 1){ // Se a opção for diferente de 1, cancela o pouso
        printf("\nPouso cancelado!\n");
        return; 
    }

    limpa_tela();
    Aviao av = remover_Fila(&fila_pouso); // Remove o primeiro avião da fila 
    anima_Pouso(av); 
    limpa_tela();

}

int main(){
    configura_terminal(); 
    inicializa_Fila(&fila_decolagem); // Inicializa a fila de decolagem
    inicializa_Fila(&fila_pouso); // Inicializa a fila de pouso
    int op;        

    do{
        printf("======== MENU ========\n");
        printf("1 - Adicionar aviao na fila de decolagem\n");
        printf("2 - Adicionar aviao na fila de pouso\n");    
        printf("3 - listar fila de decolagem\n");
        printf("4 - listar fila de pouso\n");
        printf("5 - Mostrar primeiro aviao da fila de decolagem\n");
        printf("6 - Mostrar primeiro aviao da fila de pouso\n");
        printf("7 - Autorizar decolagem do primeiro aviao da fila de decolagem\n");
        printf("8 - Autorizar pouso do primeiro aviao da fila de pouso\n");
        printf("9  - Quantidade de avioes esperando decolagem\n");
        printf("10 - Quantidade de avioes esperando pouso\n");  
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &op);

        switch(op){
            case 1:
                inserir_Fila(&fila_decolagem);
                break;
            case 2:
                inserir_Fila(&fila_pouso);
                break;
            case 3:
                printf("\nFila de Decolagem:\n");
                exibe_Fila(fila_decolagem);
                break;
            case 4:
                printf("\nFila de Pouso:\n");
                exibe_Fila(fila_pouso);
                break;
            case 5:
                primeiro_Fila(fila_decolagem);
                break;
            case 6:
                primeiro_Fila(fila_pouso);
                break;
            case 7:
                autoriza_Decolagem();
                break;
            case 8:
                autoriza_Pouso();
                break;
            case 9:
                printf("\nQuantidade de avioes esperando decolagem: %d\n", fila_decolagem.tamanho);
                break;
            case 10:
                printf("\nQuantidade de avioes esperando pouso: %d\n", fila_pouso.tamanho);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(op != 0);

    return 0;
}

//// ANIMAÇÃO DA TELA

void anima_Decolagem(Aviao av) {
    int x, y, px, py; 

    limpa_tela();

    gotoxy(1, 20); // Imprime a pista
    printf("================================================================");
    gotoxy(1, 21); // Imprime o nome do aeroporto de origem
    printf("[ %s ]", av.origem);
    gotoxy(1, 22); // Imprime os dados do avião
    printf("Aviao: %s | Destino: %s | Passageiros: %d", av.nome, av.destino, av.passageiros);

    x = 2; y = 19; // Posição inicial do avião na pista
    px = x; py = y; // Variáveis para armazenar a posição anterior do avião, usadas para apagar o avião na posição antiga
    while (x < 20) { // o avião corre pela pista até a posição de decolagem
        gotoxy(px, py); 
        printf("      "); // Apaga o avião na posição anterior
        gotoxy(x,  y);  
        printf("AVIAO"); // Imprime o avião na nova posição
        espera(100); // Espera um pouco para criar a animação
        px = x; // Atualiza a posição anterior para a atual 
        py = y; // Atualiza a posição anterior para a atual
        x += 2; // Move o avião para a direita, simulando a corrida pela pista
    }

    while (y > 2) { // o avião sobe até sair da tela
        gotoxy(px, py); 
        printf("      "); // Apaga o avião na posição anterior
        gotoxy(x,  y);  
        printf("AVIAO"); // Imprime o avião na nova posição
        espera(120); 
        px = x; // Atualiza a posição anterior para a atual 
        py = y; // Atualiza a posição anterior para a atual
        x += 2; // Move o avião para a direita
        y--; // Move o avião para cima, simulando a decolagem
    }

    gotoxy(px, py);  
    printf("      ");  // apaga último
    gotoxy(1, 2); // Imprime a mensagem de que o avião decolou
    printf("Aviao %s decolou!", av.nome); // Exibe o nome do avião que decolou
    espera(1000); 
    limpa_tela();
}
void anima_Pouso(Aviao av) {
    int x, y, px, py;

    limpa_tela();

    gotoxy(1, 20); // Imprime a pista
    printf("================================================================================");
    gotoxy(1, 21); // Imprime o nome do aeroporto de destino
    printf("[ %s ]", av.destino); 
    gotoxy(1, 22); // Imprime os dados do avião
    printf("Aviao: %s | Origem: %s | Passageiros: %d", av.nome, av.origem, av.passageiros);


    x = 60; // Posição inicial x do avião no céu
    y = 2;  // Posição inicial y do avião no céu
    px = x; // Variáveis para armazenar a posição x anterior do avião, usadas para apagar o avião na posição antiga
    py = y; // Variáveis para armazenar a posição y anterior do avião, usadas para apagar o avião na posição antiga
    while (y < 19) { // o avião desce até a pista
        gotoxy(px, py); 
        printf("      "); // Apaga o avião na posição anterior
        gotoxy(x,  y);  
        printf("AVIAO"); // Imprime o avião na nova posição
        espera(120);
        px = x; py = y; x -= 2; y++;
    }

    // limpa transição
    gotoxy(px, py); 
    printf("      "); // Apaga o avião na posição anterior

    px = x; // Atualiza a posição anterior para a atual
    py = y; 
    while (x > 2) { // o avião corre pela pista até a posição 
        gotoxy(px, py); 
        printf("      "); // Apaga o avião na posição anterior
        gotoxy(x,  y);  
        printf("AVIAO"); // Imprime o avião na nova posição
        espera(100);
        px = x; // Atualiza a posição anterior para a atual
        py = y; 
        x -= 2; // Move o avião para a esquerda
    }

    gotoxy(px, py); 
    printf("      "); // Apaga o avião na posição anterior
    gotoxy(1, 2);
    printf("Aviao %s pousou!", av.nome); // Exibe o nome do avião que pousou
    espera(1000);
    limpa_tela();
}