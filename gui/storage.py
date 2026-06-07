from __future__ import annotations

import sqlite3
from pathlib import Path

from gui.models import Book, HistoryEntry


def default_storage_path() -> Path:
    return Path(__file__).resolve().parent.parent / "data" / "biblioteca.sqlite3"


class LibraryRepository:
    def __init__(self, storage_path: Path | None = None) -> None:
        self.storage_path = storage_path or default_storage_path()
        self.books: list[Book] = []
        self.history: list[HistoryEntry] = []
        self.theme_mode = "dark"

    def _connect(self) -> sqlite3.Connection:
        self.storage_path.parent.mkdir(parents=True, exist_ok=True)
        connection = sqlite3.connect(self.storage_path)
        connection.row_factory = sqlite3.Row
        connection.execute("PRAGMA foreign_keys = ON")
        self._initialize_schema(connection)
        return connection

    def _initialize_schema(self, connection: sqlite3.Connection) -> None:
        connection.executescript(
            """
            CREATE TABLE IF NOT EXISTS books (
                code INTEGER PRIMARY KEY,
                title TEXT NOT NULL,
                author TEXT NOT NULL,
                year INTEGER NOT NULL,
                available INTEGER NOT NULL
            );

            CREATE TABLE IF NOT EXISTS history (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                user_id INTEGER NOT NULL,
                book_code INTEGER NOT NULL,
                book_title TEXT NOT NULL,
                book_author TEXT NOT NULL,
                loan_date TEXT NOT NULL,
                return_date TEXT NOT NULL,
                status TEXT NOT NULL
            );

            CREATE TABLE IF NOT EXISTS settings (
                key TEXT PRIMARY KEY,
                value TEXT NOT NULL
            );
            """
        )

    def _get_setting(self, connection: sqlite3.Connection, key: str, default: str) -> str:
        row = connection.execute("SELECT value FROM settings WHERE key = ?", (key,)).fetchone()
        return default if row is None else str(row["value"])

    def _set_setting(self, connection: sqlite3.Connection, key: str, value: str) -> None:
        connection.execute(
            "INSERT INTO settings(key, value) VALUES(?, ?) ON CONFLICT(key) DO UPDATE SET value = excluded.value",
            (key, value),
        )

    def load(self) -> None:
        with self._connect() as connection:
            self.theme_mode = self._get_setting(connection, "theme_mode", "dark")

            book_rows = connection.execute(
                "SELECT code, title, author, year, available FROM books ORDER BY code"
            ).fetchall()
            self.books = [
                Book(
                    code=int(row["code"]),
                    title=str(row["title"]),
                    author=str(row["author"]),
                    year=int(row["year"]),
                    available=int(row["available"]),
                )
                for row in book_rows
            ]

            history_rows = connection.execute(
                """
                SELECT user_id, book_code, book_title, book_author, loan_date, return_date, status
                FROM history
                ORDER BY id ASC
                """
            ).fetchall()
            self.history = [
                HistoryEntry(
                    user_id=int(row["user_id"]),
                    book_code=int(row["book_code"]),
                    book_title=str(row["book_title"]),
                    book_author=str(row["book_author"]),
                    loan_date=str(row["loan_date"]),
                    return_date=str(row["return_date"]),
                    status=str(row["status"]),
                )
                for row in history_rows
            ]

    def save(self) -> None:
        with self._connect() as connection:
            connection.execute("DELETE FROM books")
            connection.executemany(
                "INSERT INTO books(code, title, author, year, available) VALUES(?, ?, ?, ?, ?)",
                [(book.code, book.title, book.author, book.year, book.available) for book in self.books],
            )

            connection.execute("DELETE FROM history")
            connection.executemany(
                """
                INSERT INTO history(user_id, book_code, book_title, book_author, loan_date, return_date, status)
                VALUES(?, ?, ?, ?, ?, ?, ?)
                """,
                [
                    (
                        entry.user_id,
                        entry.book_code,
                        entry.book_title,
                        entry.book_author,
                        entry.loan_date,
                        entry.return_date,
                        entry.status if isinstance(entry.status, str) else entry.status.value,
                    )
                    for entry in self.history
                ],
            )

            self._set_setting(connection, "theme_mode", self.theme_mode)

    def set_theme_mode(self, mode: str) -> None:
        self.theme_mode = mode
        self.save()

    def upsert_book(self, book: Book) -> None:
        for index, existing in enumerate(self.books):
            if existing.code == book.code:
                self.books[index] = book
                return
        self.books.append(book)

    def remove_book(self, code: int) -> bool:
        original_size = len(self.books)
        self.books = [book for book in self.books if book.code != code]
        return len(self.books) != original_size

    def get_book(self, code: int) -> Book | None:
        for book in self.books:
            if book.code == code:
                return book
        return None

    def search_books(self, query: str = "", only_available: bool = False) -> list[Book]:
        return [book for book in self.books if book.matches(query, only_available)]

    def add_history_entry(self, entry: HistoryEntry) -> None:
        self.history.insert(0, entry)

    def remove_history_entry(self, index: int) -> bool:
        if 0 <= index < len(self.history):
            self.history.pop(index)
            return True
        return False

    def book_options(self) -> list[str]:
        return [f"{book.code} - {book.title}" for book in self.books]

    def total_available_copies(self) -> int:
        return sum(book.available for book in self.books)
