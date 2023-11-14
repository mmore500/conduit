import typing


def get_default_linestyles() -> typing.List[str]:
    return [":", "--", "-"]


def get_default_palette() -> typing.List[str]:
    # alternate palettes:
    # ["#648FFF", "#40B0A6"] or ["#40b07f", "#646eff"]
    # return ["#5c5cff", "#64e0ff", "#40B0A6"]
    # return ["#91f7ff", "#ff82d4", "#91aaff"]
    return ['#9467bd', '#bcbd22', '#17becf']  # purple, gold, teal


def get_default_runtime_indicators() -> typing.List[str]:
    return [
        # "Num Messages Per Laden Pull",
        "Latency, Log10 Updates",
        "ms Per Update",
        "Msgs Received Per Sent",
        "Bunching",
        # "Delivery Failure, %",
    ]
