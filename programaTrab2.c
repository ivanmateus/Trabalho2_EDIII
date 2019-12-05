//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#include <stdio.h>
#include <stdlib.h>
#include "programaTrab2.h"
#include "./src/adjList.h"
#include "./src/escreverNaTela.h"

//Compara se dois vetores são iguais
//(i.e. têm os mesmos valores nas mesmas posições)
int compareArrays(int a[], int b[], int size)	{
	for(int i = 0; i < size; ++i){
    //Se dois valores diferentes foram encontrados, retorna 0
		if(a[i] != b[i]){
			return 0;
    }
	}
  //Se os vetores forem iguais, retorna 1
	return 1;
}

//Imprime os resultados do algoritmo de Djikstra
void printDjikstra(int origin, int D[], int ANT[], indexList *graphIndex){
  //Pega o nó da lista de índices correspondente ao índice origem dado
  indexNode *index = searchByIndex(origin, graphIndex);
  //Se o nó não foi encontrado, para a execução
  if(index == NULL){
    return;
  }
  //Calcula o número de nós do grafo
  int length = indexLength(graphIndex);
  for(int i = 0; i < length; ++i) {
    //Se o índice atual do grafo não for igual a origem
    if(i != origin){
      //Então pega o seu nó na lista de índices
      indexNode *aux = searchByIndex(i, graphIndex);
      //E do seu antecessor dado pelo vetor de antecessores
      //gerado pelo algoritmo de Djikstra
      indexNode *ant = searchByIndex(ANT[i], graphIndex);
      //Se qualquer um dos dois nós de índice
      //forem NULL (o que não é possível, mas mesmo assim
      //está sendo checado), vá para a próxima iteração
      if(aux == NULL || ant == NULL){
        continue;
      }
      //Senão, imprime os dados correspondentes da origem
      printf("%s %s ", index->cidade, index->estado);
      //Do índice atual
      printf("%s %s ", aux->cidade, aux->estado);
      //A distância calculada pelo algoritmo de Djikstra dada
      //pelo vetor de distâncias D
      printf("%d ", D[i]);
      //E os dados correspondentes ao antecessor
      printf("%s %s\n", ant->cidade, ant->estado);
    }
  }
}

//Executa o algoritmo de Djikstra
int djikstra(int origin, int *D, int *ANT, adjList *G, indexList *graphIndex) {
  //Se a origem dada for -1, significa que a
  //busca pelo índice na lista de índices não encontrou
  //a cidade informada. Portanto, pare a execução.
  if(origin == -1){
    printf("Cidade inexistente.\n");
    return 0;
  }

  //Calcula o número de vértices no grafo
  int length = indexLength(graphIndex);
  //Cria duas variáveis (mesma notação utilizada em sala):
  //V: o vetor com todas as entradas do grafo
  //S: o vetor com as cidades já avaliadas pelo algoritmo
  int S[length], V[length];
  //Põe a origem no vetor S, na posição "origem" do vetor
  S[origin] = origin;
  //E põe distância 0 no vetor de distâncias, na posição "origem"
  D[origin] = 0;
  //Inicializa os vetores S e V
  for(int i = 0; i < length; ++i) {
    //V recebe todos os valores
    V[i] = i;
    //E S recebe -1 em todas as posições, com
    //exceção da posição "origem", que já foi setada
    if(i != origin) {
      S[i] = -1;
    }
  }

  //Pega a lista de adjacências da origem
  adjNode *aux = G[origin];
  //E inicializa os vetores D e ANT (de distâncias e antecessores, respectivamente)
  while(aux != NULL){
    //Para os vértices adjacentes, põe a distância correspondente
    //na posição correspondente
    D[aux->index] = aux->distancia;
    //E põe a origem como seu antecessor
    ANT[aux->index] = origin;
    aux = aux->next;
  }

  //Enquanto os vetores S e V forem diferentes
  while(!compareArrays(S, V, length)){
    //Variável que guarda o vértice de distância mínima
    int min = 0;
    //Procura um valor inicial para min que já esteja no
    //vetor S, ou seja, que já foi avaliado. Para a primeira
    //iteração, esse valor será a própria origem, que é o
    //único valor diferente de -1 no vetor
    while(S[min] != -1){
      ++min;
    }
    //Então, para todos os vértices no grafo
    for(int i = 0; i < length; ++i){
      //Se o atual não for o mínimo, sua distância for menor que a
      //do mínimo e se ele também não foi avaliado ainda
      if(i != min && D[i] < D[min] && S[i] == -1){
        //Então torna ele o novo mínimo
        min = i;
      }
      //Senão, se a distância for a mesma, eles forem diferentes
      //e o atual não foi avaliado ainda
      else if (i != min && D[i] == D[min] && S[i] == -1) {
        //Verifica qual dos vértices é o menor e torna ele o min
        min = i < min ? i : min;
      }
    }
    //Depois que um mínimo foi encontrado, coloca ele
    //no vetor S, para simbolizar que ele foi avaliado
    S[min] = min;
    //Pega a lista de adjacências do min
    aux = G[min];
    //E para todos os vértices adjacentes
    while(aux != NULL){
      //Se a distância do vértice atual for diferente da 
      //distância do min + o peso da aresta do atual
      if(D[aux->index] != D[min] + aux->distancia){
        //E se a distância do min + o peso da aresta do atual for
        //menor que a distância do vértice atual
        if(D[min] + aux->distancia < D[aux->index]){
          //Seta a distância do atual como sendo 
          //a distância do min + o peso da aresta do atual
          D[aux->index] = D[min] + aux->distancia;
          //E faz o min o antecessor do atual
          ANT[aux->index] = min;
        }        
      }
      aux = aux->next;
    }
  }
  return 1;
}

//Checa se um vértice já faz parte do componente
int isVInComponent(int U[], int V){
  if(U[V] != -1){
    return 1;
  }
  return 0;
}

//Executa o algoritmo de Prim
int prim(int origin, adjList *G, adjList *minTree, indexList *graphIndex, int length){
  //Se a origem dada for -1, significa que a
  //busca pelo índice na lista de índices não encontrou
  //a cidade informada. Portanto, pare a execução.
  if(origin == -1){
    printf("Cidade inexistente.\n");
    return 0;
  }

  //Declara duas variáveis (mesma notação utilizada em sala):
  //U: guarda os vértices que já estão no componente, ou seja,
  //já fazem parte da árvore geradora mínima.
  //V: guarda todos os vértices da árvore
  int U[length], V[length];
  //Põe a origem no componente
  U[origin] = origin;
  //Inicializa os vetores S e V
  for(int i = 0; i < length; ++i) {
    //V recebe todos os vértices
    V[i] = i;
    //U recebe -1 para todas as posições, exceto a origem
    if(i != origin) {
      U[i] = -1;
    }
  }

  //Enquanto U e V forem diferentes
  while(!compareArrays(U, V, length)){
    //Variáveis que guardam os dados da aresta (u,v) de menor peso
    int u = -1;
    int v = -1;
    //Guarda o nó da lista de adjacências correspondente à
    //aresta (u,v) mínima (minimum weight edge)
    adjNode *minWEdge = NULL;

    //Para todos os vértices do grafo
    for(int i = 0; i < length; ++i){
      //Se o vértice tiver no componente
      if(isVInComponent(U, i)){
        //Pega a lista de adjacências do vértice i
        adjNode *aux = G[i];
        //Enquanto houver vértices adjacentes
        while(aux != NULL){
          //Se o vértice em questão ainda não tiver no componente
          if(!isVInComponent(U, aux->index)){
            //Se nenhuma aresta mínima foi setada ainda, seta a atual.
            if(minWEdge == NULL){
              minWEdge = aux;
              u = i;
              v = aux->index;
            }
            //Senão, se o peso da aresta atual for diferente e menor que o peso
            //da aresta de peso mínimo, seta a atual como a mínima.
            else if(aux->distancia != minWEdge->distancia){
              if(aux->distancia < minWEdge->distancia){
                minWEdge = aux;
                u = i;
                v = aux->index;
              }
            }
            //Senão, se o peso da aresta atual for igual ao peso
            //da aresta de peso mínimo, verifica, para as arestas
            //(u,v) e (i,aux->index), seo valor do vértice i é 
            //menor que o do u ou, se eles forem iguais, verifica
            //se o valor de aux->index é menor que o do v.
            //Se qualquer uma dessas situações ocorrer, seta a aresta atual
            //como a mínima 
            else if(aux->distancia == minWEdge->distancia){
              if((i < u) || (i == u && aux->index < v)){
                minWEdge = aux;
                u = i;
                v = aux->index;
              }
            }
            aux = aux->next;
          } else {
            aux = aux->next;
          }
        }
      }
    }
    //Após achar a aresta mínima, insere ela na árvore mínima
    //Procura o nó do índice correspondente ao vértice u
    indexNode *index = searchByIndex(u, graphIndex);
    //Pega os dados da cidade/estado origem através do índice
    //achado anteriormente e os outros dados (cidade/estado destino,
    //distancia e tempoViagem) pega do nó minWEdge
    dataReg *reg = (dataReg *)malloc(sizeof(dataReg));
    strcpy(reg->estadoOrigem,index->estado);
    strcpy(reg->estadoDestino,minWEdge->estado);
    reg->distancia = minWEdge->distancia;
    reg->cidadeOrigem = index->cidade;
    reg->cidadeDestino = minWEdge->cidade;
    reg->tempoViagem = minWEdge->tempoViagem;
    //Após pegar os dados, insere somente da origem ao destino, e não
    //insere do destino para a origem (como foi feito para construir a lista
    //de adjacências), pois foi como foi especificado pela professora e pelo monitor
    insertAdjList(reg, &minTree[searchByCity(reg->cidadeOrigem,graphIndex)], graphIndex, 0);
    //E insere o vértice no vetor U para simbolizar que
    //o mesmo já foi inserido na árvore/componente
    U[v] = v;
  }
  return 1;
}

int main(void){
  char arqInp[50];
  char field[15];
	char *cidadeOrigem = (char *)malloc(sizeof(char *));
	int option = -1;
	scanf("%d %s",&option, arqInp);

  //Constroi a lista de índices que mapeia as cidades
  indexList *graphIndex = (indexList *)malloc(sizeof(indexList));
	*graphIndex = NULL;
  buildIndex(arqInp, graphIndex);
  int length = indexLength(graphIndex);  

  //Inicializa a lista de adjacências do grafo de entrada G
  adjList *G = (adjList *)malloc(sizeof(adjList *) * length);
  initAdjList(G, length);

  //Inicializa a lista de adjacências da árvore
  //geradora mínima para a funcionalidade 11 (algoritmo de Prim)
  adjList *T = (adjList *)malloc(sizeof(adjList *) * length);
  *T = NULL;
  initAdjList(T, length);

  //Inicializa os vetores de distâncias e de antecessores
  //para a funcionalidade 10 (algoritmo de Djikstra)
  int D[length], ANT[length];
  for(int i = 0; i < length; ++i) {
    ANT[i] = -1;
    D[i] = 999999999;
  }

  switch (option)	{
    //Funcionalidade 9: construir a lista de adjacências baseado no .bin
		case 9:
      buildAdjList(arqInp, G, graphIndex);			
	    printAdjList(G, graphIndex);
      break;

    //Funcionalidade 10: executar o algoritmo de Djikstra
    case 10:
			scanf("%s",field);
			scan_quote_string(cidadeOrigem);
      if(buildAdjList(arqInp, G, graphIndex)){
        if(djikstra(searchByCity(cidadeOrigem, graphIndex), D, ANT, G, graphIndex)){
          printDjikstra(searchByCity(cidadeOrigem, graphIndex), D, ANT, graphIndex);
        }
      }
      break;

    //Funcionalidade 11: executar o algoritmo de Prim
    case 11:
			scanf("%s",field);
			scan_quote_string(cidadeOrigem);
      if(buildAdjList(arqInp, G, graphIndex)){
        if(prim(searchByCity(cidadeOrigem, graphIndex), G, T, graphIndex, length)){
          printAdjList(T,graphIndex);
        }
      }
      break;

		default:
			break;
	}
  return 0;
}