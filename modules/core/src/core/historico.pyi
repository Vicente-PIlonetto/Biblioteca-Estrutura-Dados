from .livro import Livro
from enum import IntEnum


class StatusHistorico(IntEnum):
    NORMAL = 0
    ENTREGUE = 1
    ATRASADO = 2


class Historico:
    Livro: Livro
    data_emprestimo: str
    dataDevolucao: str

    idUsuario: int
    status: StatusHistorico