#include "../include/historico.h"

#include <stdlib.h>
#include <string.h>


Historico* criarHistorico(
  unsigned int idUsuario,
  Livro* livro,
  const char* dataEmprestimo,
  const char* dataDevolucao,
  enum StatusHistorico status
){
  Historico* tmp = malloc(sizeof(Historico));

  if(tmp == NULL)
    return NULL;

  tmp->idUsuario = idUsuario;

  tmp->livro = copiaLivro(livro);
  if(tmp->livro == NULL){
    free(tmp);
    return  NULL;
  }

  tmp->dataEmprestimo = strdup(dataEmprestimo);

  if(tmp->dataEmprestimo == NULL){
    liberarLivro(tmp->livro);
    free(tmp);

    return NULL;
  }

  tmp->dataDevolucao = strdup(dataEmprestimo);

  if(tmp->dataDevolucao == NULL){
    free(tmp->dataEmprestimo);
    liberarLivro(tmp->livro);
    free(tmp);

    return NULL;
  }

  tmp->status = status;

  return tmp;
}


Historico* copiaHistorico(Historico* historico){
  return criarHistorico(
    historico->idUsuario,
    historico->livro,
    historico->dataEmprestimo,
    historico->dataDevolucao,
    historico->status
  );
}


void liberarHistorico(Historico* historico){
  free(historico->dataEmprestimo);

  free(historico->dataDevolucao);

  liberarLivro(historico->livro);

  free(historico);
}
