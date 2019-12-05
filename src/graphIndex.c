//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphIndex.h"
#include "../programaTrab2.h"

//Cria um novo nó para a lista de índices
indexNode *createIndexNode(int index, char *city, char *state){
	indexNode *new = (indexNode *)malloc(sizeof(indexNode));	//Aloca memória para o nó
	//Se não conseguiu alocar, retorna NULL
	if(new == NULL){
		return 0;
	}
	//Insere os dados no novo nó
  new->i = index;
	new->cidade = (char *)malloc(strlen(city) * sizeof(char));
	strcpy(new->cidade,city);
	strcpy(new->estado,state);
  new->estado[2] = '\0';
	new->next = NULL;
	return new;
}

//Insere um novo nó na lista de índices
int insertIndex(char *city, char *state, indexList *list){
	indexNode *aux = *list;

	//Se a lista estiver vazia
	if(aux == NULL){
		//Torna o novo nó o cabeça da lista, com índice 0
		indexNode *new = createIndexNode(0, city, state);
		*list = new;
		return 1;
	}
	//Se a nova cidade vier antes da cidade no cabeça
	//da lista (em ordem alfabética)
  if(strcmp(city, aux->cidade) < 0){
		//Cria o nó com o índice 0
    indexNode *new = createIndexNode(0, city, state);
		//Insere a nova cidade antes e a torna o cabeça da lista
		new->next = aux;
    *list = new;
		//E incrementa todos os índices seguintes
    while(aux != NULL){
      aux->i = aux->i + 1;
      aux = aux->next;
    }
		return 1;
  }
  
	//Enquanto a lista não chega no fim
	while(aux->next != NULL){
		//Se já houver uma cidade na lista, não insere novamente
		if(strcmp(city, aux->cidade) == 0){
			return 0;
		}
		//Se a posição da cidade for entre o nó atual (aux)
		//e o pŕoximo (aux->next), então insere o novo entre eles
    if(strcmp(city, aux->next->cidade) < 0){
      indexNode *new = createIndexNode(aux->next->i, city, state);
      //Guarde o ponteiro para o próximo
      indexNode *temp = aux->next;
			//Aponte o ponteiro do atual para o novo
			aux->next = new;
			//Aponte o ponteiro do novo para o antigo pŕoximo (guardado na variável temp)
      new->next = temp;
			//E incremente os índices seguintes
      while(temp != NULL){
        temp->i = temp->i + 1;
        temp = temp->next;
      }
			return 1;
    }
		aux = aux->next;
	}
	//Se a lista chegou ao fim e a nova cidade for diferente
	//da última, então insere o novo nó no fim
  if(aux->next == NULL && strcmp(city, aux->cidade) != 0){
    indexNode *new = createIndexNode(aux->i, city, state);
		aux->next = new;
		return 1;
  }
	return 0;
}

//Constroi a lista de índices baseado no arquivo de entrada .bin
void buildIndex(char *fName, indexList *graphIndex){
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
   	return;
  }

	//Leia o registro de cabeçalho
	headerReg *hReg = (headerReg *)malloc(HREGSIZE);
	fread(&(hReg->status),STATUSSIZE,1,fp);
	fread(&(hReg->numeroVertices),VERTSIZE,1,fp);
	fread(&(hReg->numeroArestas),EDGESIZE,1,fp);
	fread(hReg->dataUltimaCompactacao,sizeof(char),LASTCOMPSIZE,fp);

	//Cria um registro de dados auxiliar para guardar
	//temporariamente os dados de cada registro
	dataReg *reg = (dataReg *)malloc(DREGSIZE);
	
	//Cria um buffer para ler a parte variável
	//do registro de dados (i.e. os campos "cidadeOrigem",
	//"cidadeDestino" e "tempoViagem")
	char buf[VARSIZE];

	//Variável para guardar o RRN do registro atual
	int rrn = 0;

	//Se o arquivo estiver inconsistente, para a execução
  if(hReg->status == '0') {
    return;
  }

	//Enquanto houver registros a serem lidos
	while(fread(reg->estadoOrigem,ORIGINSIZE,1,fp)) {
		if(reg->estadoOrigem[0] != '*'){
			//Leia os outros campos de tamanho fixo
			fread(reg->estadoDestino,DESTSIZE,1,fp);
			fread(&(reg->distancia),DISTANCESIZE,1,fp);
			
			//Leia a parte variável do registro
			fread(buf,VARSIZE,1,fp);

			//Separe os campos pelo delimitador "|"
			char *bufPtr = buf;
			char *field = strsep(&bufPtr,"|");
			reg->cidadeOrigem = field;
			
			field = strsep(&bufPtr,"|");
			reg->cidadeDestino = field;
			
			//Insere ambas as cidades (origem e destino)
      insertIndex(reg->cidadeOrigem, reg->estadoOrigem, graphIndex);
      insertIndex(reg->cidadeDestino, reg->estadoDestino, graphIndex);
			
			//Vá para o próximo RRN
			++rrn;
		} else {
			++rrn;
			fseek(fp,DREGSIZE-ORIGINSIZE,SEEK_CUR);
		}
	}

	fclose(fp);
}

//Procura o índice correspondente a uma cidade
int searchByCity(char *city, indexList *list) {
  indexNode *aux = *list;
	//Enquanto a lista não termina
  while(aux != NULL){
		//Se achou a cidade, retorna seu índice
    if(strcmp(city, aux->cidade) == 0) {
      return aux->i;
    }
    aux = aux->next;
  }
	//Se não achou cidade alguma, retorna -1
  return -1;
}

//Procura um nó da lista baseado no índice
indexNode *searchByIndex(int index, indexList *list) {
  indexNode *aux = *list;
	//Enquanto a lista não terminar
  while(aux != NULL){
		//Se achou o índice, retorna o nó
    if(index == aux->i) {
      return aux;
    }
    aux = aux->next;
  }
	//Se não achou, retorna NULL
  return NULL;
}

//Calcula o tamanho da lista de índices,
//ou seja, o número de vértices do grafo, que é o 
//mesmo que o número de cidades diferentes
int indexLength(indexList *list) {
  indexNode *aux = *list;
  //Variável que guarda o tamanho da lista
	int i;
	//Incrementa a variável até a lista terminar
  for(i = 0; aux != NULL; ++i){
    aux = aux->next;
  }
	//Retorna o valor calculado
  return i;
}