#include "../include/listaEncadeada.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

ListaEncadeada *criarLista(Livro *item) {
  ListaEncadeada *tmp = malloc(sizeof(ListaEncadeada));

  if (tmp == NULL)
    return NULL;

  tmp->item = item;
  tmp->prox = NULL;
  tmp->liberar = 1;

  return tmp;
}

void liberarLista(ListaEncadeada *lista) {
  ListaEncadeada *aux;

  while (lista != NULL) {
    aux = lista->prox;

    if (lista->liberar)
      liberarLivro(lista->item);

    free(lista);

    lista = aux;
  }
}

ListaEncadeada *adicionarItemInicio(ListaEncadeada *lista, Livro *livro) {
  if (lista == NULL || !lista->liberar)
    return NULL;

  ListaEncadeada *tmp = criarLista(livro);

  if (tmp == NULL)
    return NULL;

  tmp->prox = lista;

  return tmp;
}

char adicionarItemFim(ListaEncadeada *lista, Livro *livro) {
  if (lista == NULL || !lista->liberar)
    return 0;

  while (lista->prox)
    lista = lista->prox;

  lista->prox = criarLista(livro);

  return lista->prox != NULL;
}

char adicionarItemIndice(ListaEncadeada *lista, unsigned int posix,
                         Livro *livro, char replace) {
  if (lista == NULL || posix == 0)
    return 0;

  posix--;

  while (posix) {
    if (lista->prox == NULL)
      return 0;

    lista = lista->prox;
    posix--;
  }

  ListaEncadeada *aux = lista->prox;

  lista->prox = criarLista(livro);

  if (lista->prox == NULL) {
    lista->prox = aux;
    return 0;
  }

  if (replace) {
    if (aux == NULL) {
      lista->prox->prox = NULL;
    } else {
      lista->prox->prox = aux->prox;
      aux->prox = NULL;
      liberarLista(aux);
    }
  } else {
    lista->prox->prox = aux;
  }

  return 1;
}

uint32_t ListaEncadeadaGetLength(ListaEncadeada *lista) {
  uint32_t l = 0;

  while (lista) {
    l++;
    lista = lista->prox;
  }

  return l;
}

Livro *obterLivroIndice(ListaEncadeada *lista, unsigned int idx) {
  if (lista == NULL)
    return NULL;

  while (idx > 0) {
    if (lista->prox == NULL)
      return lista->item;

    lista = lista->prox;
    idx--;
  }

  return lista->item;
}

ListaEncadeada *obterLivroRange(ListaEncadeada *lista, unsigned int idxIni,
                                unsigned int idxFinal) {
  ListaEncadeada *listaRetorno = NULL;
  ListaEncadeada *listaAux = NULL;

  if (lista == NULL || idxIni > idxFinal)
    return NULL;

  while (idxIni != 0) {
    if (lista->prox == NULL)
      return NULL;

    lista = lista->prox;
    idxIni--;
    idxFinal--;
  }
  if(idxFinal != UINT32_MAX)
    idxFinal++;

  while (idxFinal != 0) {
    if (lista == NULL) {
      liberarLista(listaRetorno);
      return NULL;
    }

    if (listaRetorno == NULL) {
      listaRetorno = criarLista(lista->item);

      if (listaRetorno == NULL)
        return NULL;

      listaRetorno->liberar = 0;
      listaAux = listaRetorno;
    } else {
      listaAux->prox = criarLista(lista->item);

      if (listaAux->prox == NULL) {
        liberarLista(listaRetorno);
        return NULL;
      }

      listaAux->prox->liberar = 0;
      listaAux = listaAux->prox;
    }

    lista = lista->prox;
    idxFinal--;
  }

  return listaRetorno;
}

Livro *compararCodigo(ListaEncadeada *lista, int codigo) {
  while (lista) {
    if (compararCodigoLivro(lista->item, codigo))
      return lista->item;

    lista = lista->prox;
  }

  return NULL;
}

static ListaEncadeada *adicionarReferencia(ListaEncadeada **inicio,
                                           ListaEncadeada **fim, Livro *livro) {
  ListaEncadeada *novo = criarLista(livro);

  if (novo == NULL)
    return NULL;

  novo->liberar = 0;

  if (*inicio == NULL) {
    *inicio = novo;
    *fim = novo;
  } else {
    (*fim)->prox = novo;
    *fim = novo;
  }

  return novo;
}

ListaEncadeada *compararAnoPublicacao(ListaEncadeada *lista,
                                      short anoPublicacao,
                                      enum LivroAnoComparacao comparacao) {
  ListaEncadeada *ret = NULL;
  ListaEncadeada *fim = NULL;

  while (lista) {
    if (compararAnoPublicacaoLivro(lista->item, anoPublicacao, comparacao)) {
      if (adicionarReferencia(&ret, &fim, lista->item) == NULL) {
        liberarLista(ret);
        return NULL;
      }
    }

    lista = lista->prox;
  }

  return ret;
}

ListaEncadeada *compararDisponivel(ListaEncadeada *lista,
                                   enum TipoDisponibilidade disponibilidade) {
  ListaEncadeada *ret = NULL;
  ListaEncadeada *fim = NULL;

  while (lista) {
    if (compararDisponivelLivro(lista->item) == disponibilidade) {
      if (adicionarReferencia(&ret, &fim, lista->item) == NULL) {
        liberarLista(ret);
        return NULL;
      }
    }

    lista = lista->prox;
  }

  return ret;
}

ListaEncadeada *compararTituloIgual(ListaEncadeada *lista, const char *texto) {
  ListaEncadeada *ret = NULL;
  ListaEncadeada *fim = NULL;

  while (lista) {
    if (compararTituloIgualLivro(lista->item, texto)) {
      if (adicionarReferencia(&ret, &fim, lista->item) == NULL) {
        liberarLista(ret);
        return NULL;
      }
    }

    lista = lista->prox;
  }

  return ret;
}

ListaEncadeada *compararTituloDentro(ListaEncadeada *lista, const char *texto) {
  ListaEncadeada *ret = NULL;
  ListaEncadeada *fim = NULL;

  while (lista) {
    if (compararTituloDentroLivro(lista->item, texto)) {
      if (adicionarReferencia(&ret, &fim, lista->item) == NULL) {
        liberarLista(ret);
        return NULL;
      }
    }

    lista = lista->prox;
  }

  return ret;
}

ListaEncadeada *compararAutorIgual(ListaEncadeada *lista, const char *texto) {
  ListaEncadeada *ret = NULL;
  ListaEncadeada *fim = NULL;

  while (lista) {
    if (compararAutorIgualLivro(lista->item, texto)) {
      if (adicionarReferencia(&ret, &fim, lista->item) == NULL) {
        liberarLista(ret);
        return NULL;
      }
    }

    lista = lista->prox;
  }

  return ret;
}

ListaEncadeada *compararAutorDentro(ListaEncadeada *lista, const char *texto) {
  ListaEncadeada *ret = NULL;
  ListaEncadeada *fim = NULL;

  while (lista) {
    if (compararAutorDentroLivro(lista->item, texto)) {
      if (adicionarReferencia(&ret, &fim, lista->item) == NULL) {
        liberarLista(ret);
        return NULL;
      }
    }

    lista = lista->prox;
  }

  return ret;
}