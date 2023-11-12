import itertools as it
import typing


from matplotlib.collections import (
    PathCollection as mpl_PathCollection,
)
from matplotlib.font_manager import FontProperties as mpl_FontProperties
from matplotlib.legend_handler import (
    HandlerPathCollection as mpl_HandlerPathCollection,
    HandlerLine2D as mpl_HandlerLine2D,
)
import matplotlib.patches as mpl_patches
from matplotlib import pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns

from ..utils import (
    lru_cache_universal,
    seaborn_monkeypatched_kdecache,
)
from ._frame_scatter_subsets import frame_scatter_subsets
from ._set_kde_lims import set_kde_lims


@lru_cache_universal()
@seaborn_monkeypatched_kdecache()
def calc_performance_semantics_axis_lims(
    data: pd.DataFrame,
    x: str = "Simstep Period Inlet (ns)",
    y: str = "Latency Simsteps Inlet",
    hue: typing.Optional[str] = None,
) -> typing.Tuple[float, float, float, float]:

    dummy_fig = plt.figure()
    dummy_ax = dummy_fig.add_subplot(111)

    sns.kdeplot(
        ax=dummy_ax,
        data=data,
        x=x,
        y=y,
        alpha=0.0,
        cut=10,
        fill=True,  # needed for set_kde_lims
        legend=False,
        levels=2,
        log_scale=(False, True),
        thresh=1e-12,
        zorder=100,
        **({"hue": hue} if hue is not None else {}),
    )

    # ensure that entire KDE is visible
    set_kde_lims(dummy_ax, log_x=False, log_y=True)
    xmin, xmax = dummy_ax.get_xlim()
    ymin, ymax = dummy_ax.get_ylim()

    # prevent dummy axes from showing in jupyter notebook
    plt.close(dummy_fig)

    # ensure adequate lower coverage
    xmin = min(0, xmin)
    ymin = min(0.1, ymin)

    # make room for rugplot elements
    xmax *= 1.1
    ymax *= 3

    return xmin, xmax, ymin, ymax
