#include <stdio.h>
#include <stdlib.h>

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
int contador_avioes = 1;

void inicializa_Fila(Fila *f) { // Inicializa a fila
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

//
Aviao remover_Fila(Fila *f){
    Aviao av;
    NODOF aux;

    if(f->inicio == NULL){ // Se a fila estiver vazia
        printf("\nFila Vazia!\n");
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

void exibe_Aviao(Aviao av) {
    printf("\nID: %d\n", av.id);
    printf("Nome: %s\n", av.nome);
    printf("Origem: %s\n", av.origem);
    printf("Destino: %s\n", av.destino);
    printf("Numero de passageiros: %d\n", av.passageiros);
}

void exibe_Fila(Fila f) {
    NODOF aux;
    aux = f.inicio; // Auxiliar recebe o início da fila
    if(aux == NULL){ // Se a fila estiver vazia
        printf("Fila Vazia!\n");
        return;
    }
    int posicao = 1; // Variável para exibir a posição do avião na fila
    
    while(aux != NULL){ // Enquanto o auxiliar não for NULL
        printf("\nPosicao: %d", posicao);
        exibe_Aviao(aux->aviao); // Exibe os dados do avião
        aux = aux->prox; // O auxiliar recebe o próximo nó
        posicao++;
        printf("------------------------\n");
    }
}


int main(){
    inicializa_Fila(&fila_decolagem); // Inicializa a fila de decolagem
    inicializa_Fila(&fila_pouso); // Inicializa a fila de pouso
    int op;        

    do{
    printf("======== MENU ========\n");
    printf("1 - Adicionar aviao na fila de decolagem\n");
    printf("2 - Adicionar aviao na fila de pouso\n");    
    printf("3 - listar fila de decolagem\n");
    printf("4 - listar fila de pouso\n");
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
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while(op != 0);

    return 0;
}