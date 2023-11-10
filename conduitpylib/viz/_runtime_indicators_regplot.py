import itertools as it
import typing

from matplotlib import patheffects as mpl_pe
from matplotlib import pyplot as plt
import pandas as pd
import seaborn as sns

from ._get_defaults import get_default_runtime_indicators
from ._runtime_indicator_regplot import runtime_indicator_regplot


def runtime_indicators_regplot(
    data: pd.DataFrame,
    compare: str = "Instrumentation",
    comparison_x: str = "Longitudinal",
    comparison_y: str = "Snapshot",
    indicators: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
) -> sns.FacetGrid:
    if indicators is None:
        indicators = get_default_runtime_indicators()

    if palette is None:
        palette = sns.color_palette()

    melted = pd.melt(
        data,
        id_vars=[compare, "Process Instance UUID"],
        value_vars=indicators,
        var_name="Runtime Indicator",
        value_name="Value",
    )
    pivoted = pd.pivot(
        melted,
        index=["Process Instance UUID", "Runtime Indicator"],
        columns=compare,
        values="Value",
    ).reset_index()

    # hack to make a FacetGrid
    facetgrid = sns.FacetGrid(
        data=pivoted,
        col="Runtime Indicator",
        col_order=indicators,
        sharex=False,
        sharey=False,
    )
    facetgrid.set_titles(col_template="{col_name}")

    facetgrid.map_dataframe(
        runtime_indicator_regplot,
        x=comparison_x,
        y=comparison_y,
        color="purple",
    )

    # add colorcoded axis backgrounds
    for i, ax in enumerate(facetgrid.axes.flat):
        xmin, xmax = ax.get_xlim()
        ymin, ymax = ax.get_ylim()
        dx = (xmax - xmin) / 8
        dy = (ymax - ymin) / 6
        if i == 0:
            ax.add_patch(
                plt.Rectangle(
                    (xmin - dx, ymin),
                    dx,
                    ymax - ymin,
                    facecolor=palette[0],
                    alpha=0.3,
                    clip_on=False,
                    linewidth=0,
                    zorder=-999,
                )
            )

        ax.add_patch(
            plt.Rectangle(
                (xmin, ymin - dy),
                xmax - xmin,
                dy,
                facecolor=palette[1],
                alpha=0.3,
                clip_on=False,
                linewidth=0,
                zorder=-999,
            )
        )

    return facetgrid
