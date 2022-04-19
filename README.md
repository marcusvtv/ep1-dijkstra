# ep1-dijkstra
Escreva um programa que receba pela linha de comando o nome de um arquivo (contendo a descrição de um digrafo). O arquivo fornecido ao programa deverá ter o seguinte layout: 


n m s t

u1 v1 c1

u2 v2 c2

...

um vm cm


onde n é a quantidade de vértices, m a quantidade de arcos, s é o vértice origem, t é o vértice destino, ui e vi são a origem e o destino do arco i e ci é o custo da aresta i. Por exemplo, para o digrafo abaixo, se s = 1 e t = 4, teríamos o arquivo à direita do grafo.

5 7 1 4

1 2 5

1 5 10

2 3 4

2 4 13

2 5 2

3 4 7

5 4 6

Utilizando o Algoritmo de Dijkstra, seu programa deverá calcular e exibir um caminho de custo mínimo de s a t. Para o grafo acima, a saída do programa poderia ser:

Caminho mínimo do vértice 1 para o vértice 4: (1, 2) (2, 5) (5, 4)

Custo: 13

Siga rigorosamente as especificações contidas neste texto. Não se esqueça de colocar comentários no seu programa, indentar o código, usar nomes de variáveis significativos, modularizar o código, enfim, programar com estilo. Programas com erros de compilação serão rejeitados.
 
