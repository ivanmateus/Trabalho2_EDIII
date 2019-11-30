#include <stdio.h>
#include <stdlib.h>
#include "programaTrab2.h"
#include "./src/adjList.h"
#include "./src/escreverNaTela.h"

void buildIndex(char *fName, indexList *graphIndex){
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
  	printf("Falha no processamento do arquivo.\n");
   	return;
  }

	//Read the header register
	headerReg *hReg = (headerReg *)malloc(HREGSIZE);
	fread(&(hReg->status),STATUSSIZE,1,fp);
	fread(&(hReg->numeroVertices),VERTSIZE,1,fp);
	fread(&(hReg->numeroArestas),EDGESIZE,1,fp);
	fread(hReg->dataUltimaCompactacao,sizeof(char),LASTCOMPSIZE,fp);

	//Create an auxiliary register to temporarily store the data
	dataReg *reg = (dataReg *)malloc(DREGSIZE);
	
	//Create a buffer to read the data of the variable
	//part of the register (i.e. the fields "cidadeOrigem",
	//"cidadeDestino" and "tempoViagem")
	char buf[VARSIZE];

	//Variable to store the RRN of the current register
	int rrn = 0;

  if(hReg->status == '0') {
    printf("Falha no carregamento do arquivo.\n");
    return;
  }

	while(fread(reg->estadoOrigem,ORIGINSIZE,1,fp)) {
		if(reg->estadoOrigem[0] == '\n'){
			printf("Falha na execução da funcionalidade.\n");
			return;				
		} else if(reg->estadoOrigem[0] != '*'){
			//Read and print the fixed size fields
			fread(reg->estadoDestino,DESTSIZE,1,fp);
			fread(&(reg->distancia),DISTANCESIZE,1,fp);
			
			//Read the rest of the register
			fread(buf,VARSIZE,1,fp);

			//Separate the fields using the "|" delimiter
			//then print
			char *bufPtr = buf;
			char *field = strsep(&bufPtr,"|");
			reg->cidadeOrigem = field;
			
			field = strsep(&bufPtr,"|");
			reg->cidadeDestino = field;
			
      insertIndex(reg->cidadeOrigem, reg->estadoOrigem, graphIndex);
      insertIndex(reg->cidadeDestino, reg->estadoDestino, graphIndex);
			
			//Go to the next RRN
			++rrn;
		} else {
			++rrn;
			fseek(fp,DREGSIZE-ORIGINSIZE,SEEK_CUR);
		}
	}

	fclose(fp);
}

void buildAdjList(char *fName, adjList *graph, indexList *index){
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
  	printf("Falha no processamento do arquivo.\n");
   	return;
  }

	//Read the header register
	headerReg *hReg = (headerReg *)malloc(HREGSIZE);
	fread(&(hReg->status),STATUSSIZE,1,fp);
	fread(&(hReg->numeroVertices),VERTSIZE,1,fp);
	fread(&(hReg->numeroArestas),EDGESIZE,1,fp);
	fread(hReg->dataUltimaCompactacao,sizeof(char),LASTCOMPSIZE,fp);

	//Create an auxiliary register to temporarily store the data
	dataReg *reg = (dataReg *)malloc(DREGSIZE);
	
	//Create a buffer to read the data of the variable
	//part of the register (i.e. the fields "cidadeOrigem",
	//"cidadeDestino" and "tempoViagem")
	char buf[VARSIZE];

	//Variable to store the RRN of the current register
	int rrn = 0;

  if(hReg->status == '0') {
    printf("Falha no carregamento do arquivo.\n");
    return;
  }

	while(fread(reg->estadoOrigem,ORIGINSIZE,1,fp)) {
		if(reg->estadoOrigem[0] == '\n'){
			printf("Falha na execução da funcionalidade.\n");
			return;				
		} else if(reg->estadoOrigem[0] != '*'){
			//Read the fixed size fields
			fread(reg->estadoDestino,DESTSIZE,1,fp);
			fread(&(reg->distancia),DISTANCESIZE,1,fp);
			
			//Read the rest of the register
			fread(buf,VARSIZE,1,fp);

			//Separate the fields using the "|" delimiter
			//then print
			char *bufPtr = buf;
			char *field = strsep(&bufPtr,"|");
			reg->cidadeOrigem = field;
			
			field = strsep(&bufPtr,"|");
			reg->cidadeDestino = field;
			
			field = strsep(&bufPtr,"|");
			reg->tempoViagem = field;

      //Insert the node in the adjacence list
      insertAdjList(reg, &graph[searchByCity(reg->cidadeOrigem,index)], index, 0);
      insertAdjList(reg, &graph[searchByCity(reg->cidadeDestino,index)], index, 1);
			
			//Go to the next RRN
			++rrn;
		} else {
			++rrn;
			fseek(fp,DREGSIZE-ORIGINSIZE,SEEK_CUR);
		}
	}

	fclose(fp);
}

int compareArrays(int a[], int b[], int size)	{
	for(int i = 0; i < size; ++i){
		if(a[i] != b[i]){
			return 0;
    }
	}
	return 1;
}

void printDjikstra(int origem, int D[], int ANT[], indexList *graphIndex){
  indexNode *index = searchByIndex(origem, graphIndex);
  int length = indexLength(graphIndex);
  for(int i = 0; i < length; ++i) {
    if(i != origem){
      indexNode *aux = searchByIndex(i, graphIndex);
      indexNode *ant = searchByIndex(ANT[i], graphIndex);
      printf("%s %s ", index->cidade, index->estado);
      printf("%s %s ", aux->cidade, aux->estado);
      printf("%d ", D[i]);
      printf("%s %s\n", ant->cidade, ant->estado);
    }
  }
}

void djikstra(int origem, adjList *G, indexList *graphIndex) {
  int length = indexLength(graphIndex);
  int S[length], V[length], D[length], ANT[length];
  S[origem] = origem;
  for(int i = 0; i < length; ++i) {
    V[i] = i;
    ANT[i] = -1;
    if(i != origem) {
      S[i] = -1;
      D[i] = 999999999;
    }
  }
  D[origem] = 0;
  adjNode *aux = G[origem];
  aux = aux->next;
  while(aux != NULL){
    D[aux->index] = aux->distancia;
    ANT[aux->index] = origem;
    aux = aux->next;
  }

  while(!compareArrays(S, V, length)){
    int less = 0;
    while(S[less] != -1){
      ++less;
    }
    for(int i = 0; i < length; ++i){
      if(i != less && D[i] < D[less] && S[i] == -1){
        less = i;
      } else if (i != less && D[i] == D[less] && S[i] == -1) {
        less = i < less ? i : less;
      }
    }
    S[less] = less;
    aux = G[less];
    aux = aux->next;
    while(aux != NULL){
      if(D[aux->index] != D[less] + aux->distancia){
        if(D[less] + aux->distancia < D[aux->index]){
          D[aux->index] = D[less] + aux->distancia;
          ANT[aux->index] = less;
        }        
      }
      aux = aux->next;
    }
  }

  printDjikstra(origem, D, ANT, graphIndex);
}

int main(void){
  char arqInp[50];
  char field[15];
	char *cidadeOrigem = (char *)malloc(sizeof(char *));
	int option = -1;
	scanf("%d %s",&option, arqInp);

  //Build the index list that maps the cities
  indexList *graphIndex = (indexList *)malloc(sizeof(indexList));
	*graphIndex = NULL;
  buildIndex(arqInp, graphIndex);

  //Build the adjacency list with each city mapped in the index above
  adjList *G = (adjList *)malloc(sizeof(adjList *) * indexLength(graphIndex));
  *G = NULL;
  indexNode *auxIndex = *graphIndex;
  for(int i = 0; i < indexLength(graphIndex); ++i) {
    adjNode *new = createAdjNode(auxIndex->cidade,auxIndex->estado,0,"NULO",graphIndex);
    G[i] = new;
    auxIndex = auxIndex->next;
  }

	switch (option)	{
		case 9:
      buildAdjList(arqInp, G, graphIndex);			
	    for(int i = 0; i < indexLength(graphIndex); ++i) {
        adjNode *auxNode = G[i];
        printf("%s %s ", auxNode->cidade, auxNode->estado);
        auxNode = auxNode->next;
        while (auxNode != NULL){
          if(strcmp(auxNode->tempoViagem,"") == 0){
            printf("%s %s %d ", auxNode->cidade, auxNode->estado, auxNode->distancia);
            auxNode = auxNode->next;
          } else {
            printf("%s %s %d %s ", auxNode->cidade, auxNode->estado, auxNode->distancia, auxNode->tempoViagem);
            auxNode = auxNode->next;
          }
        }
        printf("\n");
      }		
      break;

    case 10:
			scanf("%s",field);
			scan_quote_string(cidadeOrigem);
      buildAdjList(arqInp, G, graphIndex);
      djikstra(searchByCity(cidadeOrigem, graphIndex), G, graphIndex);
      break;

		default:
			break;
	}
  return 0;
}