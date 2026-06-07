from __future__ import annotations

from dataclasses import dataclass
from enum import Enum
from typing import Any, Dict


class LoanStatus(str, Enum):
    NORMAL = "NORMAL"
    ENTREGUE = "ENTREGUE"
    ATRASADO = "ATRASADO"

    @property
    def label(self) -> str:
        return {
            LoanStatus.NORMAL: "Normal",
            LoanStatus.ENTREGUE: "Entregue",
            LoanStatus.ATRASADO: "Atrasado",
        }[self]


@dataclass(slots=True)
class Book:
    code: int
    title: str
    author: str
    year: int
    available: int

    def matches(self, query: str, only_available: bool = False) -> bool:
        if only_available and self.available <= 0:
            return False

        query = query.strip().lower()
        if not query:
            return True

        return (
            query in str(self.code).lower()
            or query in self.title.lower()
            or query in self.author.lower()
            or query in str(self.year).lower()
        )

    def to_dict(self) -> Dict[str, Any]:
        return {
            "code": self.code,
            "title": self.title,
            "author": self.author,
            "year": self.year,
            "available": self.available,
        }

    @classmethod
    def from_dict(cls, payload: Dict[str, Any]) -> "Book":
        return cls(
            code=int(payload["code"]),
            title=str(payload["title"]),
            author=str(payload["author"]),
            year=int(payload["year"]),
            available=int(payload["available"]),
        )


@dataclass(slots=True)
class HistoryEntry:
    user_id: int
    book_code: int
    book_title: str
    book_author: str
    loan_date: str
    return_date: str
    status: LoanStatus

    def to_dict(self) -> Dict[str, Any]:
        return {
            "user_id": self.user_id,
            "book_code": self.book_code,
            "book_title": self.book_title,
            "book_author": self.book_author,
            "loan_date": self.loan_date,
            "return_date": self.return_date,
            "status": self.status.value,
        }

    @classmethod
    def from_dict(cls, payload: Dict[str, Any]) -> "HistoryEntry":
        return cls(
            user_id=int(payload["user_id"]),
            book_code=int(payload["book_code"]),
            book_title=str(payload["book_title"]),
            book_author=str(payload.get("book_author", "")),
            loan_date=str(payload["loan_date"]),
            return_date=str(payload["return_date"]),
            status=LoanStatus(str(payload["status"])),
        )
