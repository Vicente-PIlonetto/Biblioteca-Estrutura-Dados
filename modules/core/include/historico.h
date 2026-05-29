#ifndef HISTORICO_H
#define HISTORICO_H

#include "livro.h"


enum StatusHistorico {
  NORMAL,
  ENTREGUE,
  ATRASADO
};

typedef struct Historico {
  unsigned int idUsuario;

  Livro* livro;

  char* dataEmprestimo;
  char* dataDevolucao;

  enum StatusHistorico status;
} Historico;

Historico* criarHistorico(
  unsigned int idUsuario,
  Livro* livro,
  char* dataEmprestimo,
  char* dataDevolucao,
  enum StatusHistorico status
);

Historico* copiaHistorico(Historico* historico);

void liberarHistorico(Historico* historico);

#endif