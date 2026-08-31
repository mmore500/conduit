from pathlib import Path

def get_notomoji_font_path() -> Path:
    return Path(__file__).parent / "assets" / "NotoEmoji-Regular.ttf"
