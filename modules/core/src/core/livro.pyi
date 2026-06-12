class Livro:
    """
    Representa um livro cadastrado na biblioteca.
    """

    titulo: str
    "Titulo do livro"
    autor: str
    "Autor do livro"
    codigo: int
    "Código de identificação do livro"
    ano_publicacao: int
    "Ano de publicação do livro"
    quantidade_disponivel: int
    "Quantidade de livros disponíveis para a reserva"

    def __init__(self, titulo: str, autor: str, codigo: int, ano_publicacao: int, quantidade_disponivel: int): ...
