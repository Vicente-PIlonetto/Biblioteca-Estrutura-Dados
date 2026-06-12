#include "../include/pilhaHistorico.h"

#include <stdlib.h>

PilhaHistorico* criarPilhaHistorico(Historico* item){
  PilhaHistorico* tmp = malloc(sizeof(PilhaHistorico));

  if(tmp == NULL)
    return NULL;

  tmp->item = item;
  tmp->prox = NULL;

  return tmp;
}


PilhaHistorico* adicionarHistorico(
  PilhaHistorico* pilha,
  Historico* item
){
  PilhaHistorico* aux = criarPilhaHistorico(item);

  if(aux == NULL){
    return pilha;
  }

  aux->prox = pilha;

  return aux;
}


void liberarPilhaHistorico(PilhaHistorico* pilha){
  PilhaHistorico* aux;

  while(pilha != NULL){
    aux = pilha->prox;

    liberarHistorico(pilha->item);

    free(pilha);

    pilha = aux;
  }
}