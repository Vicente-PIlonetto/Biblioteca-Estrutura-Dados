from __future__ import annotations

from dataclasses import dataclass


@dataclass(frozen=True)
class ThemePalette:
    mode: str
    window_bg: str
    sidebar_bg: str
    content_bg: str
    block_bg: str
    block_alt_bg: str
    block_border: str
    text: str
    muted: str
    accent: str
    accent_hover: str
    danger: str
    danger_hover: str
    tree_bg: str
    tree_header_bg: str
    tree_selected_bg: str
    tree_text: str
    tree_header_text: str


THEMES: dict[str, ThemePalette] = {
    "dark": ThemePalette(
        mode="dark",
        window_bg="#0A0A0A",
        sidebar_bg="#121212",
        content_bg="#0E0E0E",
        block_bg="#1C1C1C",
        block_alt_bg="#161616",
        block_border="#2B2B2B",
        text="#F5F5F5",
        muted="#B4B4B4",
        accent="#2B2B2B",
        accent_hover="#3A3A3A",
        danger="#C44E4E",
        danger_hover="#A63E3E",
        tree_bg="#101010",
        tree_header_bg="#1B1B1B",
        tree_selected_bg="#2E2E2E",
        tree_text="#F0F0F0",
        tree_header_text="#FFFFFF",
    ),
    "light": ThemePalette(
        mode="light",
        window_bg="#F4F4F4",
        sidebar_bg="#E7E7E7",
        content_bg="#FAFAFA",
        block_bg="#FFFFFF",
        block_alt_bg="#F0F0F0",
        block_border="#D0D0D0",
        text="#000000",
        muted="#4A4A4A",
        accent="#111111",
        accent_hover="#2A2A2A",
        danger="#B94B4B",
        danger_hover="#A33F3F",
        tree_bg="#FFFFFF",
        tree_header_bg="#ECECEC",
        tree_selected_bg="#D9D9D9",
        tree_text="#000000",
        tree_header_text="#000000",
    ),
}


def normalize_theme(mode: str) -> str:
    normalized = mode.lower().strip()
    if normalized in {"light", "claro"}:
        return "light"
    if normalized in {"dark", "escuro"}:
        return "dark"
    return "dark"
