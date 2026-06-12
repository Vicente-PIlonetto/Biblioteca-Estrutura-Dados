from enum import IntEnum
from .livro import Livro

class Comparacao(IntEnum):
    IGUAL = 0
    MENOR = 1
    MENOR_IGUAL = 2
    MAIOR = 3
    MAIOR_IGUAL = 4



class ListaEncadeada:
    "Lista encadeada contendo todos os livros salvos"

    length: int
    "Quantidade de livros nessa lista"

    def __init__(self, livro: Livro | None = None) -> None: ...

    def append_at(self, livro: Livro, idx: int, replace: bool = True) -> None:
        "Função para adicionar um novo ítem, caso índice maior que length, será adicionado o item ao final"
        ...

    def obter_livro_indice(self, idx: int) -> Livro:
        "Função que retorna um Livro no índice específicado"
        ...

    def obter_livro_range(self, init: int, end: int = 4294967295) -> ListaEncadeada:
        "Função que retorna uma nova lista encadeada com os ítens no intervalo específicado"
        ...

    def obter_livro_codigo(self, codigo: int) -> Livro | None:
        "Função que retorna um livro com base no código informado, caso não encontre, nada será retornado"
        ...

    def obter_livro_ano_publicacao(
        self,
        ano_publicacao: int,
        comparacao: Comparacao
    ) -> ListaEncadeada:
        """
        Retorna uma lista encadeada contendo os livros que satisfazem
        a comparação com o ano de publicação informado.

        Args:
            ano_publicacao: Ano utilizado na comparação.
            comparacao:
                Tipo de comparação a ser realizada:

                - 0: igual a (==)
                - 1: menor que (<)
                - 2: menor ou igual a (<=)
                - 3: maior que (>)
                - 4: maior ou igual a (>=)

        Returns:
            ListaEncadeada contendo os livros encontrados.
        """
        ...

    def obter_livro_disponibilidade(self, disponibilidade: bool) -> ListaEncadeada:
        "Função que retorna uma lista encadeada contendo os livros disponíveis com base na variável disponibilidade"
        ...

    def obter_livro_titulo(self, titulo: str) -> ListaEncadeada:
        "Função que retorna uma lista encadeada de livros que possuem mesmo título que o especificado"
        ...

    def obter_livro_titulo_parcial(self, titulo: str) -> ListaEncadeada:
        "Função que retorna uma lista encadeada de livros que possuem parte do título igual ao especificado"
        ...

    def obter_livro_autor(self, autor: str) -> ListaEncadeada:
        "Função que retorna uma lista encadeada de livros que possuem mesmo autor que o especificado"
        ...

    def obter_livro_autor_parcial(self, autor: str) -> ListaEncadeada:
        "Função que retorna uma lista encadeada de livros que possuem parte do autor igual ao especificado"
        ...