import typing


def get_default_linestyles() -> typing.List[str]:
    return [":", "--", "-"]


def get_default_palette() -> typing.List[str]:
    # alternate palettes:
    # ["#648FFF", "#40B0A6"] or ["#40b07f", "#646eff"]
    # return ["#5c5cff", "#64e0ff", "#40B0A6"]
    return ["#91f7ff", "#ff82d4", "#91aaff"]
