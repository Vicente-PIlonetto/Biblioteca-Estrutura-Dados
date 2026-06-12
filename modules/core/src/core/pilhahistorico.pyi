from .historico import Historico


class PilhaHistorico:
    livro: Historico
    "Livro associado ao histórico"

    data_emprestimo: str
    "Data do empréstimo"

    data_devolucao: str
    "Data da devolução"

    id_usuario: int
    "ID do usuário"

    status: int
    "Status do histórico"