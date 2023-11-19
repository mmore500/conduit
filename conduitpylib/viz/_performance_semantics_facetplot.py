import functools
import operator
import typing
from unittest.mock import patch
import warnings

import pandas as pd
import patchworklib as pw

from ._calc_performance_semantics_axis_lims import (
    calc_performance_semantics_axis_lims,
)
from ._get_defaults import get_default_linestyles, get_default_palette
from ._performance_semantics_kdeplot import performance_semantics_kdeplot
from ._performance_semantics_scatterplot import (
    performance_semantics_scatterplot,
)


@patch.dict(pw.param, {"margin": 0.0})
def performance_semantics_facetplot(
    data: pd.DataFrame,
    x: str = "Simstep Period Inlet (ns)",
    y: str = "Latency Simsteps Inlet",
    hue: typing.Optional[str] = None,
    hue_order: typing.Optional[typing.List[str]] = None,
    title: str = "",
    brick_size_graphs: typing.Tuple[float, float] = (0.51, 0.5),
    brick_size_legend: typing.Tuple[float, float] = (0.68, 1.2),
    legend_font_name: typing.Optional[str] = None,
    linestyles: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
) -> pw.Brick:
    if hue is not None:
        if hue_order is None:
            hue_order = sorted(data[hue].unique())
        if not set(hue_order) <= set(data[hue].unique()):
            raise ValueError("hue_order has hue categories missing from data.")
    elif hue_order is not None:
        raise ValueError(
            "If hue is None, hue_order must also be None.",
        )

    if linestyles is None:
        # default linestyles
        linestyles = get_default_linestyles()
    if hue_order is not None and len(linestyles) < len(hue_order):
        warnings.warn("more hue categories than linestyles")

    if palette is None:
        # default palette
        palette = get_default_palette()
    if hue_order is not None and len(palette) < len(hue_order):
        warnings.warn("more hue categories than palette colors")

    xmin, xmax, ymin, ymax = calc_performance_semantics_axis_lims(
        data=data,
        x=x,
        y=y,
        hue=hue,
    )


    fig_legend = performance_semantics_scatterplot(
        data=data,
        x=x,
        y=y,
        hue=hue,
        hue_order=hue_order,
        legend="only",
        legend_font_name=legend_font_name,
        linestyles=linestyles,
        palette=palette,
        title=title,
        xlim=(xmin, xmax),
        ylim=(ymin, ymax),
    )
    brick_legend = pw.load_seaborngrid(fig_legend, figsize=brick_size_legend)

    fig_kde = performance_semantics_kdeplot(
        data=data,
        x=x,
        y=y,
        hue=hue,
        hue_order=hue_order,
        palette=palette[:2],
        xlim=(xmin, xmax),
        ylim=(ymin, ymax),
    )
    brick_kde = pw.load_seaborngrid(fig_kde, figsize=brick_size_graphs)

    fig_scatterplots = [
        performance_semantics_scatterplot(
            data=data[data[hue] == hue_order[i]],
            hue=None,
            background_color=palette[i],
            legend="hide",
            linestyles=linestyles[i:],
            palette=palette[i:],
            xlim=(xmin, xmax),
            ylabel="",
            ylim=(ymin, ymax),
        )
        for i in range(len(hue_order))
    ]
    bricks_scatterplots = [
        pw.load_seaborngrid(fig_scatterplot, figsize=brick_size_graphs)
        for fig_scatterplot in fig_scatterplots
    ]

    bricks = [brick_legend, brick_kde, *bricks_scatterplots]
    concat = functools.reduce(operator.or_, bricks)

    return concat
