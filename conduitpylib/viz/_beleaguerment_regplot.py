import typing

from frozendict import frozendict
from matplotlib import axes as mpl_axes
from matplotlib import patheffects as mpl_pe
from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns

from ._annotate_spearman import annotate_spearman


def beleaguerment_regplot(
    data: pd.DataFrame,
    x: str = "Messages Received Per Second",
    y: str = "Messages Sent Per Second",
    ax: mpl_axes.Axes = None,
    annotation_kwargs: typing.Dict = frozendict(),
    regline_kwargs: typing.Dict = frozendict(),
    scatter_kwargs: typing.Dict = frozendict(),
    scatter_outline_kwargs: typing.Dict = frozendict(),
    **kwargs,
) -> mpl_axes.Axes:
    if ax is None:
        ax = plt.gca()

    sns.regplot(
        data=data,
        x=x,
        y=y,
        ax=ax,
        line_kws={
            **dict(
                # zorder=100,
                lw=3,
                ls="--",
                path_effects=[
                    mpl_pe.Stroke(linewidth=4, foreground="white"),
                    mpl_pe.Stroke(linewidth=2, foreground="lightsalmon"),
                ],
            ),
            **regline_kwargs,
        },
        scatter_kws=dict(
            lw=0.5,
            marker="D",
            **{
                **scatter_kwargs,
                **kwargs,
            },
        ),
        truncate=False,
    )

    # outline scattered points
    sns.kdeplot(
        data=data,
        x=x,
        y=y,
        ax=ax,
        fill=False,
        **{
            **dict(
                thresh=1e-6,
                cut=10,
                lw=0.5,
                alpha=0.6,
                levels=1,
            ),
            **scatter_outline_kwargs,
        },
    )

    annotate_spearman(
        data=data, x=x, y=y, ax=ax, **annotation_kwargs
    )
