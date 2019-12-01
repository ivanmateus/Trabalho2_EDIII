//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#include <stdio.h>
#include <stdlib.h>
#include "adjList.h"

//Create a new node to the adjacency list
adjNode *createAdjNode(char *cidade, char *estado, int distancia, char *tempoViagem, indexList *graphIndex){
	adjNode *new = (adjNode *)malloc(sizeof(adjNode));	//Allocate memory for the node
	if(new == NULL){
		return NULL;
	}
	//Search for the corresponding index
	int index = searchByCity(cidade, graphIndex);
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

//Initialize an adjacency list
void initAdjList(adjList *G, int length){
  for(int i = 0; i < length; ++i) {
    G[i] = (adjNode *)malloc(sizeof(adjNode));
		G[i] = NULL;
  }
}

//Insert a new node in the adjacency list with
//data given by a register (dataReg). The function receives a "back"
//flag to indicate the direction of the insertion
//(from the origin to the destination or the contrary)
int insertAdjList(dataReg *reg, adjList *head, indexList *graphIndex, int back){
	//Instance an aux node to help in the insertion
	adjNode *aux = *head;
	//If the flag is set, then the insertion is from the destination to the origin (dest->origin)
	//So the origin city will be inserted in the adjacency list of the dest. city.
	//The same applies to the state.
	//If the flag isn't set, then the insertion is from origin to destination (origin->dest)
	char *cidade = (char *)malloc(strlen(back ? reg->cidadeOrigem : reg->cidadeDestino) * sizeof(char));
	back ? strcpy(cidade,reg->cidadeOrigem) : strcpy(cidade,reg->cidadeDestino);
	char *estado = (char *)malloc(strlen(back ? reg->estadoOrigem : reg->estadoDestino) * sizeof(char));
	back ? strcpy(estado,reg->estadoOrigem) : strcpy(estado,reg->estadoDestino);

	//If the list is still empty
	if(aux == NULL){
		//The new node becomes the head of the list
		adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
    *head = new;
		return 1;
	}
	//If the city of the new node comes
	//before the first one, insert before
	//and make it the head of the list
  if(strcmp(cidade, aux->cidade) < 0){
    adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
		new->next = aux;
    *head = new;
		return 1;
  }

	//While there are nodes in the list
	while(aux->next != NULL){
		//If there is an entry already, don't insert
		if(strcmp(cidade, aux->cidade) == 0){
			return 0;
		}
		//If the new city is between the current (aux)
		//and the next (aux->next), the insert it
		//between them
    if(strcmp(cidade, aux->next->cidade) < 0){
      adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
      //Store the pointer to the next
			adjNode *temp = aux->next;
			//Point the current to the new
			aux->next = new;
			//Point the new to the old next (stored in temp)
      new->next = temp;
			return 1;
    }
		aux = aux->next;
	}
	//If the last element of the list has been reached
	//and the new city is different from this element's one, insert
  if(aux->next == NULL && strcmp(cidade, aux->cidade) != 0){
    adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
		aux->next = new;
		return 1;
  }
	return 0;
}

//Build an adjancency list based on the .bin file
int buildAdjList(char *fName, adjList *graph, indexList *index){
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
  	printf("Falha no carregamento do arquivo.\n");
   	return 0;
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

	//If the document isn't consistent, stop the execution
  if(hReg->status == '0') {
    printf("Falha no carregamento do arquivo.\n");
    return 0;
  }

	//While there are registers to be read
	while(fread(reg->estadoOrigem,ORIGINSIZE,1,fp)) {
		if(reg->estadoOrigem[0] == '\n'){
			printf("Falha na execução da funcionalidade.\n");
			return 0;				
		} else if(reg->estadoOrigem[0] != '*'){
			//Read the fixed size fields
			fread(reg->estadoDestino,DESTSIZE,1,fp);
			fread(&(reg->distancia),DISTANCESIZE,1,fp);
			
			//Read the rest of the register
			fread(buf,VARSIZE,1,fp);

			//Separate the fields using the "|" delimiter
			char *bufPtr = buf;
			char *field = strsep(&bufPtr,"|");
			reg->cidadeOrigem = field;
			
			field = strsep(&bufPtr,"|");
			reg->cidadeDestino = field;
			
			field = strsep(&bufPtr,"|");
			reg->tempoViagem = field;

      //Insert the node in the adjacency list
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
	return 1;
}

//Print the adjacency list
void printAdjList(adjList *G, indexList *graphIndex) {
	indexNode *auxInd = *graphIndex;
  for(int i = 0; i < indexLength(graphIndex); ++i) {
		//Print the city/state correspondent to the list
    printf("%s %s ", auxInd->cidade, auxInd->estado);
    adjNode *auxNode = G[i];
		//And print the list
    while (auxNode != NULL){
			//If the field "tempoViagem" is "", don't print it
      if(strcmp(auxNode->tempoViagem,"") == 0){
        printf("%s %s %d ", auxNode->cidade, auxNode->estado, auxNode->distancia);
        auxNode = auxNode->next;
      } else {
        printf("%s %s %d %s ", auxNode->cidade, auxNode->estado, auxNode->distancia, auxNode->tempoViagem);
        auxNode = auxNode->next;
      }
    }
		auxInd = auxInd->next;
    printf("\n");
  }
}