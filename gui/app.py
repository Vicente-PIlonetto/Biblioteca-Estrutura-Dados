from __future__ import annotations

import sys
from pathlib import Path
from typing import Any

if __package__ is None or __package__ == "":
    sys.path.insert(0, str(Path(__file__).resolve().parent.parent))

try:
    import tkinter as tk
    from tkinter import messagebox, ttk
    import customtkinter as ctk
except ImportError as exc:  # pragma: no cover - depends on system Tk availability
    tk = None  # type: ignore[assignment]
    messagebox = None  # type: ignore[assignment]
    ttk = None  # type: ignore[assignment]
    ctk = None  # type: ignore[assignment]
    _CTK_IMPORT_ERROR = exc
else:
    _CTK_IMPORT_ERROR = None

from gui.models import Book, HistoryEntry, LoanStatus
from gui.storage import LibraryRepository
from gui.theme import THEMES, normalize_theme


THEME_FILE = Path(__file__).resolve().parent / "ctk_theme.json"


class BibliotecaApp(ctk.CTk if ctk is not None else object):
    def __init__(self, repository: LibraryRepository | None = None) -> None:
        if ctk is None:
            raise RuntimeError(
                "CustomTkinter/Tk nao esta disponivel neste ambiente. "
                "Instale o suporte ao Tk do sistema para abrir a interface grafica."
            ) from _CTK_IMPORT_ERROR

        super().__init__()

        self.repository = repository or LibraryRepository()
        self.repository.load()
        self.theme_mode = normalize_theme(self.repository.theme_mode)
        self.palette = THEMES[self.theme_mode]

        ctk.set_appearance_mode(self.theme_mode)
        ctk.set_default_color_theme(str(THEME_FILE))

        self.title("Biblioteca - Painel de Controle")
        self.geometry("1380x860")
        self.minsize(1220, 760)

        self._selected_book_code: int | None = None
        self._selected_history_index: int | None = None

        self._build_ui()
        self._refresh_all_views()

    def _build_ui(self) -> None:
        self.configure(fg_color=self.palette.window_bg)
        self.grid_columnconfigure(0, weight=0)
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(0, weight=1)

        self.sidebar = ctk.CTkFrame(self, width=320, corner_radius=0, fg_color=self.palette.sidebar_bg)
        self.sidebar.grid(row=0, column=0, sticky="nsew")
        self.sidebar.grid_rowconfigure(8, weight=1)

        self.content_shell = ctk.CTkFrame(self, corner_radius=0, fg_color=self.palette.content_bg)
        self.content_shell.grid(row=0, column=1, sticky="nsew")
        self.content_shell.grid_rowconfigure(0, weight=1)
        self.content_shell.grid_columnconfigure(0, weight=1)

        self.scroll = ctk.CTkScrollableFrame(self.content_shell, fg_color=self.palette.content_bg)
        self.scroll.grid(row=0, column=0, sticky="nsew", padx=18, pady=18)
        self.scroll.grid_columnconfigure(0, weight=1)

        self._build_sidebar()
        self._build_content_blocks()
        self._build_tree_styles()

    def _build_sidebar(self) -> None:
        self._sidebar_title(0, "Acesse seus dados por aqui", "Gerencie os livros e o historio de emprestimos devidos")

        self.theme_switch = ctk.CTkSegmentedButton(
            self.sidebar,
            values=["Escuro", "Claro"],
            command=self._on_theme_change,
        )
        self.theme_switch.set("Claro" if self.theme_mode == "light" else "Escuro")
        self.theme_switch.grid(row=1, column=0, padx=18, pady=(0, 18), sticky="ew")

        self.summary_books = self._summary_card(2, "Livros cadastrados", "0")
        self.summary_available = self._summary_card(3, "Exemplares disponiveis", "0")
        self.summary_history = self._summary_card(4, "Registros no historico", "0")

        actions = self._card(self.sidebar)
        actions.grid(row=5, column=0, padx=18, pady=(10, 14), sticky="ew")
        actions.grid_columnconfigure(0, weight=1)

        ctk.CTkButton(actions, text="Carregar do SQLite", command=self.load_from_disk).grid(row=0, column=0, padx=14, pady=(14, 10), sticky="ew")
        ctk.CTkButton(actions, text="Salvar no SQLite", command=self.save_to_disk).grid(row=1, column=0, padx=14, pady=(0, 14), sticky="ew")

        self.sidebar_note = ctk.CTkLabel(
            self.sidebar,
            text=f"Banco: {self.repository.storage_path.name}",
            text_color=self.palette.muted,
            wraplength=250,
            justify="left",
        )
        self.sidebar_note.grid(row=9, column=0, padx=20, pady=18, sticky="w")

    def _build_content_blocks(self) -> None:
        header = self._card(self.scroll)
        header.grid(row=0, column=0, padx=6, pady=(0, 18), sticky="ew")
        header.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(header, text="Painel operacional", font=("Segoe UI", 28, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, padx=20, pady=(18, 4), sticky="w"
        )
        ctk.CTkLabel(
            header, text="Utilize os blocos abaixo para gerenciar os livros e o historico de emprestimos da biblioteca", font=("Segoe UI", 13), text_color=self.palette.muted, wraplength=800, justify="left"
        ).grid(row=1, column=0, padx=20, pady=(0, 18), sticky="w")

        search_block = self._card(self.scroll, alternate=True)
        search_block.grid(row=1, column=0, padx=6, pady=(0, 18), sticky="ew")
        for column_index in range(6):
            search_block.grid_columnconfigure(column_index, weight=1)

        ctk.CTkLabel(search_block, text="Busca e filtros", font=("Segoe UI", 18, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, columnspan=6, padx=20, pady=(18, 10), sticky="w"
        )
    
        self.search_var = tk.StringVar()
        self.only_available_var = tk.BooleanVar(value=False)
        self._field(search_block, 1, 0, 3, "Pesquisar", self.search_var)
        self.available_check = ctk.CTkCheckBox(
            search_block,
            text="Somente disponíveis",
            variable=self.only_available_var,
            command=self._refresh_books_view,
        )
        self.available_check.grid(row=1, column=3, padx=(0, 12), pady=18, sticky="w")
        ctk.CTkButton(search_block, text="Filtrar", command=self._refresh_books_view).grid(row=1, column=4, padx=(0, 8), pady=18, sticky="ew")
        ctk.CTkButton(search_block, text="Limpar", fg_color=self.palette.block_alt_bg, command=self._clear_search).grid(row=1, column=5, padx=(0, 20), pady=18, sticky="ew")

        book_block = self._card(self.scroll)
        book_block.grid(row=2, column=0, padx=6, pady=(0, 18), sticky="ew")
        for column_index in range(6):
            book_block.grid_columnconfigure(column_index, weight=1)

        ctk.CTkLabel(book_block, text="Cadastro de livros", font=("Segoe UI", 18, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, columnspan=6, padx=20, pady=(18, 10), sticky="w"
        )

        self.book_code_var = tk.StringVar()
        self.book_title_var = tk.StringVar()
        self.book_author_var = tk.StringVar()
        self.book_year_var = tk.StringVar()
        self.book_available_var = tk.StringVar(value="1")

        self._field(book_block, 1, 0, 1, "Código", self.book_code_var)
        self._field(book_block, 1, 1, 2, "Título", self.book_title_var)
        self._field(book_block, 1, 3, 2, "Autor", self.book_author_var)
        self._field(book_block, 2, 0, 1, "Ano", self.book_year_var)
        self._field(book_block, 2, 1, 1, "Disponíveis", self.book_available_var)

        book_actions = ctk.CTkFrame(book_block, fg_color="transparent")
        book_actions.grid(row=2, column=3, columnspan=3, padx=20, pady=18, sticky="ew")
        for column_index in range(3):
            book_actions.grid_columnconfigure(column_index, weight=1)
        ctk.CTkButton(book_actions, text="Salvar livro", command=self.save_book).grid(row=0, column=0, padx=(0, 8), sticky="ew")
        ctk.CTkButton(book_actions, text="Limpar", fg_color=self.palette.block_alt_bg, command=self.clear_book_form).grid(row=0, column=1, padx=8, sticky="ew")
        ctk.CTkButton(book_actions, text="Remover", fg_color=self.palette.danger, hover_color=self.palette.danger_hover, command=self.remove_selected_book).grid(row=0, column=2, padx=(8, 0), sticky="ew")

        list_block = self._card(self.scroll, alternate=True)
        list_block.grid(row=3, column=0, padx=6, pady=(0, 18), sticky="ew")
        list_block.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(list_block, text="Lista de livros", font=("Segoe UI", 18, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, padx=20, pady=(18, 10), sticky="w"
        )
        self.books_tree = self._build_treeview(
            list_block,
            columns=("code", "title", "author", "year", "available"),
            headings=("Código", "Título", "Autor", "Ano", "Disponíveis"),
        )
        self.books_tree.grid(row=1, column=0, padx=14, pady=(0, 12), sticky="nsew")
        self.books_tree.bind("<<TreeviewSelect>>", self._on_book_selected)
        self.books_count_label = ctk.CTkLabel(list_block, text="0 resultados", text_color=self.palette.muted)
        self.books_count_label.grid(row=2, column=0, padx=20, pady=(0, 18), sticky="e")

        history_block = self._card(self.scroll)
        history_block.grid(row=4, column=0, padx=6, pady=(0, 18), sticky="ew")
        for column_index in range(6):
            history_block.grid_columnconfigure(column_index, weight=1)

        ctk.CTkLabel(history_block, text="Histórico de empréstimos", font=("Segoe UI", 18, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, columnspan=6, padx=20, pady=(18, 10), sticky="w"
        )

        self.history_user_var = tk.StringVar()
        self.history_code_var = tk.StringVar()
        self.history_loan_date_var = tk.StringVar()
        self.history_return_date_var = tk.StringVar()
        self.history_status_var = tk.StringVar(value=LoanStatus.NORMAL.value)
        self.history_book_var = tk.StringVar()

        self._field(history_block, 1, 0, 1, "Usuário", self.history_user_var)
        self._field(history_block, 1, 1, 1, "Código do livro", self.history_code_var)
        self._field(history_block, 1, 2, 1, "Empréstimo", self.history_loan_date_var)
        self._field(history_block, 1, 3, 1, "Devolução", self.history_return_date_var)

        ctk.CTkLabel(history_block, text="Status", text_color=self.palette.text).grid(row=1, column=4, padx=(20, 8), pady=(18, 6), sticky="w")
        self.history_status_menu = ctk.CTkOptionMenu(history_block, values=[status.value for status in LoanStatus], variable=self.history_status_var)
        self.history_status_menu.grid(row=1, column=5, padx=(0, 20), pady=18, sticky="ew")

        ctk.CTkLabel(history_block, text="Livro", text_color=self.palette.text).grid(row=2, column=0, padx=(20, 8), pady=(0, 6), sticky="w")
        self.history_book_menu = ctk.CTkOptionMenu(history_block, values=["Sem livros cadastrados"], variable=self.history_book_var)
        self.history_book_menu.grid(row=2, column=1, padx=(0, 8), pady=(0, 18), sticky="ew")

        history_actions = ctk.CTkFrame(history_block, fg_color="transparent")
        history_actions.grid(row=2, column=2, columnspan=4, padx=20, pady=(0, 18), sticky="ew")
        for column_index in range(3):
            history_actions.grid_columnconfigure(column_index, weight=1)
        ctk.CTkButton(history_actions, text="Salvar histórico", command=self.save_history_entry).grid(row=0, column=0, padx=(0, 8), sticky="ew")
        ctk.CTkButton(history_actions, text="Limpar", fg_color=self.palette.block_alt_bg, command=self.clear_history_form).grid(row=0, column=1, padx=8, sticky="ew")
        ctk.CTkButton(history_actions, text="Remover", fg_color=self.palette.danger, hover_color=self.palette.danger_hover, command=self.remove_selected_history).grid(row=0, column=2, padx=(8, 0), sticky="ew")

        history_list_block = self._card(self.scroll, alternate=True)
        history_list_block.grid(row=5, column=0, padx=6, pady=(0, 18), sticky="ew")
        history_list_block.grid_columnconfigure(0, weight=1)

        ctk.CTkLabel(history_list_block, text="Histórico recente", font=("Segoe UI", 18, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, padx=20, pady=(18, 10), sticky="w"
        )
        self.history_tree = self._build_treeview(
            history_list_block,
            columns=("user_id", "book", "loan", "return", "status"),
            headings=("Usuário", "Livro", "Empréstimo", "Devolução", "Status"),
        )
        self.history_tree.grid(row=1, column=0, padx=14, pady=(0, 12), sticky="nsew")
        self.history_tree.bind("<<TreeviewSelect>>", self._on_history_selected)
        self.history_count_label = ctk.CTkLabel(history_list_block, text="0 registros", text_color=self.palette.muted)
        self.history_count_label.grid(row=2, column=0, padx=20, pady=(0, 18), sticky="e")

        status_block = self._card(self.scroll, alternate=True)
        status_block.grid(row=6, column=0, padx=6, pady=(0, 18), sticky="ew")
        status_block.grid_columnconfigure(0, weight=1)
        ctk.CTkLabel(status_block, text="Status", font=("Segoe UI", 18, "bold"), text_color=self.palette.text).grid(
            row=0, column=0, padx=20, pady=(18, 10), sticky="w"
        )
        self.status_var = tk.StringVar(value="Pronto.")
        self.status_label = ctk.CTkLabel(status_block, textvariable=self.status_var, text_color=self.palette.muted, anchor="w", justify="left")
        self.status_label.grid(row=1, column=0, padx=20, pady=(0, 18), sticky="ew")

    def _build_tree_styles(self) -> None:
        style = ttk.Style()
        style.theme_use("clam")
        style.configure(
            "Treeview",
            background=self.palette.tree_bg,
            fieldbackground=self.palette.tree_bg,
            foreground=self.palette.text,
            rowheight=30,
            borderwidth=0,
            font=("Segoe UI", 10),
        )
        style.configure(
            "Treeview.Heading",
            background=self.palette.tree_header_bg,
            foreground=self.palette.tree_header_text,
            relief="flat",
            font=("Segoe UI", 10, "bold"),
        )
        style.map(
            "Treeview",
            background=[("selected", self.palette.tree_selected_bg)],
            foreground=[("selected", self.palette.text)],
        )

    def _card(self, parent, alternate: bool = False):
        return ctk.CTkFrame(parent, fg_color=self.palette.block_alt_bg if alternate else self.palette.block_bg, corner_radius=18)

    def _sidebar_title(self, row: int, title: str, subtitle: str) -> None:
        card = self._card(self.sidebar)
        card.grid(row=row, column=0, padx=18, pady=(18, 16), sticky="ew")
        card.grid_columnconfigure(0, weight=1)
        ctk.CTkLabel(card, text=title, font=("Segoe UI", 28, "bold"), text_color=self.palette.text).grid(row=0, column=0, padx=18, pady=(18, 4), sticky="w")
        ctk.CTkLabel(card, text=subtitle, font=("Segoe UI", 13), text_color=self.palette.muted, wraplength=260, justify="left").grid(
            row=1, column=0, padx=18, pady=(0, 18), sticky="w"
        )

    def _summary_card(self, row: int, label: str, value: str):
        card = self._card(self.sidebar, alternate=True)
        card.grid(row=row, column=0, padx=18, pady=8, sticky="ew")
        card.grid_columnconfigure(0, weight=1)
        value_label = ctk.CTkLabel(card, text=value, font=("Segoe UI", 28, "bold"), text_color=self.palette.text)
        value_label.grid(row=0, column=0, padx=18, pady=(16, 2), sticky="w")
        label_widget = ctk.CTkLabel(card, text=label, font=("Segoe UI", 12), text_color=self.palette.muted)
        label_widget.grid(row=1, column=0, padx=18, pady=(0, 16), sticky="w")
        return value_label, label_widget

    def _field(self, parent, row: int, column: int, width: int, label: str, variable: Any) -> None:
        box = ctk.CTkFrame(parent, fg_color="transparent")
        box.grid(row=row, column=column, columnspan=width, padx=20, pady=18, sticky="ew")
        box.grid_columnconfigure(0, weight=1)
        ctk.CTkLabel(box, text=label, text_color=self.palette.text).grid(row=0, column=0, sticky="w", pady=(0, 6))
        ctk.CTkEntry(box, textvariable=variable).grid(row=1, column=0, sticky="ew")

    def _build_treeview(self, parent, columns: tuple[str, ...], headings: tuple[str, ...]) -> Any:
        wrapper = ctk.CTkFrame(parent, fg_color=self.palette.block_bg, corner_radius=14)
        wrapper.grid_columnconfigure(0, weight=1)
        wrapper.grid_rowconfigure(0, weight=1)
        wrapper.grid(row=1, column=0, padx=14, pady=(0, 12), sticky="nsew")

        tree = ttk.Treeview(wrapper, columns=columns, show="headings", selectmode="browse")
        for column, heading in zip(columns, headings):
            tree.heading(column, text=heading)
            tree.column(column, anchor="w", width=140, stretch=True)

        if columns[0] == "code":
            tree.column("code", width=90, anchor="center")
            tree.column("year", width=90, anchor="center")
            tree.column("available", width=110, anchor="center")
        else:
            tree.column("user_id", width=100, anchor="center")
            tree.column("loan", width=130, anchor="center")
            tree.column("return", width=130, anchor="center")
            tree.column("status", width=110, anchor="center")

        scrollbar = ttk.Scrollbar(wrapper, orient="vertical", command=tree.yview)
        tree.configure(yscrollcommand=scrollbar.set)
        tree.grid(row=0, column=0, sticky="nsew")
        scrollbar.grid(row=0, column=1, sticky="ns")
        return tree

    def _set_status(self, message: str) -> None:
        self.status_var.set(message)

    def _clear_search(self) -> None:
        self.search_var.set("")
        self.only_available_var.set(False)
        self._refresh_books_view()

    def _refresh_all_views(self) -> None:
        self._refresh_summary()
        self._refresh_books_view()
        self._refresh_history_view()
        self._refresh_book_menu()

    def _refresh_summary(self) -> None:
        self.summary_books[0].configure(text=str(len(self.repository.books)))
        self.summary_available[0].configure(text=str(self.repository.total_available_copies()))
        self.summary_history[0].configure(text=str(len(self.repository.history)))

    def _refresh_books_view(self) -> None:
        self.books_tree.delete(*self.books_tree.get_children())
        books = self.repository.search_books(self.search_var.get(), self.only_available_var.get())
        for book in books:
            self.books_tree.insert("", "end", values=(book.code, book.title, book.author, book.year, book.available))
        self.books_count_label.configure(text=f"{len(books)} resultados")
        self._refresh_summary()
        self._refresh_book_menu()

    def _refresh_history_view(self) -> None:
        self.history_tree.delete(*self.history_tree.get_children())
        for index, entry in enumerate(self.repository.history):
            status = entry.status if isinstance(entry.status, str) else entry.status.value
            self.history_tree.insert("", "end", iid=str(index), values=(entry.user_id, entry.book_title, entry.loan_date, entry.return_date, status))
        self.history_count_label.configure(text=f"{len(self.repository.history)} registros")
        self._refresh_summary()

    def _refresh_book_menu(self) -> None:
        values = self.repository.book_options()
        if not values:
            values = ["Sem livros cadastrados"]
        self.history_book_menu.configure(values=values)
        if self.history_book_var.get() not in values:
            self.history_book_var.set(values[0])

    def _selected_tree_value(self, tree: Any) -> tuple[str, ...] | None:
        selection = tree.selection()
        if not selection:
            return None
        return tree.item(selection[0], "values")

    def _on_book_selected(self, _event: Any) -> None:
        values = self._selected_tree_value(self.books_tree)
        if not values:
            return
        self._selected_book_code = int(values[0])
        self.book_code_var.set(values[0])
        self.book_title_var.set(values[1])
        self.book_author_var.set(values[2])
        self.book_year_var.set(values[3])
        self.book_available_var.set(values[4])
        self._set_status(f"Livro selecionado: {values[1]}")

    def _on_history_selected(self, _event: Any) -> None:
        selection = self.history_tree.selection()
        if not selection:
            return
        self._selected_history_index = int(selection[0])
        self._set_status("Registro de histórico selecionado.")

    def clear_book_form(self) -> None:
        self._selected_book_code = None
        self.book_code_var.set("")
        self.book_title_var.set("")
        self.book_author_var.set("")
        self.book_year_var.set("")
        self.book_available_var.set("1")
        self._set_status("Formulário de livro limpo.")

    def clear_history_form(self) -> None:
        self._selected_history_index = None
        self.history_user_var.set("")
        self.history_code_var.set("")
        self.history_loan_date_var.set("")
        self.history_return_date_var.set("")
        self.history_status_var.set(LoanStatus.NORMAL.value)
        values = self.repository.book_options()
        self.history_book_var.set(values[0] if values else "Sem livros cadastrados")
        self._set_status("Formulário de histórico limpo.")

    def _parse_int(self, value: str, field: str) -> int:
        try:
            return int(value.strip())
        except ValueError as exc:
            raise ValueError(f"Campo '{field}' precisa ser numérico.") from exc

    def _selected_book_from_history_menu(self) -> Book | None:
        value = self.history_book_var.get().strip()
        if not value or value == "Sem livros cadastrados":
            return None

        code_text, _, _ = value.partition(" - ")
        try:
            return self.repository.get_book(int(code_text))
        except ValueError:
            return None

    def _on_theme_change(self, value: str) -> None:
        new_theme = normalize_theme(value)
        if new_theme == self.theme_mode:
            return

        self.repository.set_theme_mode(new_theme)
        self.theme_mode = new_theme
        self.palette = THEMES[self.theme_mode]
        ctk.set_appearance_mode(self.theme_mode)
        for child in self.winfo_children():
            child.destroy()
        self._build_ui()
        self._refresh_all_views()
        self._set_status(f"Tema alterado para {value.lower()}.")

    def save_book(self) -> None:
        try:
            book = Book(
                code=self._parse_int(self.book_code_var.get(), "código"),
                title=self.book_title_var.get().strip(),
                author=self.book_author_var.get().strip(),
                year=self._parse_int(self.book_year_var.get(), "ano"),
                available=self._parse_int(self.book_available_var.get(), "disponíveis"),
            )
            if not book.title or not book.author:
                raise ValueError("Título e autor não podem ficar vazios.")

            self.repository.upsert_book(book)
            self.repository.save()
            self._refresh_all_views()
            self._set_status(f"Livro salvo: {book.title}")
        except Exception as exc:  # noqa: BLE001
            messagebox.showerror("Erro ao salvar livro", str(exc))
            self._set_status("Falha ao salvar livro.")

    def remove_selected_book(self) -> None:
        values = self._selected_tree_value(self.books_tree)
        if not values:
            messagebox.showinfo("Remover livro", "Selecione um livro primeiro.")
            return

        code = int(values[0])
        if not messagebox.askyesno("Remover livro", f"Remover o livro {values[1]}?"):
            return

        if self.repository.remove_book(code):
            self.repository.save()
            self.clear_book_form()
            self._refresh_all_views()
            self._set_status(f"Livro removido: {values[1]}")

    def save_history_entry(self) -> None:
        try:
            user_id = self._parse_int(self.history_user_var.get(), "usuário")
            book_code = self._parse_int(self.history_code_var.get(), "código do livro")
            book = self.repository.get_book(book_code) or self._selected_book_from_history_menu()

            if book is None:
                raise ValueError("Não foi encontrado um livro com esse código.")

            entry = HistoryEntry(
                user_id=user_id,
                book_code=book.code,
                book_title=book.title,
                book_author=book.author,
                loan_date=self.history_loan_date_var.get().strip(),
                return_date=self.history_return_date_var.get().strip(),
                status=LoanStatus(self.history_status_var.get()),
            )

            if not entry.loan_date:
                raise ValueError("Data de empréstimo não pode ficar vazia.")

            self.repository.add_history_entry(entry)
            self.repository.save()
            self._refresh_all_views()
            self._set_status(f"Histórico salvo para o livro {book.title}")
        except Exception as exc:  # noqa: BLE001
            messagebox.showerror("Erro ao salvar histórico", str(exc))
            self._set_status("Falha ao salvar histórico.")

    def remove_selected_history(self) -> None:
        if self._selected_history_index is None:
            selection = self.history_tree.selection()
            if not selection:
                messagebox.showinfo("Remover histórico", "Selecione um registro primeiro.")
                return
            self._selected_history_index = int(selection[0])

        if not messagebox.askyesno("Remover histórico", "Remover o registro selecionado?"):
            return

        if self.repository.remove_history_entry(self._selected_history_index):
            self.repository.save()
            self.clear_history_form()
            self._refresh_all_views()
            self._set_status("Registro removido do histórico.")

    def load_from_disk(self) -> None:
        try:
            self.repository.load()
            self.theme_mode = normalize_theme(self.repository.theme_mode)
            self.palette = THEMES[self.theme_mode]
            ctk.set_appearance_mode(self.theme_mode)
            for child in self.winfo_children():
                child.destroy()
            self._build_ui()
            self._refresh_all_views()
            self._set_status(f"Dados carregados de {self.repository.storage_path}")
        except Exception as exc:  # noqa: BLE001
            messagebox.showerror("Erro ao carregar", str(exc))
            self._set_status("Falha ao carregar banco SQLite.")

    def save_to_disk(self) -> None:
        try:
            self.repository.theme_mode = self.theme_mode
            self.repository.save()
            self._set_status(f"Dados salvos em {self.repository.storage_path}")
        except Exception as exc:  # noqa: BLE001
            messagebox.showerror("Erro ao salvar", str(exc))
            self._set_status("Falha ao salvar banco SQLite.")

    def run(self) -> None:
        self.mainloop()


def run_app() -> None:
    BibliotecaApp().run()


if __name__ == "__main__":
    try:
        run_app()
    except RuntimeError as exc:
        print(str(exc), file=sys.stderr)
        raise SystemExit(1) from exc