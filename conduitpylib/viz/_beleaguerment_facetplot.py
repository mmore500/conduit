import typing

from frozendict import frozendict
import numpy as np
import pandas as pd
import seaborn as sns

from ._apply_symmetric_axes import apply_symmetric_axes
from ._DrawBatched import DrawBatched
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
    rugplot_kwargs: typing.Dict = frozendict(),
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
        sharex=False,
        sharey=False,
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
            "alpha": 0.7,
            "cut": 10,
            "fill": True,
            "legend": True,
            "palette": palette,
            **kde_kwargs,
        },
    )
    sns.rugplot(
        data=data,
        x=x,
        y=y,
        ax=bothax,
        **{
            "color": "darkgray",
            "linewidth": 20,
            "height": 0.12,
            "legend": False,
        },
    )

    batching_kwargs = {
        key: rugplot_kwargs.pop(key)
        for key in (
            "batch_size",
            "batch_limit",
            "progress_apply",
            "random_state",
            "sort_by",
        )
        if key in rugplot_kwargs
    }
    DrawBatched(sns.rugplot, data, **batching_kwargs)(
        data=DrawBatched.DataPlaceholder,
        x=x,
        y=y,
        hue=hue,
        hue_order=hue_order,
        ax=bothax,
        **{
            "linewidth": 5,
            "height": 0.1,
            "legend": False,
            "palette": palette,
            **rugplot_kwargs,
        },
    )
    sns.move_legend(bothax, loc="upper left")

    bothax.axline((0, 0), slope=1, color="black", linestyle=":", lw=2)

    g.axes.flat[0].set_xlim(left=0)
    g.axes.flat[0].set_ylim(bottom=0)

    map(apply_symmetric_axes, g.axes.flat)

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
        ax.ticklabel_format(scilimits=(-2, 2))
        ax.grid(True)

    return g
