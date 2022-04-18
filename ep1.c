#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <conio.h>
#define true 1
#define false 0
typedef int bool;
typedef int TIPOCUSTO;

// define o tipo adjacência
typedef struct adjacencia {
    int vertice;
    TIPOCUSTO custo;
    struct adjacencia *proximo;
} ADJACENCIA;

// define o tipo vértice
typedef struct vertice
{
    ADJACENCIA *cabeca;
} VERTICE;

//cria a estrutura de um grafo
typedef struct grafo
{
    int vertices;
    int arcos;
    VERTICE *adjacente;
} GRAFO;

/* cria um grafo dado o numero de vertices "vertices"*/
GRAFO *criaGrafo(int vertices){
    GRAFO *grafo = (GRAFO *) malloc(sizeof(GRAFO));
    grafo->vertices = vertices; //atribui o numero v de vertices ao grafo
    grafo->arcos = 0;
    grafo->adjacente = (VERTICE *)malloc(vertices*sizeof(VERTICE));
    int i;
    for (i = 0; i < vertices; i++)
    {
        grafo->adjacente[i].cabeca = NULL; //define null para cada elemento no arranjo
    }
    return(grafo);
}

ADJACENCIA *criaAdjacencia(int vertice, int custo){
    ADJACENCIA *temp = (ADJACENCIA *) malloc(sizeof(ADJACENCIA));
    temp->vertice = vertice;
    temp->custo = custo;
    temp->proximo = NULL;
    return(temp);
}

bool criaAresta(GRAFO *grafo, int verticeInicial, int verticeFinal, TIPOCUSTO custo){
    if(!grafo) return false;
    if((verticeFinal<0)||(verticeFinal>=grafo->vertices)) return(false);
    if((verticeInicial<0)||(verticeInicial>=grafo->vertices)) return(false);
    ADJACENCIA *novo = criaAdjacencia(verticeFinal,custo);
    novo->proximo = grafo->adjacente[verticeInicial].cabeca;
    grafo->adjacente[verticeInicial].cabeca = novo;
    grafo->arcos++;
    return(true);
}

void imprimeGrafo(GRAFO *grafo){
    printf("Vértices: %d\nArestas: %d\n", grafo->vertices, grafo->arcos);
    int i;
    for (i = 0; i < grafo->vertices; i++)
    {
        printf("v%d: ",i);
        ADJACENCIA *adjacente = grafo->adjacente[i].cabeca;
        while(adjacente) {
            printf("v%d(%d) ", adjacente->vertice, adjacente->custo);
            adjacente = adjacente->proximo;
        }
        printf("\n");
    }
}

//usado para inicializar grafo dijkstra
void inicializaDijkstra(GRAFO *grafo, int *custos, int *predecessores, int origem){
    int v;
    for(v=0;v<grafo->vertices; v++){
        custos[v] = INT_MAX/2;
        predecessores[v] = -1;
    }
    custos[origem] = 0;
}


/*Relaxar é usado para atualizar as distâncias/custos e predecessores de um vértice adjacente
  Toma os vértices (u,v) e verifica se o custo pra chegar em V, partindo de U é menor do que o custo atual para chegar em V.
  Caso seja menor, atualiza a custos[v] para a custos[u]+custo de (u,v) e poe u como predecessor de v*/

void relaxa(GRAFO *grafo, int *custos, int *predecessores, int u, int v){
    ADJACENCIA *adjacencia = grafo->adjacente[u].cabeca;
    while (adjacencia && adjacencia->vertice != v)
        adjacencia = adjacencia->proximo;
    if (adjacencia) {
        if(custos[v] > custos[u] + adjacencia->custo) {
            custos[v] = custos[u] + adjacencia->custo;
            predecessores[v] = u;
        }
    }
}

//percorre os vertices para saber se há um vertice aberto
bool existeVerticeAberto(GRAFO *grafo, int *aberto){
    int i;
    for(i=0; i<grafo->vertices; i++)
        if (aberto[i]) return(true);
    return(false);
}

// Dentre os abertos retorna o vertice com menor custo
int verticeComMenorDistancia(GRAFO *grafo, int *aberto, int *custos){
    int i;
    for(i=0; i<grafo->vertices; i++)
        if (aberto[i]) break;
    if( i== grafo->vertices) return(-1);
    int menor = i;
    for(i=menor+1; i < grafo->vertices; i++)
        if (aberto[i] && (custos[menor]>custos[i]))
            menor = i;
    return(menor);
}

//retorna um vetor com as custos do vertice de origem para os outros vértices
int *dijkstra(GRAFO *grafo, int origem) {
    int *custos = (int *) malloc(grafo->vertices*sizeof(int));
    int predecessores[grafo->vertices];
    bool aberto[grafo->vertices];
    inicializaDijkstra(grafo, custos, predecessores ,origem);
    int i;
    for(i=0; i<grafo->vertices;i++)
        aberto[i] = true;
  
    while(existeVerticeAberto(grafo,aberto)){
        int u = verticeComMenorDistancia(grafo,aberto,custos);
        aberto[u] = false;
        ADJACENCIA *adjacente = grafo->adjacente[u].cabeca;
        while(adjacente){
            relaxa(grafo,custos,predecessores,u,adjacente->vertice);
            adjacente = adjacente->proximo;
        }
    }
    return(custos);
}

GRAFO *iniciaPrograma(char* nomeArquivo) {

    int quantVertices = 0, quantArestas = 0, verticeInicial = 0, verticeDestino = 0;
    int u,v,custo_aresta;
    int *custo, **arestas;
    /*
    printf("Digite o nome do arquivo de texto:\n");
    scanf(" %s", nomeArquivo);
    printf("\n");
*/
    // abre arquivo

    FILE *arquivo = fopen(nomeArquivo, "r");

    // guarda as informacoes adquiridas pelo arquivo
    fscanf (arquivo, " %d %d %d %d", &quantVertices, &quantArestas, &verticeInicial, &verticeDestino);
    
    // cria o grafo com a quantide de vértices
    GRAFO *grafo = criaGrafo(quantVertices);


    // guarda arestas em forma de matriz, e custo em um vetor
    for (int i = 0; i < quantArestas; i++) {
        for (int j = 0; j < 1; j++) {
            fscanf(arquivo, "%d %d %d", &u, &v, &custo_aresta);            
            criaAresta(grafo, u-1, v-1, custo_aresta);
        }
    }
    fclose(arquivo);
    
    
    int *r;
    int vertice = 0;
    int i;

    r = dijkstra(grafo, verticeInicial-1);
    printf("Custo: D(v%d -> v%d) = %d\n", vertice+1, verticeDestino, r[verticeDestino-1]);
    return(grafo);
}

int main( int argc, char *argv[ ] )
{
    GRAFO *grafo = iniciaPrograma(argv[1]);
    getch();
    return 0;
}
