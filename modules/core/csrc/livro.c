#include "../include/livro.h"
#include <stdlib.h>
#include <string.h>

Livro *criarLivro(
  char *titulo,
  char *autor,
  short anoPublicacao,
  short quantidadeDisponivel,
  unsigned int codigo
) {
  unsigned long l;
  Livro *tmp = (Livro *)malloc(sizeof(Livro));
  if (tmp == NULL) return NULL;

  tmp->titulo = strdup(titulo);

  if (tmp->titulo == NULL) {
    free(tmp);
    return NULL;
  }

  tmp->autor = strdup(autor);
  if (tmp->autor == NULL) {
    free(tmp->titulo);
    free(tmp);
    return NULL;
  }

  tmp->anoPublicacao = anoPublicacao;
  tmp->quantidadeDisponivel = quantidadeDisponivel;
  tmp->codigo = codigo;

  return tmp;
}

Livro* copiaLivro(Livro* livro){
  return criarLivro(
    livro->titulo,
    livro->autor,
    livro->anoPublicacao,
    livro->quantidadeDisponivel,
    livro->codigo
  );
}

void liberarLivro(Livro *livro) {
  free(livro->autor);
  free(livro->titulo);
  free(livro);
}

char compararCodigoLivro(Livro* livro, int codigo){
  return livro->codigo == codigo;
}

char compararAnoPublicacaoLivro(Livro* livro, short anoPublicacao, enum LivroAnoComparacao comparacao){
  switch (comparacao) {
    case ANO_IGUAL:
      return livro->anoPublicacao == anoPublicacao;
    case ANO_MAIOR:
      return livro->anoPublicacao > anoPublicacao;
    case ANO_MENOR:
      return livro->anoPublicacao < anoPublicacao;
    case ANO_MAIOR_IGUAL:
      return livro->anoPublicacao >= anoPublicacao;
    case ANO_MENOR_IGUAL:
      return livro->anoPublicacao <= anoPublicacao;
    default:
      return 0;
  }
}

char compararDisponivelLivro(Livro* livro){
  return livro->quantidadeDisponivel;
}

char compararTituloIgualLivro(Livro* livro, const char* texto){
  return strcmp(livro->titulo, texto) == 0;
}

char compararTituloDentroLivro(Livro* livro, const char* texto){
  return strstr(livro->titulo, texto) != NULL;
}

char compararAutorIgualLivro(Livro* livro, const char* texto){
  return strcmp(livro->autor, texto) == 0;
}

char compararAutorDentroLivro(Livro* livro, const char* texto){
  return strstr(livro->autor, texto) != NULL;
}