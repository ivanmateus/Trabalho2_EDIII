//Felipe Tiago de Carli - 10525686
//Gabriel de Andrade Dezan - 10525706
//Ivan Mateus de Lima Azevedo - 10525602

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graphIndex.h"
#include "../programaTrab2.h"

//Create a new node to the index list
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

//Insert a new node in the index list
int insertIndex(char *city, char *state, indexList *list){
	indexNode *aux = *list;

	//If the list is empty
	if(aux == NULL){
		//Make the new node its head
		indexNode *new = createIndexNode(0, city, state);
		*list = new;
		return 1;
	}
	//If the new city comes before the first one
  if(strcmp(city, aux->cidade) < 0){
    indexNode *new = createIndexNode(aux->i, city, state);
		//Insert it before and make it the new head
		new->next = aux;
    *list = new;
		//And increment the following indexes
    while(aux != NULL){
      aux->i = aux->i + 1;
      aux = aux->next;
    }
		return 1;
  }
  
	//Variable to store the current index
  int i = 1;
	//While the list doesn't end
	while(aux->next != NULL){
		//If there's already an entry, don't insert
		if(strcmp(city, aux->cidade) == 0){
			return 0;
		}
		//If the new city is between the current (aux)
		//and the next (aux->next), insert it between them
    if(strcmp(city, aux->next->cidade) < 0){
      indexNode *new = createIndexNode(aux->next->i, city, state);
			//Store the pointer to the next
      indexNode *temp = aux->next;
			//Point the current to the new
			aux->next = new;
			//Point the new to the old next (stored in temp)
      new->next = temp;
			//And increment the following indexes
      while(temp != NULL){
        temp->i = temp->i + 1;
        temp = temp->next;
      }
			return 1;
    }
		aux = aux->next;
    ++i;
	}
	//If the last element has been reached and
	//the city hasn't been inserted, insert it in the end
  if(aux->next == NULL && strcmp(city, aux->cidade) != 0){
    indexNode *new = createIndexNode(i, city, state);
		aux->next = new;
		return 1;
  }
	return 0;
}

//Build the index list based on the .bin file
void buildIndex(char *fName, indexList *graphIndex){
	FILE *fp = fopen(fName, "rb");
	if (!fp) {
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

	//If the file isn't consistent, stop the execution
  if(hReg->status == '0') {
    return;
  }

	//While there are registers to be read
	while(fread(reg->estadoOrigem,ORIGINSIZE,1,fp)) {
		if(reg->estadoOrigem[0] == '\n'){
			return;				
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
			
			//Insert both cities (origin and destination)
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

//Search the corresponding index of a given city
int searchByCity(char *city, indexList *list) {
  indexNode *aux = *list;
	//While the list doesn't end
  while(aux != NULL){
		//If the city was found, return the index
    if(strcmp(city, aux->cidade) == 0) {
      return aux->i;
    }
    aux = aux->next;
  }
	//If no city was found, return -1
  return -1;
}

//Search an index list node based on a given index
indexNode *searchByIndex(int index, indexList *list) {
  indexNode *aux = *list;
	//While the list doesn't end
  while(aux != NULL){
		//If the index was found, return the node
    if(index == aux->i) {
      return aux;
    }
    aux = aux->next;
  }
	//If no index was found, return NULL
  return NULL;
}

//Calculate the length of the index list
//which means the number of cities in the graph
//(i.e. the number of vertices)
int indexLength(indexList *list) {
  indexNode *aux = *list;
  //Variable to store the size of the list
	int i;
	//Increment the variable until the list ends
  for(i = 0; aux != NULL; ++i){
    aux = aux->next;
  }
	//Return the value
  return i;
}