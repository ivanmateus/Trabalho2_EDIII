//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#ifndef PROGRAMATRAB2_H_
#define PROGRAMATRAB2_H_

#include <stdio.h>
#include <stdlib.h>
#include "./src/graphIndex.h"

//-----Constants for header register
#define STATUSSIZE 1 * sizeof(char)
#define VERTSIZE 1 * sizeof(int)
#define EDGESIZE 1 * sizeof(int)
#define LASTCOMPSIZE 10 * sizeof(char)
#define HREGSIZE 19

//-----Constants for data register
#define ORIGINSIZE 2 * sizeof(char)
#define DESTSIZE 2 * sizeof(char)
#define DISTANCESIZE 1 * sizeof(int)
#define DREGSIZE 85
#define VARSIZE (DREGSIZE - ORIGINSIZE - DESTSIZE - DISTANCESIZE)

//-----Data structure for header register
typedef struct {
  char status;
  int numeroVertices;
  int numeroArestas;
  char dataUltimaCompactacao[10];
} headerReg;

//-----Data structure for data register
typedef struct {
  char estadoOrigem[2];
  char estadoDestino[2];
  int distancia;
  char *cidadeOrigem;
  char *cidadeDestino;
  char *tempoViagem;
} dataReg;

#endif