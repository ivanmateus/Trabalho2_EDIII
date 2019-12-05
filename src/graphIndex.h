//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#ifndef GRAPHINDEX_H_
#define GRAPHINDEX_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//-----Estrutura de dados para um nó da lista de índices
typedef struct indexNode {
	int i;
  char *cidade;
  char estado[3];
	struct indexNode* next;
} indexNode;

//-----Estrutura de dados para uma lista de índices
typedef struct indexNode* indexList;

indexNode *createIndexNode(int index, char *city, char state[2]);
int insertIndex(char *city, char state[2], indexList *list);
void buildIndex(char *fName, indexList *graphIndex);
int searchByCity(char *city, indexList *list);
int indexLength(indexList *list);
indexNode *searchByIndex(int index, indexList *list);

#endif