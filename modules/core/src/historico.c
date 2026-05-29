#include "../include/historico.h"

#include <stdlib.h>
#include <string.h>


Historico* criarHistorico(
  unsigned int idUsuario,
  Livro* livro,
  char* dataEmprestimo,
  char* dataDevolucao,
  enum StatusHistorico status
){
  Historico* tmp = malloc(sizeof(Historico));

  if(tmp == NULL)
    return NULL;

  tmp->idUsuario = idUsuario;

  tmp->livro = copiaLivro(livro);

  tmp->dataEmprestimo = malloc(strlen(dataEmprestimo) + 1);

  if(tmp->dataEmprestimo == NULL){
    liberarLivro(tmp->livro);
    free(tmp);

    return NULL;
  }

  strcpy(tmp->dataEmprestimo, dataEmprestimo);

  tmp->dataDevolucao = malloc(strlen(dataDevolucao) + 1);

  if(tmp->dataDevolucao == NULL){
    free(tmp->dataEmprestimo);
    liberarLivro(tmp->livro);
    free(tmp);

    return NULL;
  }

  strcpy(tmp->dataDevolucao, dataDevolucao);

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
