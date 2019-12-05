//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#include <stdio.h>
#include <stdlib.h>
#include "adjList.h"

//Cria um novo nó para a lista de adjacências
adjNode *createAdjNode(char *cidade, char *estado, int distancia, char *tempoViagem, indexList *graphIndex){
	adjNode *new = (adjNode *)malloc(sizeof(adjNode));	//Aloca memória para o nó
	//Se não conseguiu alocar memória, retorna NULL
	if(new == NULL){
		return NULL;
	}
	//Procura pelo índice correspondente baseado na cidade
	int index = searchByCity(cidade, graphIndex);
	//E insere os dados no nó
  new->index = index;
	new->cidade = (char *)malloc(strlen(cidade) * sizeof(char));
	strcpy(new->cidade,cidade);
	strcpy(new->estado,estado);
  new->estado[2] = '\0';
	new->distancia = distancia;
	new->tempoViagem = (char *)malloc(strlen(tempoViagem) * sizeof(char));
	strcpy(new->tempoViagem,tempoViagem);
	new->next = NULL;
	return new;
}

//Inicializa uma lista de adjacências
void initAdjList(adjList *G, int length){
  for(int i = 0; i < length; ++i) {
		//Aloca memória para o ponteiro
    G[i] = (adjNode *)malloc(sizeof(adjNode));
		//E seta ele para NULL
		G[i] = NULL;
  }
}

//Insere um novo nó na lista de adjacências com os
//dados que estão contidos em um registro de dados lido.
//A função recebe uma flag "back", que é utilizada para
//indicar a direção da inserção (se é origem->destino,
//ou destino->origem)
int insertAdjList(dataReg *reg, adjList *head, indexList *graphIndex, int back){
	//Instancia um nó auxiliar para ajudar na inserção
	adjNode *aux = *head;
	//Se a flag "back" estiver setada, então a direção é
	//da cidade/estado origem à cidade/estado destino. Dessa forma, temos que
	//se a flag estiver setada, a cidade a ser inserida é a cidade origem, na
	//lista de adjacências da cidade destino. Se a flag estiver zerada, a cidade
	//inserida é a cidade destino na lista de adjacências da cidade origem.
	char *cidade = (char *)malloc(strlen(back ? reg->cidadeOrigem : reg->cidadeDestino) * sizeof(char));
	back ? strcpy(cidade,reg->cidadeOrigem) : strcpy(cidade,reg->cidadeDestino);
	char *estado = (char *)malloc(strlen(back ? reg->estadoOrigem : reg->estadoDestino) * sizeof(char));
	back ? strcpy(estado,reg->estadoOrigem) : strcpy(estado,reg->estadoDestino);

	//Se a lista tiver vazia ainda
	if(aux == NULL){
		//O novo nó se torna o cabeça da lista
		adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
    *head = new;
		return 1;
	}
	//Se a cidade a ser inserida vem antes da cidade
	//que está na cabeça da lista (em ordem alfabética),
	//insere o novo nó antes da cabeça e o torna
	//o novo cabeça
  if(strcmp(cidade, aux->cidade) < 0){
    adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
		new->next = aux;
    *head = new;
		return 1;
  }

	//Enquanto houver nós a serem lidos
	while(aux->next != NULL){
		//Se já houver uma cidade na lista, não insere novamente
		if(strcmp(cidade, aux->cidade) == 0){
			return 0;
		}
		//Se a posição da cidade for entre o nó atual (aux)
		//e o pŕoximo (aux->next), então insere o novo entre eles
    if(strcmp(cidade, aux->next->cidade) < 0){
      adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
      //Guarde o ponteiro para o próximo
			adjNode *temp = aux->next;
			//Aponte o ponteiro do atual para o novo
			aux->next = new;
			//Aponte o ponteiro do novo para o antigo pŕoximo (guardado na variável temp)
      new->next = temp;
			return 1;
    }
		aux = aux->next;
	}
	//Se a lista chegou ao fim e a nova cidade for diferente
	//da última, então insere o novo nó no fim
  if(aux->next == NULL && strcmp(cidade, aux->cidade) != 0){
    adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
		aux->next = new;
		return 1;
  }
	return 0;
}

//Constroi uma lista de adjacências baseado no arquivo .bin de entrada
int buildAdjList(char *fName, adjList *graph, indexList *index){
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
  	printf("Falha no carregamento do arquivo.\n");
   	return 0;
  }

	//Lê o registro de cabeçalho
	headerReg *hReg = (headerReg *)malloc(HREGSIZE);
	fread(&(hReg->status),STATUSSIZE,1,fp);
	fread(&(hReg->numeroVertices),VERTSIZE,1,fp);
	fread(&(hReg->numeroArestas),EDGESIZE,1,fp);
	fread(hReg->dataUltimaCompactacao,sizeof(char),LASTCOMPSIZE,fp);

	//Cria um registro auxiliar para guardar os dados temporariamente
	dataReg *reg = (dataReg *)malloc(DREGSIZE);
	
	//Cria um buffer para ler a parte variável do registro
	//de dados (i.e. os campos "cidadeOrigem",
	//"cidadeDestino" e "tempoViagem")
	char buf[VARSIZE];

	//Variável para guardar o RRN do registro atual
	int rrn = 0;

	//Se o documento não estiver consistente, pare a execução
  if(hReg->status == '0') {
    printf("Falha no carregamento do arquivo.\n");
    return 0;
  }

	//Enquanto houver registros a serem lidos no arquivo
	while(fread(reg->estadoOrigem,ORIGINSIZE,1,fp)) {
		//Se o registro não foi excluído
		if(reg->estadoOrigem[0] != '*'){
			//Leia os outros campos de tamanho fixo
			fread(reg->estadoDestino,DESTSIZE,1,fp);
			fread(&(reg->distancia),DISTANCESIZE,1,fp);
			
			//Leia o resto do registro
			fread(buf,VARSIZE,1,fp);

			//Separa os campos de acordo com o delimitador "|"
			char *bufPtr = buf;
			char *field = strsep(&bufPtr,"|");
			reg->cidadeOrigem = field;
			
			field = strsep(&bufPtr,"|");
			reg->cidadeDestino = field;
			
			field = strsep(&bufPtr,"|");
			reg->tempoViagem = field;

      //Insere na lista de adjacências nas duas direções
			//da cidade origem à cidade destino
      insertAdjList(reg, &graph[searchByCity(reg->cidadeOrigem,index)], index, 0);
      //e da cidade destino à cidade origem
			insertAdjList(reg, &graph[searchByCity(reg->cidadeDestino,index)], index, 1);
			
			//Vá para o próximo RRN
			++rrn;
		} else {
			++rrn;
			fseek(fp,DREGSIZE-ORIGINSIZE,SEEK_CUR);
		}
	}

	fclose(fp);
	return 1;
}

//Imprime a lista de adjacências
void printAdjList(adjList *G, indexList *graphIndex) {
	indexNode *auxInd = *graphIndex;
  for(int i = 0; i < indexLength(graphIndex); ++i) {
		//Imprime a cidade/estado correspondente no índice
    printf("%s %s ", auxInd->cidade, auxInd->estado);
    adjNode *auxNode = G[i];
		//E imprime a lista de adjacências
    while (auxNode != NULL){
			//Se o campo tempoViagem for vazio, não imprime
      if(strcmp(auxNode->tempoViagem,"") == 0){
        printf("%s %s %d ", auxNode->cidade, auxNode->estado, auxNode->distancia);
        auxNode = auxNode->next;
      } else {
        printf("%s %s %d %s ", auxNode->cidade, auxNode->estado, auxNode->distancia, auxNode->tempoViagem);
        auxNode = auxNode->next;
      }
    }
		//Vai para a próxima cidade no índice
		auxInd = auxInd->next;
    printf("\n");
  }
}