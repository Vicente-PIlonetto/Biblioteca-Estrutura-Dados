#ifndef HISTORICO_H
#define HISTORICO_H

#include "livro.h"


enum StatusHistorico {
  NORMAL,
  ENTREGUE,
  ATRASADO
};

typedef struct Historico {
  Livro* livro;

  char* dataEmprestimo;
  char* dataDevolucao;

  unsigned int idUsuario;
  enum StatusHistorico status;
} Historico;

Historico* criarHistorico(
  unsigned int idUsuario,
  Livro* livro,
  const char* dataEmprestimo,
  const char* dataDevolucao,
  enum StatusHistorico status
);

Historico* copiaHistorico(Historico* historico);

void liberarHistorico(Historico* historico);

#endif