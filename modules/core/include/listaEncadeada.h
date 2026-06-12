#ifndef LISTA__ENCADEADA_H
#define LISTA__ENCADEADA_H

#include "livro.h"
#include <stdint.h>

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

char adicionarItemIndice(ListaEncadeada *lista, unsigned int posix, Livro* livro, char replace);

void liberarLista(ListaEncadeada *lista);

Livro *obterLivroIndice(ListaEncadeada *lista, unsigned int idx);

uint32_t ListaEncadeadaGetLength(ListaEncadeada *lista);

ListaEncadeada *obterLivroRange(ListaEncadeada *lista, unsigned int idxIni, unsigned int idxFinal);

Livro* compararCodigo(ListaEncadeada *lista, int codigo);

ListaEncadeada* compararAnoPublicacao(ListaEncadeada *lista, short anoPublicacao, enum LivroAnoComparacao comparacao);

ListaEncadeada*  compararDisponivel(ListaEncadeada *lista, enum TipoDisponibilidade disponibilidade);

ListaEncadeada* compararTituloIgual(ListaEncadeada *lista, const char* texto);

ListaEncadeada* compararTituloDentro(ListaEncadeada *lista, const char* texto);

ListaEncadeada* compararAutorIgual(ListaEncadeada *lista, const char* texto);

ListaEncadeada* compararAutorDentro(ListaEncadeada *lista, const char* texto);

#endif