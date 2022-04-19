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

// retorna um vetor com as custos do vertice de origem para os outros vértices
void dijkstra(GRAFO *grafo, int origem, int destino, int quantArestas)
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

    /*verifica se o vertice destino foi alcancado. Se alcancado prossiga para exibir o caminho.
    do contrario, exibe a mensagem de não existe caminho entre os dois vértices.*/
    if (predecessores[destino - 1] != -1)
    {
        caminho = encontraCaminhoMinimo(origem, destino, predecessores, quantArestas);
        imprimeCaminhoMinimo(origem, destino, caminho, quantArestas);
        printf("Custo: %d\n", custos[destino - 1]);
    }
    else
        printf("Não existe caminho entre os vertices  %d e %d.\n", origem, destino);
}

// abre o arquivo, cria o grafo correspondente e chama dijkstra
void iniciaPrograma(char *nomeArquivo)
{

    int quantVertices = 0, quantArestas = 0, verticeInicial = 0, verticeDestino = 0;
    int u, v, custo_aresta;
    int *custo, **arestas;
    bool arestaNegativa = false;

    // abre arquivo
    FILE *arquivo = fopen(nomeArquivo, "r");

    // guarda as informacoes adquiridas pelo arquivo
    fscanf(arquivo, " %d %d %d %d", &quantVertices, &quantArestas, &verticeInicial, &verticeDestino);

    /*testa para saber se o vertice inicial e final estão entre 1 e a quantidade de vértices*/
    if (verticeInicial < 1 || verticeDestino < 1 || verticeInicial > quantVertices || verticeDestino > quantVertices)
    {
        printf("O vertice origem ou destino esta fora do grafo.");
    }
    else // caso contrário, cria o grafo e prossegue
    {

        // cria o grafo com a quantide de vértices
        GRAFO *grafo = criaGrafo(quantVertices);

        // leitura das linhas do arquivo, criando arestas no grafo criado
        for (int i = 0; i < quantArestas; i++)
        {
            fscanf(arquivo, "%d %d %d", &u, &v, &custo_aresta);
            if (custo_aresta < 0)
                arestaNegativa = true;
            else
                criaAresta(grafo, u - 1, v - 1, custo_aresta);
        }

        // fecha o arquivo
        fclose(arquivo);

        // testa se tem aresta de custo negativo
        if (!arestaNegativa)
        {
            // testa se o vertice inicial é diferente do vertice destino. Se forem diferentes, executa dijkstra, do contrário imprime custo zero.
            if (verticeInicial != verticeDestino)
            {
                dijkstra(grafo, verticeInicial - 1, verticeDestino, quantArestas);
            }
            else
            {
                printf("Caminho minimo do vertice %d para o vertice %d: (%d, %d)\n", verticeInicial, verticeDestino, verticeInicial, verticeDestino);
                printf("Custo: 0");
            }
        }
        else
            printf("Nao e possivel calcular caminho minimo em um grafo que contenha aresta de custo negativo usando o algoritmo de dijkstra.");
    }
}

int main(int argc, char *argv[])
{
    iniciaPrograma(argv[1]);
    return 0;
}
