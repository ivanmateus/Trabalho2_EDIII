#ifndef ADJLIST_H_
#define ADJLIST_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../programaTrab2.h"
#include "../src/graphIndex.h"

//-----Data structure for a node of the adjacency list
typedef struct adjNode {
	int index;
  char *cidade;
  char estado[3];
  int distancia;
  char *tempoViagem;
	struct adjNode* next;
} adjNode;

//-----Data structure for a list of nodes
typedef struct adjNode* adjList;

adjNode *createAdjNode(char *cidadeDestino, char *estadoDestino, int distancia, char *tempoViagem, indexList *list);
int insertAdjList(dataReg *reg, adjList *list, indexList *index, int back);

#endif