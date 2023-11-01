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

from ._frame_scatter_subsets import frame_scatter_subsets
from ._set_kde_lims import set_kde_lims


def performance_semantics_plot(
    data: pd.DataFrame,
    hue: str,
    hue_order: typing.Optional[typing.List[str]] = None,
    heat: str = "% Msgs Lost",
    x: str = "Simstep Period Inlet (ns)",
    y: str = "Latency Simsteps Inlet",
    linestyles: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
    size_inches: typing.Tuple[float, float] = (3.5, 2.5),
) -> sns.JointGrid:
    hue_order = ["Sans lac-417", "With lac-417"]
    assert set(hue_order) == set(data[hue].unique())

    if hue_order is None:
        hue_order = sorted(data[hue].unique())

    if linestyles is None:
        linestyles = [":", "--"]

    if palette is None:
        # alternate palettes: ["#648FFF", "#40B0A6"] & ["#40b07f", "#646eff"]
        palette = ["#5c5cff", "#64e0ff"]
    assert len(palette) == len(hue_order)

    jointgrid = sns.JointGrid(data=data, x=x, y=y, hue=hue, ratio=8, height=8)

    sns.kdeplot(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        alpha=0.4,
        cut=10,
        fill=True,  # remove fill manually below, needed for set_kde_lims
        hue=hue,
        legend=False,
        levels=2,
        log_scale=(False, True),
        palette=palette,
        thresh=1e-12,
        zorder=100,
    )

    # ensure that entire KDE is visible
    set_kde_lims(jointgrid.ax_joint, log_x=False, log_y=True)

    # set linestyles of marginal KDE outlines
    # adapted from https://stackoverflow.com/a/70089200
    for path_collection, linestyle in zip(
        jointgrid.ax_joint.collections,
        reversed(linestyles),
    ):
        # need faux fill for set_kde_lims. set
        color = path_collection.get_facecolor()
        path_collection.set_facecolor("none")
        path_collection.set_edgecolor(color)
        path_collection.set_linestyle(linestyle)
        path_collection.set_linewidth(2)

    # PLOT JOINT DISTRIBUTION
    ###########################################################################
    # scatterplot observations
    sns.scatterplot(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        hue=heat,
        markers="D",
        # style=hue,
        # markers=["o", "D"],
        palette="flare",
        alpha=0.3,
    )

    # set up joint plot axes...
    # ... annotate latency simsteps inlet values
    jointgrid.refline(y=1, ls="-", zorder=-5)
    jointgrid.refline(y=2, ls="--", zorder=-5)
    # ... set simsteps inlet to log scale
    jointgrid.ax_joint.set_yscale("log")
    # ... set simstep period time to zero minimum
    current_xmin = jointgrid.ax_joint.get_xlim()[0]
    jointgrid.ax_joint.set_xlim(xmin=min(0, current_xmin))
    # ... set communication latency to at least 0.1 minimum
    current_ymin = jointgrid.ax_joint.get_ylim()[0]
    jointgrid.ax_joint.set_ylim(ymin=min(0.1, current_ymin))
    # ... set axis labels
    jointgrid.ax_joint.set_xlabel("Update Rate (ns)")
    jointgrid.ax_joint.set_ylabel("Communication Latency (updates)")

    # PLOT MARGINAL DISTRIBUTIONS
    ###########################################################################
    # plot marginal distributions as kernel density estimates
    jointgrid.plot_marginals(
        sns.kdeplot,
        alpha=0.1,
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
            data=data,
            hue=heat,
            palette="flare",
            legend=False,
            alpha=0.03,
            expand_margins=False,
            clip_on=False,
            height=-0.8,
            **kwarg,
        )

    # SET UP LEGEND
    ###########################################################################
    # manually register category splits into legend...
    # ... add quasi-title to legend entry by providing key but no value
    (
        handles,
        labels,
    ) = (
        jointgrid.ax_joint.get_legend_handles_labels()
    )  # get existing handles and labels
    empty_patch = mpl_patches.Patch(color="none", label="Extra label")

    # don't know why, but have to manulaly add heat title
    # when scatter style is disabled
    handles = [empty_patch, *handles]
    labels = [heat, *labels]

    handles.append(empty_patch)
    labels.append(hue)
    # ... then add legend entries manually
    for ls, color, label in zip(reversed(linestyles), palette, hue_order):
        example_patch = mpl_patches.Patch(
            facecolor=color,
            linestyle=ls,
            label=label,
            lw=2,
            edgecolor=color,
            alpha=0.4,
        )
        handles.append(example_patch)
        labels.append(label)

    # helper function to disable transparency on legend entries
    # adapted from https://stackoverflow.com/a/59629242
    def remove_alpha(handle, orig) -> None:
        handle.update_from(orig)
        handle.set_alpha(1.0)

    # create legend
    legend = jointgrid.ax_joint.legend(
        handles,
        labels,
        handler_map={
            mpl_PathCollection: mpl_HandlerPathCollection(update_func=remove_alpha),
            plt.Line2D: mpl_HandlerLine2D(update_func=remove_alpha),
        },  # apply helper function
        borderaxespad=0,
        bbox_to_anchor=(-0.4, -0.1, 0.1, 1.2),  # place legend to left of plot
    )  # apply new handles and labels to plot
    legend._legend_box.set_height(legend.get_bbox_to_anchor().height)

    # title background adapted from https://stackoverflow.com/a/76103441
    title = "🙂/🤕"
    legend.set_title(
        title,
        # use custom font with monochrome emojis
        # because vanilla matplotlib backend doesn't support color bitmap fonts
        prop=mpl_FontProperties(fname="../NotoEmoji-Regular4.ttf", size=25),
    )
    legend._legend_title_box._text.set_bbox(
        {"facecolor": "gray", "alpha": 0.3, "lw": 0},
    )
    legend._legend_box.sep = 20

    # PLOT BUNCHING SMEAR ON JOINT DISTRIBUTION
    ###########################################################################
    # due to unwanted side effects, must come after legend plot

    max_latency = data["Latency Simsteps Inlet"]
    min_latency = data["Latency Simsteps Inlet"] / data["Num Messages Per Pull"]
    jointgrid.ax_joint.errorbar(
        data=data,
        x=x,
        y=y,
        fmt="none",  # don't draw data points
        alpha=0.05,
        ecolor="lightgray",
        yerr=np.vstack(
            [
                # below-error height
                data["Latency Simsteps Inlet"] - min_latency,
                # above-error height
                max_latency - data["Latency Simsteps Inlet"],
            ],
        ),
        zorder=-5,
        legend=False,
    )

    # FINALIZE
    ###########################################################################
    for ax in jointgrid.ax_marg_x, jointgrid.ax_marg_y:
        ax.get_xaxis().set_visible(False)
        ax.get_yaxis().set_visible(False)

    jointgrid.ax_joint.set_ylim(top=jointgrid.ax_joint.get_ylim()[1] * 3)
    jointgrid.ax_joint.set_xlim(right=jointgrid.ax_joint.get_xlim()[1] * 1.1)

    plt.tight_layout()
