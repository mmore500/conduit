import typing

from frozendict import frozendict
import numpy as np
import pandas as pd
import seaborn as sns

from ._beleaguerment_regplot import beleaguerment_regplot
from ._get_defaults import get_default_palette


def beleaguerment_facetplot(
    data: pd.DataFrame,
    x: str = "Messages Received Per Second",
    y: str = "Messages Sent Per Second",
    hue: typing.Optional[str] = None,
    hue_order: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
    annotation_kwargs: typing.Dict = frozendict(),
    kde_kwargs: typing.Dict = frozendict(),
    regline_kwargs: typing.Dict = frozendict(),
    scatter_kwargs: typing.Dict = frozendict(),
    scatter_outline_kwargs: typing.Dict = frozendict(),
) -> sns.FacetGrid:
    if palette is None:
        palette = get_default_palette()

    data_real = data.copy()
    data_dummy = data.copy()

    if hue is not None:
        if hue_order is None:
            hue_order = sorted(data[hue].unique())
        if not set(hue_order) <= set(data[hue].unique()):
            raise ValueError("hue_order has hue categories missing from data.")
    elif hue_order is not None:
        raise ValueError("If hue is None, hue_order must also be None.")

    if hue is not None:
        data_dummy[hue] = "dummy"
        data_dummy[x] = np.nan
        data_dummy[y] = np.nan

    data_with_spoof = pd.concat([data_dummy, data_real])

    g = sns.FacetGrid(
        data_with_spoof,
        aspect=1.2,
        col=hue,
        hue=hue,
        hue_order=["dummy", *hue_order],
        sharex=True,
        sharey=True,
        palette=[(0.0, 0.0, 0.0, 0.0), *palette],
        height=4,
    )

    bothax = g.axes.flat[0]
    bothax.clear()
    bothax.cla()
    sns.kdeplot(
        data=data,
        x=x,
        y=y,
        hue=hue,
        hue_order=hue_order,
        ax=bothax,
        **{
            **dict(
                alpha=0.7,
                cut=10,
                fill=True,
                legend=True,
                palette=palette,
            ),
            **kde_kwargs,
        },
    )
    sns.move_legend(bothax, loc="upper left")

    bothax.axline((0, 0), slope=1, color="black", linestyle=":", lw=2)

    g.map_dataframe(
        beleaguerment_regplot,
        x=x,
        y=y,
        annotation_kwargs=annotation_kwargs,
        regline_kwargs=regline_kwargs,
        scatter_kwargs=scatter_kwargs,
        scatter_outline_kwargs=scatter_outline_kwargs,
    )

    for ax in g.axes.flat:
        ax.set_xlim(left=0)
        ax.set_ylim(bottom=0)
        ax.set_xticks(ax.get_yticks())
        ax.ticklabel_format(scilimits=(-2, 2))

    return g
