//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#ifndef ADJLIST_H_
#define ADJLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../programaTrab2.h"
#include "../src/graphIndex.h"

//-----Estrutura de dados para um nó da lista de adjacências
typedef struct adjNode {
	int index;
  char *cidade;
  char estado[3];
  int distancia;
  char *tempoViagem;
	struct adjNode* next;
} adjNode;

//-----Estrutura para uma lista de adjacências
typedef struct adjNode* adjList;

adjNode *createAdjNode(char *cidadeDestino, char *estadoDestino, int distancia, char *tempoViagem, indexList *list);
void initAdjList(adjList *G, int length);
int insertAdjList(dataReg *reg, adjList *list, indexList *index, int back);
int buildAdjList(char *fName, adjList *graph, indexList *index);
void printAdjList(adjList *G, indexList *graphIndex);

#endif