#ifndef PILHA_HISTORICO_H
#define PILHA_HISTORICO_H
#include "historico.h"

typedef struct PilhaHistorico {
  Historico* item;
  struct PilhaHistorico* prox;
} PilhaHistorico;


PilhaHistorico* criarPilhaHistorico(Historico* item);

PilhaHistorico* adicionarHistorico(
  PilhaHistorico* pilha,
  Historico* item
);

void liberarPilhaHistorico(PilhaHistorico* pilha);

#endif