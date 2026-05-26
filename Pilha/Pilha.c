// bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// remove o #include <conio.h> daqui!

#define tam 30
#define livre 0          
#define parede 1        
#define visitada 2      
#define beco 3   

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
    #include <termios.h>  // ← aqui no else, só para Mac!

    void gotoxy(int x, int y) {
        printf("\033[%d;%dH", y, x);
    }

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

// Estrutura Pilha
typedef struct Nodo_Pilha {
    int elemento;           // Codernadas
    struct Nodo_Pilha *prox;
} *NODOP;

// Variável global 
NODOP pilha = NULL;

// Função que cria um novo nodo
NODOP Cria_Nodo() {
    NODOP p;
    p = (NODOP) malloc(sizeof(struct Nodo_Pilha));
    if (!p) {
        printf("Problemas na Alocacao!!!");
        exit(0);
    }
    return p;
}

// Função que coloca na pilha
void push(int dado) {
    NODOP nodo; // cria nó
    nodo = Cria_Nodo(); //aloca nó
    nodo->elemento = dado; // recebe posição
    nodo->prox = pilha; // aponta pro primeiro
    pilha = nodo; // se torna o primeiro da pilha
}

// Função que tira da pilha
int pop() {
    NODOP nodo; // cria nó
    int elem; 
    
    if (!pilha) { // se a pilha estiver vazia retorna
        return -1;
    }
    
    nodo = pilha; // novo nó recebe o primeiro elemento (topo da pilha)
    elem = nodo->elemento; //pega o valor que está no topo da pilha
    pilha = nodo->prox; // faz a pilha apontar pro 2 elemento
    free(nodo); // libera topo pilha
    return elem; // retorna elemento
}

// Função para verificar se a está pilha vazia
int PilhaVazia() {
    return (pilha == NULL);
}

/// LABIRINTO
int lab[tam][tam];

void inicializa_Labirinto() {
    srand(time(NULL)); // semente aleatória diferente a cada execução
    
    for (int i = 0; i < tam; i++) {
        for (int j = 0; j < tam; j++) {
            
            if (i == 0 || i == tam-1 || j == 0 || j == tam-1) { // se for borda
                lab[i][j] = parede; // Viram parede
            } else { //senao
                lab[i][j] = (rand() % 10 < 2) ? parede : livre; // Ficam livres
            }
        }
    }
    
    lab[2][2] = livre; // posição inicial do rato
    lab[tam-2][tam-1] = livre; // saída
}

void exibe_Labirinto(int rato_i, int rato_j) {

    int offset_y = 2;
    int offset_x = 1;

    for (int i = 0; i < tam; i++) {

        gotoxy(offset_x, i + offset_y);

        for (int j = 0; j < tam; j++) {

            if (i == rato_i && j == rato_j)
                printf(" @");

            else if (lab[i][j] == livre)
                printf("  ");

            else if (lab[i][j] == parede)
                printf("%c%c", 219, 219);

            else if (lab[i][j] == visitada)
                printf("..");

            else if (lab[i][j] == beco)
                printf("%c%c", 178, 178);
        }
    }

    gotoxy(0, tam + offset_y + 3);
}

void resolve_Labirinto(){
    int i = 2; // Linha inicial do rato
    int j = 2; // Coluna inicial do rato

    while(!(i == tam-2 && j == tam-1)) { // Enquanto o rato não chegar na saída
        lab[i][j] = visitada; // Marca a posição atual como visitada
        exibe_Labirinto(i, j); // Exibe o labirinto
        Sleep(10);

        if(lab[i][j-1] == livre) {        // esquerda
            push(i * 100 + j);
            j--;
        } else if(lab[i+1][j] == livre) { // baixo
            push(i * 100 + j);
            i++;
        } else if(lab[i][j+1] == livre) { // direita
            push(i * 100 + j);
            j++;
        } else if(lab[i-1][j] == livre) { // cima
            push(i * 100 + j);
            i--;
        } else {
            lab[i][j] = beco;
            if(PilhaVazia()) {
                gotoxy(0,tam+6);
                printf("Labirinto sem saida!");
                return;
            }
            int val = pop();
            i = val / 100;
            j = val % 100;
        }
    }
    exibe_Labirinto(i, j); // Exibe o labirinto com o rato na saída
    gotoxy(0, tam + 6); // move o cursor para a linha abaixo
    printf("\nRato encontrou a saida!\n");
}

int main() {
    configura_terminal();
    limpa_tela();
    inicializa_Labirinto();
    resolve_Labirinto();
    return 0;
}

