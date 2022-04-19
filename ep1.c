/* ep1.c 2022.1 - Grafos - Professor Glauber Cintra
Integrantes:
    Marcus Vinícius Alves Vieira
    Jhennyfer Rodrigues de Oliveira 
    Bruno Wilson Moura do Nascimento

*/

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define true 1
#define false 0
typedef int bool;
typedef int TIPOCUSTO;

/*

--------------- Definição de estruturas de dados para representar o Grafo ------------------

*/

// define o tipo adjacência
typedef struct adjacencia
{
    int vertice;
    TIPOCUSTO custo;
    struct adjacencia *proximo;
} ADJACENCIA;

// define o tipo vértice
typedef struct vertice
{
    ADJACENCIA *cabeca;
} VERTICE;

// cria a estrutura de um grafo
typedef struct grafo
{
    int vertices;
    int arcos;
    VERTICE *adjacente;
} GRAFO;

//--------------------------------------------------------------------------------------------

/* cria um grafo dado o numero de vertices "vertices"*/
GRAFO *criaGrafo(int vertices)
{
    GRAFO *grafo = (GRAFO *)malloc(sizeof(GRAFO));
    grafo->vertices = vertices; // atribui o numero v de vertices ao grafo
    grafo->arcos = 0;
    grafo->adjacente = (VERTICE *)malloc(vertices * sizeof(VERTICE));
    int i;
    for (i = 0; i < vertices; i++)
    {
        grafo->adjacente[i].cabeca = NULL; // define null para cada elemento no arranjo
    }
    return (grafo);
}

/* Cria adjacencia */
ADJACENCIA *criaAdjacencia(int vertice, int custo)
{
    ADJACENCIA *temp = (ADJACENCIA *)malloc(sizeof(ADJACENCIA));
    temp->vertice = vertice;
    temp->custo = custo;
    temp->proximo = NULL;
    return (temp);
}

/* Função para criar aresta */
bool criaAresta(GRAFO *grafo, int verticeInicial, int verticeFinal, TIPOCUSTO custo)
{
    if (!grafo)
        return false;
    if ((verticeFinal < 0) || (verticeFinal >= grafo->vertices))
        return (false);
    if ((verticeInicial < 0) || (verticeInicial >= grafo->vertices))
        return (false);
    ADJACENCIA *novo = criaAdjacencia(verticeFinal, custo);
    novo->proximo = grafo->adjacente[verticeInicial].cabeca;
    grafo->adjacente[verticeInicial].cabeca = novo;
    grafo->arcos++;
    return (true);
}

// usado para inicializar grafo dijkstra
void inicializaDijkstra(GRAFO *grafo, int *custos, int *predecessores, int origem)
{
    int v;
    for (v = 0; v < grafo->vertices; v++)
    {
        custos[v] = INT_MAX / 2;
        predecessores[v] = -1;
    }
    custos[origem] = 0;
}

/*Relaxar é usado para atualizar as distâncias/custos e predecessores de um vértice adjacente
  Toma os vértices (u,v) e verifica se o custo pra chegar em V, partindo de U é menor do que o custo atual para chegar em V.
  Caso seja menor, atualiza a custos[v] para a custos[u]+custo de (u,v) e poe u como predecessor de v*/
void relaxa(GRAFO *grafo, int *custos, int *predecessores, int u, int v)
{
    ADJACENCIA *adjacencia = grafo->adjacente[u].cabeca;
    while (adjacencia && adjacencia->vertice != v)
        adjacencia = adjacencia->proximo;
    if (adjacencia)
    {
        if (custos[v] > custos[u] + adjacencia->custo)
        {
            custos[v] = custos[u] + adjacencia->custo;
            predecessores[v] = u;
        }
    }
}

// percorre os vertices para saber se há um vertice aberto
bool existeVerticeAberto(GRAFO *grafo, int *aberto)
{
    int i;
    for (i = 0; i < grafo->vertices; i++)
        if (aberto[i])
            return (true);
    return (false);
}

// Dentre os abertos retorna o vertice com menor custo
int verticeComMenorDistancia(GRAFO *grafo, int *aberto, int *custos)
{
    int i;
    for (i = 0; i < grafo->vertices; i++)
        if (aberto[i])
            break;
    if (i == grafo->vertices)
        return (-1);
    int menor = i;
    for (i = menor + 1; i < grafo->vertices; i++)
        if (aberto[i] && (custos[menor] > custos[i]))
            menor = i;
    return (menor);
}

// retorna um vetor com o caminho minimo
int *encontraCaminhoMinimo(int origem, int destino, int *predecessores, int quantArestas)
{
    int aux;
    int *caminho = (int *)calloc(quantArestas, sizeof(int *));
    caminho[quantArestas - 1] = destino;
    caminho[quantArestas - 2] = predecessores[destino - 1] + 1;
    int i = 2;
    aux = predecessores[destino - 1] + 1;
    while (aux != origem + 1)
    {
        aux = predecessores[aux - 1] + 1;
        caminho[quantArestas - i - 1] = aux;
        i++;
    }

    return (caminho);
}

void imprimeCaminhoMinimo(int origem, int destino, int *caminho, int quantArestas)
{
    printf("Caminho minimo do vertice %d para o vertice %d: ", origem + 1, destino);
    for (int i = 0; i < quantArestas - 1; i++)
    {
        if (caminho[i] != 0)
        {
            printf("(%d, %d) ", caminho[i], caminho[i + 1]);
        }
    }
    printf("\n");
}

// funcao para imprimir o grafo. criada para usar em testes. nao utilizada no programa principal
void imprimeGrafo(GRAFO *grafo)
{
    printf("Vértices: %d\nArestas: %d\n", grafo->vertices, grafo->arcos);
    int i;
    for (i = 0; i < grafo->vertices; i++)
    {
        printf("v%d: ", i + 1);
        ADJACENCIA *adjacente = grafo->adjacente[i].cabeca;
        while (adjacente)
        {
            printf("v%d(%d) ", adjacente->vertice + 1, adjacente->custo);
            adjacente = adjacente->proximo;
        }
        printf("\n");
    }
}

// retorna um vetor com o menor caminho entre dois vertices
int *dijkstra(GRAFO *grafo, int origem, int destino, int quantArestas, int *custo)
{
    int *custos = (int *)malloc(grafo->vertices * sizeof(int));
    int *caminho;
    int predecessores[grafo->vertices];
    bool aberto[grafo->vertices];
    inicializaDijkstra(grafo, custos, predecessores, origem);
    int i;
    for (i = 0; i < grafo->vertices; i++)
        aberto[i] = true;

    while (existeVerticeAberto(grafo, aberto))
    {
        int u = verticeComMenorDistancia(grafo, aberto, custos);
        aberto[u] = false;
        ADJACENCIA *adjacente = grafo->adjacente[u].cabeca;
        while (adjacente)
        {
            relaxa(grafo, custos, predecessores, u, adjacente->vertice);
            adjacente = adjacente->proximo;
        }
    }

    /*verifica se o vertice destino foi alcancado. Se alcancado chama a funcao para encontrar o caminho e atribui o valor de custo.
    do contrario, atribui null ao caminho*/
    if (predecessores[destino - 1] != -1)
    {
        caminho = encontraCaminhoMinimo(origem, destino, predecessores, quantArestas);
        *custo = custos[destino - 1];
    }
    else
        caminho = NULL;

    return (caminho);
}

// abre o arquivo, e retorna o grafo correspondente, preenchendo variaveis de controle passada por referencia
GRAFO *montaGrafoDeArquivo(char *nomeArquivo, int *origem, int *destino, bool *arcoNegativo, bool *foraEscopo)
{

    int quantVertices = 0, quantArestas = 0, verticeInicial = 0, verticeDestino = 0;
    int u, v, custo_aresta;

    // abre arquivo
    FILE *arquivo = fopen(nomeArquivo, "r");

    // guarda as informacoes adquiridas pelo arquivo
    fscanf(arquivo, " %d %d %d %d", &quantVertices, &quantArestas, &verticeInicial, &verticeDestino);

    // manipula ponteiros para retorno por referencia
    *origem = verticeInicial;
    *destino = verticeDestino;

    /*testa para saber se o vertice inicial e final estão entre 1 e a quantidade de vértices*/
    if (verticeInicial < 1 || verticeDestino < 1 || verticeInicial > quantVertices || verticeDestino > quantVertices)
    {
        *foraEscopo = true;
    }

    // cria o grafo com a quantide de vértices
    GRAFO *grafo = criaGrafo(quantVertices);

    // leitura das linhas do arquivo, criando arestas no grafo criado
    for (int i = 0; i < quantArestas; i++)
    {
        fscanf(arquivo, "%d %d %d", &u, &v, &custo_aresta);
        if (custo_aresta < 0)
        {
            *arcoNegativo = true;
        }
        criaAresta(grafo, u - 1, v - 1, custo_aresta);
    }
    // fecha o arquivo
    fclose(arquivo);

    return (grafo);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
     // Erro, falta de argumentos
     printf("Argumentos insuficientes. Digite o nome do arquivo como argumento na linha de comando");
     return (0);
    }

    //variaveis de controle
    int verticeInicial = 0, verticeDestino = 0, custo = -1;
    bool arestaNegativa = false, verticeForaEscopo = false;
    
    GRAFO *grafo;
    grafo = montaGrafoDeArquivo(argv[1], &verticeInicial, &verticeDestino, &arestaNegativa, &verticeForaEscopo);

    /*testa se os vertices estao fora de escopo, se há haresta negativa ou se origem é diferente de destino*/

    if (!verticeForaEscopo && !arestaNegativa && verticeInicial != verticeDestino)
    {
        int *caminho;
        caminho = dijkstra(grafo, verticeInicial - 1, verticeDestino, grafo->arcos, &custo);
        if (caminho != NULL)
        {
            imprimeCaminhoMinimo(verticeInicial - 1, verticeDestino, caminho, grafo->arcos);
            printf("Custo: %d\n", custo);
        }
        else
            printf("Nao existe caminho do vertice %d para o vertice %d.", verticeInicial, verticeDestino);
    }
    else if (arestaNegativa)
    {
        printf("Nao e possivel usar algoritmo de dijkstra para descobrir o caminho minimo em grafo que contenha aresta de custo negativo.");
    }
    else if (verticeForaEscopo)
        printf("O vertice origem ou vertice destino estao fora do escopo do grafo.");
    else
        printf("Vertice de origem igual ao vertice de destino");

    return 0;
}
