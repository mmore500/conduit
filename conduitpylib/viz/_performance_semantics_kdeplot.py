import itertools as it
import typing


from matplotlib.collections import (
    PathCollection as mpl_PathCollection,
)
from matplotlib import figure as mpl_figure
from matplotlib.font_manager import FontProperties as mpl_FontProperties
from matplotlib.legend_handler import (
    HandlerPathCollection as mpl_HandlerPathCollection,
    HandlerLine2D as mpl_HandlerLine2D,
)
import matplotlib.patches as mpl_patches
from matplotlib import pyplot as plt
import numpy as np
import opytional as opyt
import pandas as pd
import seaborn as sns

from ..utils import seaborn_monkeypatched_kdecache
from ._compact_xaxis_units import compact_xaxis_units
from ._frame_scatter_subsets import frame_scatter_subsets
from ._set_performance_semantics_axis_lims import (
    set_performance_semantics_axis_lims,
)


@seaborn_monkeypatched_kdecache()
def performance_semantics_kdeplot(
    data: pd.DataFrame,
    hue: str,
    hue_order: typing.Optional[typing.List[str]] = None,
    x: str = "Simstep Period Inlet (ns)",
    y: str = "Latency Simsteps Inlet",
    background_color: typing.Optional[str] = None,
    linestyles: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
    size_inches: typing.Tuple[float, float] = (3.5, 2.5),
    xlim: typing.Optional[typing.Tuple[float, float]] = None,
    ylim: typing.Optional[typing.Tuple[float, float]] = None,
) -> mpl_figure.Figure:
    assert set(hue_order) >= set(data[hue])

    if hue_order is None:
        hue_order = sorted(data[hue].unique())

    if linestyles is None:
        linestyles = [":", "--"]

    if palette is None:
        # alternate palettes: ["#648FFF", "#40B0A6"] & ["#40b07f", "#646eff"]
        palette = ["#5c5cff", "#64e0ff"]

    assert len(palette) >= len(hue_order)

    jointgrid = sns.JointGrid(
        data=data,
        x=x,
        y=y,
        hue=hue,
        hue_order=hue_order,
        ratio=8,
        height=8,
    )

    sns.kdeplot(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        alpha=0.4,
        cut=10,
        fill=True,
        hue=hue,
        hue_order=hue_order,
        legend=False,
        levels=2,
        log_scale=(False, True),
        palette=palette,
        thresh=1e-12,
        zorder=101,
    )

    # set linestyles of marginal KDE outlines
    # adapted from https://stackoverflow.com/a/70089200
    for path_collection, linestyle in zip(
        jointgrid.ax_joint.collections,
        reversed(linestyles),
    ):
        # need faux fill for set_kde_lims. set
        color = path_collection.get_facecolor()
        path_collection.set_facecolor("none")
        path_collection.set_edgecolor("white")
        path_collection.set_linestyle(linestyle)
        path_collection.set_linewidth(2)

    sns.kdeplot(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        alpha=0.4,
        cut=10,
        fill=True,
        hue=hue,
        hue_order=hue_order,
        legend=False,
        levels=2,
        log_scale=(False, True),
        palette=palette,
        thresh=1e-12,
        zorder=99,
    )

    sns.kdeplot(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        alpha=1.0,
        cut=10,
        fill=False,
        hue=hue,
        hue_order=hue_order,
        legend=False,
        levels=2,
        log_scale=(False, True),
        linewidth=4,
        linestyles="solid",
        palette=palette,
        thresh=1e-12,
        zorder=100,
    )

    # set up joint plot axes...
    # ... annotate latency simsteps inlet values
    jointgrid.refline(y=1, ls="-", zorder=-5)
    jointgrid.refline(y=2, ls="--", zorder=-5)
    # ... set simsteps inlet to log scale
    jointgrid.ax_joint.set_yscale("log")

    # ... set axis labels
    jointgrid.ax_joint.set_xlabel("Update Rate (ns)")
    jointgrid.ax_joint.set_ylabel("Communication Latency (updates)")

    # PLOT MARGINAL DISTRIBUTIONS
    ###########################################################################
    # plot marginal distributions as kernel density estimates
    jointgrid.plot_marginals(
        sns.kdeplot,
        alpha=0.1 if hue is not None else 1.0,
        fill=True,
        lw=3,
        palette=palette,
    )

    # now that we have marginals, set figure dimensions
    jointgrid.fig.set_size_inches(*size_inches)

    # set linestyles of marginal KDE outlines
    # https://stackoverflow.com/a/70089200
    for line, linestyle in it.chain(
        zip(jointgrid.ax_marg_x.collections, linestyles),
        zip(jointgrid.ax_marg_y.collections, linestyles),
    ):
        line.set_linestyle(linestyle)
        line.set_alpha(0.4)

    # do manual rugplot because plot_marginals messes up colormapping
    for ax, kwarg in (
        (jointgrid.ax_marg_x, {"x": x}),
        (jointgrid.ax_marg_y, {"y": y}),
    ):
        sns.rugplot(
            ax=ax,
            color=palette[0],
            data=data,
            hue=hue,
            hue_order=hue_order,
            palette=palette,
            legend=False,
            alpha=0.03,
            expand_margins=False,
            clip_on=False,
            height=-0.8,
            **kwarg,
        )

    # FINALIZE
    ###########################################################################
    if background_color is not None:
        jointgrid.fig.patch.set_facecolor(background_color)
        jointgrid.fig.patch.set_alpha(0.2)
    for ax, key in [
        (jointgrid.ax_joint, "bottom"),
        (jointgrid.ax_joint, "left"),
        (jointgrid.ax_marg_x, "bottom"),
        (jointgrid.ax_marg_y, "left"),
    ]:
        ax.tick_params(
            colors="black",
            length=6,
            width=2,
            which="both",
        )

    for ax in jointgrid.ax_marg_x, jointgrid.ax_marg_y:
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_visible(False)

    set_performance_semantics_axis_lims(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        hue=hue,
    )
    if xlim is not None:
        jointgrid.ax_joint.set_xlim(xlim)
    if ylim is not None:
        jointgrid.ax_joint.set_ylim(ylim)

    compact_xaxis_units(ax=jointgrid.ax_joint, base_unit="s")

    return jointgrid
