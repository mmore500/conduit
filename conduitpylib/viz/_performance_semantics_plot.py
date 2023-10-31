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


def performance_semantics_plot(
    data: pd.DataFrame,
    hue: str,
    hue_order: typing.Optional[typing.List[str]] = None,
    heat: str = "Fraction Messages Dropped",
    x: str="Simstep Period Inlet (ns)",
    y: str="Latency Simsteps Inlet",
) -> sns.JointGrid:
    hue_order = ["Sans lac-417", "With lac-417"]
    assert set(hue_order) == set(data[hue].unique())

    palette = ["#648FFF", "#40B0A6"]
    assert len(palette) == len(hue_order)

    jointgrid = sns.JointGrid(
        data=data,
        x=x,
        y=y,
        hue=hue,
    )

    # PLOT JOINT DISTRIBUTION
    ###########################################################################
    # scatterplot observations
    sns.scatterplot(
        ax=jointgrid.ax_joint,
        data=data,
        x=x,
        y=y,
        hue=heat,
        style="Allocation",
        markers=["o", "D"],
        palette="flare",
        alpha=0.4,
    )

    # set up joint plot axes...
    # ... annotate latency simsteps inlet values
    jointgrid.refline(y=1, ls="-", zorder=-5)
    jointgrid.refline(y=2, ls="--", zorder=-5)
    # ... set simsteps inlet to log scale
    jointgrid.ax_joint.set_yscale("log")
    # ... set simstep period time to zero minimum
    jointgrid.ax_joint.set_xlim(xmin=0)
    # ... set axis labels
    jointgrid.ax_joint.set_xlabel("Update Rate (ns)")
    jointgrid.ax_joint.set_ylabel("Communication Latency (updates)")

    # outline subsets of scatter points
    frame_scatter_subsets(
        data,
        x=x,
        y=y,
        frame=hue,
        ax=jointgrid.ax_joint,
        frame_order=hue_order,
        log_y=True,
        palette=palette,
    )

    # PLOT MARGINAL DISTRIBUTIONS
    ###########################################################################
    # plot marginal distributions as kernel density estimates
    jointgrid.plot_marginals(sns.kdeplot, palette=palette, fill=True, lw=3, alpha=0.1)

    # set linestyles of marginal KDE outlines
    # https://stackoverflow.com/a/70089200
    linestyles = ["--", "-"]
    for line, linestyle in it.chain(
        zip(jointgrid.ax_marg_x.collections, linestyles),
        zip(jointgrid.ax_marg_y.collections, linestyles),
    ):
        line.set_linestyle(linestyle)
        line.set_alpha(0.4)


    # set density to loc scale
    jointgrid.ax_marg_x.set_yscale("symlog", linthresh=0.0000000001)
    jointgrid.ax_marg_y.set_xscale("symlog", linthresh=0.0001)

    sns.rugplot(
        data=data,
        ax=jointgrid.ax_marg_x,
        x=x,
        hue=heat,
        palette="flare",
        legend=False,
        alpha=0.05,
        expand_margins=False,
        clip_on=False,
        height=-0.3,
    )

    sns.rugplot(
        data=data,
        ax=jointgrid.ax_marg_y,
        y=y,
        hue=heat,
        palette="flare",
        legend=False,
        alpha=0.3,
        expand_margins=False,
        clip_on=False,
        height=-0.3,
    )

    # SET UP LEGEND
    ###########################################################################
    # manually register category splits into legend...
    # ... add quasi-title to legend entry by providing key but no value
    (
        handles,
        labels,
    ) = jointgrid.ax_joint.get_legend_handles_labels()  # get existing handles and labels
    empty_patch = mpl_patches.Patch(
        color="none", label="Extra label"
    )
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
        handle.set_alpha(1)

    # create legend
    legend = jointgrid.ax_joint.legend(
        handles,
        labels,
        handler_map={
            mpl_PathCollection: mpl_HandlerPathCollection(update_func=remove_alpha),
            plt.Line2D: mpl_HandlerLine2D(update_func=remove_alpha),
        },  # apply helper function
        borderaxespad=0,
        bbox_to_anchor=(-0.4, 0, 0.2, 1), # place legend to left of plot
    )  # apply new handles and labels to plot
    legend._legend_box.set_height(legend.get_bbox_to_anchor().height)

    # title background adapted from https://stackoverflow.com/a/76103441
    title = "\n🤕🆚🙂\n"
    legend.set_title(
        f"{title: ^4}",  # pad both sides with spaces to improve centering
        # use custom font with monochrome emojis
        # because vanilla matplotlib backend doesn't support color bitmap fonts
        prop=mpl_FontProperties(fname="../NotoEmoji-Regular.ttf", size=40),
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
