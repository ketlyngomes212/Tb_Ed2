Aviao remover_Fila(Fila *f){
    Aviao av;
    NODOF aux;

    if(f->inicio == NULL){ // Se a fila estiver vazia
        printf("\nFila Vazia!\n");
        return;
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