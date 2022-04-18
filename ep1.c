#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define true 1
#define false 0
typedef int bool;

int **alocaMatriz(int linhas, int colunas) {
    int i, j;
    int **m;
    m = (int **)calloc(linhas, sizeof(int*));
    if (m == NULL) {
        return NULL;
    }

    for (i = 0; i < linhas; i++) {
        m[i] = (int *)calloc(colunas, sizeof(int*));
        if (m[i] == NULL) {
            for (j = 0; j < i; j++) {
                free(m[j]);
            }
            free(m);
            return NULL;
        }
    }
    return m;
}

void imprimeMatriz(int **m, int linhas, int colunas) {
    int i, j;
    for (i = 0 ; i < linhas; i++) {
        for (j = 0 ; j < colunas; j++) {
            printf(" %d ", m[i][j]);
        }
        printf("\n");
    }
}

int *alocaVetor(int qntdPosicoes) {
    int *vetorDinamico;
    vetorDinamico = (int *)calloc(qntdPosicoes, sizeof(int*));
    if(vetorDinamico == NULL){
        return NULL; // Memória Insuficiente
    }
    return vetorDinamico;
}

void imprimeVetor(int *m, int tamanho) {
    int i;
    for (i = 0; i < tamanho; i++) {
        printf("%d ", m[i]);
    }
}

int iniciaPrograma() {

    int quantVertices = 0, quantArestas = 0, verticeInicial = 0, verticeDestino = 0;
    int *custo, **arestas;
    char nomeArquivo[400];

    printf("Digite o nome do arquivo de texto:\n");
    scanf(" %s", nomeArquivo);
    printf("\n");

    // abre arquivo

    FILE *arquivo = fopen(nomeArquivo, "r");

    if (arquivo == NULL) {
        perror("Error");
        printf("problema ao abrir o arquivo. Verifique se o arquivo de texto se encontra no mesmo diretório do programa ou se está vazio.");
        return 1;
    }

    // guarda as informacoes adquiridas pelo arquivo
    fscanf (arquivo, " %d %d %d %d", &quantVertices, &quantArestas, &verticeInicial, &verticeDestino);
    printf("%d %d %d %d", quantVertices, quantArestas, verticeInicial, verticeDestino);
    printf("\n");

    arestas = alocaMatriz(quantArestas, 2);
    custo = alocaVetor(quantArestas);

    // guarda arestas em forma de matriz, e custo em um vetor
    for (int i = 0; i < quantArestas; i++) {
        for (int j = 0; j < 1; j++) {
            fscanf(arquivo, "%d %d %d", &arestas[i][j], &arestas[i][j+1], &custo[i]);
        }
    }

    fclose(arquivo);
    imprimeMatriz(arestas, quantArestas, 2);
    imprimeVetor(custo, quantArestas);

}

int main(void) {
    iniciaPrograma();
    return (0);
}