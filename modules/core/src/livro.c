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

  l = strlen(titulo);
  tmp->titulo = (char *)malloc(sizeof(char) * l + 1);
  if (tmp->titulo == NULL) {
    free(tmp);

    return NULL;
  }
  tmp->titulo[l] = '\0';
  strcpy(tmp->titulo, titulo);

  l = strlen(autor);
  tmp->autor = (char *)malloc(sizeof(char) * l + 1);
  if (tmp->autor == NULL) {
    free(tmp->titulo);
    free(tmp);
    return NULL;
  }

  strcpy(tmp->autor, autor);
  tmp->autor[l] = '\0';

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

char compararTituloIgualLivro(Livro* livro, char* texto){
  return strcmp(livro->titulo, texto) == 0;
}

char compararTituloDentroLivro(Livro* livro, char* texto){
  return strstr(livro->titulo, texto) != NULL;
}

char compararAutorIgualLivro(Livro* livro, char* texto){
  return strcmp(livro->autor, texto) == 0;
}

char compararAutorDentroLivro(Livro* livro, char* texto){
  return strstr(livro->autor, texto) != NULL;
}

char *obterBinarioLivro(Livro* livro, unsigned long* lenArquivo){
  unsigned long lTitulo = strlen(livro->titulo);
  unsigned long lAutor = strlen(livro->autor);

  unsigned long lTotal = sizeof(unsigned long) * 2 + sizeof(int) + sizeof(short) * 2 + lTitulo + lAutor;
  char* rawData = malloc(lTotal);
  *lenArquivo = lTotal;

  memcpy(rawData, &lTitulo, sizeof(unsigned long));
  memcpy(
    rawData + sizeof(unsigned long),
    livro->titulo,
    lTitulo
  );
  memcpy(
    rawData + sizeof(unsigned long) + lTitulo,
    &lAutor,
    sizeof(unsigned long)
  );
  memcpy(
    rawData + sizeof(unsigned long)*2 + lTitulo,
    livro->autor,
    lAutor
  );
  memcpy(
    rawData + sizeof(unsigned long)*2 + lTitulo + lAutor,
    &livro->codigo,
    sizeof(unsigned int)
  );
  memcpy(
    rawData + sizeof(unsigned long)*2 + lTitulo + lAutor + sizeof(unsigned int),
    &livro->anoPublicacao,
    sizeof(short)
  );
  memcpy(
    rawData + sizeof(unsigned long)*2 + lTitulo + lAutor + sizeof(unsigned int) + sizeof(short),
    &livro->quantidadeDisponivel,
    sizeof(short)
  );
  return rawData;
}