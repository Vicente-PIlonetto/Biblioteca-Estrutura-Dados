#ifndef LISTA__ENCADEADA_H
#define LISTA__ENCADEADA_H

#include "livro.h"

#define CABECALHO_LISTA_ENCADEADA "LELIVROS"


enum TipoDisponibilidade{
  LIVRO_INDISPONIVEL,
  LIVRO_DISPONIVEL,
};

typedef struct ListaEncadeada{
  Livro *item;
  struct ListaEncadeada *prox;
  char liberar;
} ListaEncadeada;


ListaEncadeada *criarLista(Livro *item);

ListaEncadeada* adicionarItemInicio(ListaEncadeada *lista, Livro* livro);

char adicionarItemFim(ListaEncadeada *lista, Livro* livro);

void liberarLista(ListaEncadeada *lista);

Livro *obterLivroIndice(ListaEncadeada *lista, unsigned int idx);

ListaEncadeada *obterLivroRange(ListaEncadeada *lista, unsigned int idxIni, unsigned int idxFinal);

ListaEncadeada* compararCodigo(ListaEncadeada *lista, int codigo);

ListaEncadeada* compararAnoPublicacao(ListaEncadeada *lista, short anoPublicacao, enum LivroAnoComparacao comparacao);

ListaEncadeada*  compararDisponivel(ListaEncadeada *lista, enum TipoDisponibilidade disponibilidade);

ListaEncadeada* compararTituloIgual(ListaEncadeada *lista, char* texto);

ListaEncadeada* compararTituloDentro(ListaEncadeada *lista, char* texto);

ListaEncadeada* compararAutorIgual(ListaEncadeada *lista, char* texto);

ListaEncadeada* compararAutorDentro(ListaEncadeada *lista, char* texto);

ListaEncadeada *carregarListaArquivo(char* pwd);

char salvarListaArquivo(ListaEncadeada *lista, char* pwd);

#endif