#include <stdio.h>
#include <stdlib.h>
#include "adjList.h"

adjNode *createAdjNode(char *cidade, char *estado, int distancia, char *tempoViagem, indexList *graphIndex){
	adjNode *new = (adjNode *)malloc(sizeof(adjNode));	//Allocate memory for the node
	if(new == NULL){
		return NULL;
	}
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

int insertAdjList(dataReg *reg, adjList *head, indexList *graphIndex, int back){
	adjNode *aux = (*head)->next;
	char *cidade = (char *)malloc(strlen(back ? reg->cidadeOrigem : reg->cidadeDestino) * sizeof(char));
	back ? strcpy(cidade,reg->cidadeOrigem) : strcpy(cidade,reg->cidadeDestino);
	char *estado = (char *)malloc(strlen(back ? reg->estadoOrigem : reg->estadoDestino) * sizeof(char));
	back ? strcpy(estado,reg->estadoOrigem) : strcpy(estado,reg->estadoDestino);

	if(aux == NULL){
		adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
    (*head)->next = new;
		return 1;
	}
  if(strcmp(cidade, aux->cidade) < 0){
    adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
		new->next = aux;
    (*head)->next = new;
		return 1;
  }
	while(aux->next != NULL){
		if(strcmp(cidade, aux->cidade) == 0){
			return 0;
		}
    if(strcmp(cidade, aux->next->cidade) < 0){
      adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
      adjNode *temp = aux->next;
			aux->next = new;
      new->next = temp;
			return 1;
    }
		aux = aux->next;
	}
  if(aux->next == NULL && strcmp(cidade, aux->cidade) != 0){
    adjNode *new = createAdjNode(cidade, estado, reg->distancia, reg->tempoViagem, graphIndex);
		aux->next = new;
		return 1;
  }
	return 0;
}