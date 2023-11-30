import typing

from matplotlib import patheffects as mpl_pe
from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns

from ._apply_symmetric_axes import apply_symmetric_axes


def runtime_indicator_regplot(
    data: pd.DataFrame,
    x: str = "Longitudinal",
    y: str = "Snapshot",
    color: str = "purple",
    ax: typing.Optional[plt.Axes] = None,
) -> plt.Axes:
    if ax is None:
        ax = plt.gca()

    sns.regplot(
        data=data,
        x=x,
        y=y,
        ax=ax,
        line_kws={
            "color": "darkorchid",
            "zorder": 100,
            "lw": 3,
            "ls": "--",
            "path_effects": [
                mpl_pe.Stroke(linewidth=6, foreground="white"),
                mpl_pe.Stroke(linewidth=4, foreground="slateblue"),
            ],
        },
        scatter_kws={
            "alpha": 0.4,
            "s": 10,
            "lw": 0.5,
            "ec": "gold",
            "color": color,
            "marker": "D",
        },
    )
    sns.kdeplot(
        data=data,
        x=x,
        y=y,
        ax=ax,
        fill=False,
        thresh=1e-6,
        cut=10,
        color="#fdff52",
        lw=0.5,
    )
    # ax.set_title(f"{indicator}")
    ax.axline((0, 0), slope=1, color="black", linestyle=":", lw=2)

    ax.grid(True)
    ax.set_axisbelow(True)

    apply_symmetric_axes(ax)

    return ax
