import typing

import pandas as pd
import seaborn as sns

from ._get_defaults import get_default_runtime_indicators


def runtime_indicators_pairplot(
    data: pd.DataFrame,
    hue: typing.Optional[str] = None,
    hue_order: typing.Optional[str] = None,
    indicators: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
) -> sns.PairGrid:

    if palette is None:
        palette = sns.color_palette()

    if hue is not None:
        if hue_order is None:
            hue_order = sorted(data[hue].unique())
        if not set(hue_order) <= set(data[hue].unique()):
            raise ValueError("hue_order has hue categories missing from data.")
    elif hue_order is not None:
        raise ValueError("If hue is None, hue_order must also be None.")

    if indicators is None:
        indictors = get_default_runtime_indicators()

    # ideas for enhancement:
    # - flip a facetgrid axis so the diagonal is ascending
    # - color code axes by indicator
    g = sns.PairGrid(
        data,
        diag_sharey=False,
        vars=indictors,
        hue=hue,
        hue_order=hue_order,
    )
    g.map_diag(sns.kdeplot, common_norm=True, palette=palette)
    g.map_lower(sns.kdeplot, fill=True, thresh=1e-5, palette=palette)
    g.map_upper(sns.scatterplot, palette=palette)

    handles = g._legend_data.values()
    labels = g._legend_data.keys()
    g.axes.flat[0].legend(handles=handles, labels=labels)

    return g
