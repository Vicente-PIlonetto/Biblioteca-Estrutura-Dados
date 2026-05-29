#include "../include/listaEncadeada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ListaEncadeada *criarLista(Livro *item) {
  ListaEncadeada *tmp = (ListaEncadeada *)malloc(sizeof(ListaEncadeada));
  tmp->item = item;
  tmp->prox = NULL;
  tmp->liberar = 1;
  return tmp;
}

void liberarLista(ListaEncadeada *lista) {

  ListaEncadeada *aux;

  while (lista != NULL) {
    aux = lista->prox;
    if(!lista->liberar)
      liberarLivro(lista->item);
    free(lista);
    lista = aux;
  }
}

ListaEncadeada* adicionarItemInicio(ListaEncadeada *lista, Livro* livro){
  if(!lista->liberar)
    return NULL;

  ListaEncadeada* tmp = (ListaEncadeada*)malloc(sizeof(ListaEncadeada));

  tmp->item = livro;
  tmp->prox = lista;

  return tmp;
}

char adicionarItemFim(ListaEncadeada *lista, Livro* livro){
  if(!lista->liberar)
    return 0;

  while(lista->prox){
    lista = lista->prox;
  }
  lista->prox = criarLista(livro);

  return 1;
}

Livro *obterLivroIndice(ListaEncadeada *lista, unsigned int idx){
  if(lista == NULL)
    return NULL;

  while(idx > 0){
    if(lista->prox == NULL)
      return lista->item;

    lista = lista->prox;
    idx--;
  }
  return lista->item;
}

ListaEncadeada *obterLivroRange(ListaEncadeada *lista, unsigned int idxIni, unsigned int idxFinal){
  ListaEncadeada *listaRetorno = NULL, *listaAux;

  if(idxIni > idxFinal)
    return NULL;

  while(idxIni != 0){
    if(lista->prox == NULL)
      return NULL;
    lista = lista->prox;
    idxIni--;
    idxFinal--;
  }

  while(idxFinal != 0){
    if(listaRetorno == NULL){
      listaRetorno = criarLista(lista->item);
      listaRetorno->liberar = 0;
      listaAux = listaRetorno;
    } else {
      listaAux->prox = criarLista(lista->item);
      listaAux = listaAux->prox;
    }

    lista = lista->prox;
    idxFinal--;
  }

  return listaRetorno;
}

ListaEncadeada* compararCodigo(ListaEncadeada *lista, int codigo){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while (lista) {
    if(compararCodigoLivro(lista->item, codigo)){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      } else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }
    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada* compararAnoPublicacao(
  ListaEncadeada *lista,
  short anoPublicacao,
  enum LivroAnoComparacao comparacao
){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while(lista != NULL){
    if(compararAnoPublicacaoLivro(lista->item, anoPublicacao, comparacao)){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      }else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }

    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada* compararDisponivel(ListaEncadeada *lista, enum TipoDisponibilidade disponibilidade){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while(lista != NULL){
    if(compararDisponivelLivro(lista->item) == disponibilidade){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      }else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }

    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada* compararTituloIgual(ListaEncadeada *lista, char* texto){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while(lista != NULL){
    if(compararTituloIgualLivro(lista->item, texto)){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      }else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }

    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada* compararTituloDentro(ListaEncadeada *lista, char* texto){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while(lista != NULL){
    if(compararTituloDentroLivro(lista->item, texto)){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      }else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }

    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada* compararAutorIgual(ListaEncadeada *lista, char* texto){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while(lista != NULL){
    if(compararAutorIgualLivro(lista->item, texto)){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      }else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }

    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada* compararAutorDentro(ListaEncadeada *lista, char* texto){
  ListaEncadeada* listaRetorno=NULL, *aux;

  while(lista != NULL){
    if(compararAutorDentroLivro(lista->item, texto)){
      if(listaRetorno == NULL){
        listaRetorno = criarLista(lista->item);
        listaRetorno->liberar = 0;
        aux = listaRetorno;
      }else {
        aux->prox = criarLista(lista->item);
        aux = aux->prox;
      }
    }

    lista = lista->prox;
  }

  return listaRetorno;
}

ListaEncadeada *carregarListaArquivo(char* pwd){
  ListaEncadeada *lista = NULL, *listaAux;
  FILE *arquivo = fopen(pwd, "rb");
  if(arquivo == NULL)
    return NULL;
  unsigned long l;
  char* aux1 = malloc(9), *aux2;
  unsigned int codigo;
  int c;
  short anoPublicacao,quantidadeDisponivel;


  fread(aux1, 1, 8, arquivo);
  aux1[8] = '\0';
  if(strcmp(aux1, CABECALHO_LISTA_ENCADEADA) != 0){
    free(aux1);
    fclose(arquivo);
    return NULL;
  }
  free(aux1);

  while((c = fgetc(arquivo)) == ';') {
    fread(&l, sizeof(unsigned long), 1, arquivo);
    aux1 = malloc(l+1);
    fread(aux1, 1, l, arquivo);
    aux1[l] = '\0';
    fread(&l, sizeof(unsigned long), 1, arquivo);
    aux2 = malloc(l+1);
    fread(aux2, 1, l, arquivo);
    aux2[l] = '\0';
    fread(&codigo, 4, 1, arquivo);
    fread(&anoPublicacao, 2, 1, arquivo);
    fread(&quantidadeDisponivel, 2, 1, arquivo);
    if(lista == NULL){
      lista = criarLista(
        criarLivro(aux1, aux2, anoPublicacao, quantidadeDisponivel, codigo)
      );
      listaAux = lista;
    }else{
      listaAux->prox = criarLista(
        criarLivro(aux1, aux2, anoPublicacao, quantidadeDisponivel, codigo)
      );
      listaAux = listaAux->prox;
    }
  }

  return lista;
}

char salvarListaArquivo(ListaEncadeada *lista, char* pwd){
  FILE* arquivo = fopen(pwd, "wb");
  if(arquivo == NULL){
    return 0;
  }
  char* aux;
  unsigned long lArquivo;

  fwrite(CABECALHO_LISTA_ENCADEADA, 1, 8, arquivo);
  while(lista != NULL){
    aux = obterBinarioLivro(lista->item, &lArquivo);
    fwrite(";", 1, 1, arquivo);
    fwrite(aux, 1, lArquivo, arquivo);
    free(aux);
    lista = lista->prox;
  }

  fclose(arquivo);
  return 1;
}