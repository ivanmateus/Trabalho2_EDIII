#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphIndex.h"

indexNode *createIndexNode(int index, char *city, char *state){
	indexNode *new = (indexNode *)malloc(sizeof(indexNode));	//Allocate memory for the node
	if(new == NULL){
		return 0;
	}
  new->i = index;
	new->cidade = (char *)malloc(strlen(city) * sizeof(char));
	strcpy(new->cidade,city);
	strcpy(new->estado,state);
  new->estado[2] = '\0';
	new->next = NULL;
	return new;
}

int insertIndex(char *city, char *state, indexList *list){
	indexNode *aux = *list;

	if(aux == NULL){
		indexNode *new = createIndexNode(0, city, state);
		*list = new;
		return 1;
	}
  if(strcmp(city, aux->cidade) < 0){
    indexNode *new = createIndexNode(aux->i, city, state);
		new->next = aux;
    *list = new;
    while(aux != NULL){
      aux->i = aux->i + 1;
      aux = aux->next;
    }
		return 1;
  }
  
  int i = 1;
	while(aux->next != NULL){
		if(strcmp(city, aux->cidade) == 0){
			return 0;
		}
    if(strcmp(city, aux->next->cidade) < 0){
      indexNode *new = createIndexNode(aux->next->i, city, state);
      indexNode *temp = aux->next;
			aux->next = new;
      new->next = temp;
      while(temp != NULL){
        temp->i = temp->i + 1;
        temp = temp->next;
      }
			return 1;
    }
		aux = aux->next;
    ++i;
	}
  if(aux->next == NULL && strcmp(city, aux->cidade) != 0){
    indexNode *new = createIndexNode(i, city, state);
		aux->next = new;
		return 1;
  }
	return 0;
}

int searchByCity(char *city, indexList *list) {
  indexNode *aux = *list;
  while(aux != NULL){
    if(strcmp(city, aux->cidade) == 0) {
      return aux->i;
    }
    aux = aux->next;
  }
  return -1;
}

indexNode *searchByIndex(int index, indexList *list) {
  indexNode *aux = *list;
  while(aux != NULL){
    if(index == aux->i) {
      return aux;
    }
    aux = aux->next;
  }
  return NULL;
}

int indexLength(indexList *list) {
  indexNode *aux = *list;
  int i;
  for(i = 0; aux != NULL; ++i){
    aux = aux->next;
  }
  return i;
}