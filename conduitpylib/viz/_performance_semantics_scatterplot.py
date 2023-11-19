from difflib import restore
import itertools as it
import sys
import typing
import warnings

from frozendict import frozendict
from matplotlib.collections import (
    PathCollection as mpl_PathCollection,
)
from matplotlib.figure import Figure as mpl_Figure
from matplotlib.font_manager import FontProperties as mpl_FontProperties
from matplotlib.legend_handler import (
    HandlerPathCollection as mpl_HandlerPathCollection,
    HandlerLine2D as mpl_HandlerLine2D,
)
import matplotlib.lines as mpl_lines
import matplotlib.patches as mpl_patches
import matplotlib.patheffects as mpl_patheffects
from matplotlib import pyplot as plt
from matplotlib import rc as mpl_rc
import numpy as np
import opytional as opyt
import pandas as pd
import seaborn as sns

from ..utils import (
    open_dummy_axis,
    seaborn_monkeypatched_kdecache,
)
from ._compact_xaxis_units import compact_xaxis_units
from ._DrawBatched import DrawBatched
from ._get_defaults import get_default_linestyles, get_default_palette
from ._set_performance_semantics_axis_lims import (
    set_performance_semantics_axis_lims,
)
from ._frame_scatter_subsets import frame_scatter_subsets


@seaborn_monkeypatched_kdecache()
def performance_semantics_scatterplot(
    data: pd.DataFrame,
    hue: typing.Optional[str] = None,
    hue_order: typing.Optional[typing.List[str]] = None,
    heat: str = "% Msgs Lost",
    heat_norm: typing.Optional[typing.Tuple[float, float]] = (0, 100),
    x: str = "Simstep Period Inlet (ns)",
    y: str = "Latency Simsteps Inlet",
    batch_kwargs: typing.Dict = frozendict(batch_size=sys.maxsize),
    background_color: typing.Optional[str] = None,
    bunching_smear_alpha: float = 0.2,
    bunching_smear_color: str = "green",
    title: str = "",
    legend: typing.Literal["hide", "only", "show"] = "show",
    legend_contents_pad: bool = False,
    legend_font_name: typing.Optional[str] = None,
    legend_prop: typing.Dict = frozendict(),
    linestyles: typing.Optional[typing.List[str]] = None,
    palette: typing.Optional[typing.List[str]] = None,
    scatter_kwargs: typing.Dict = frozendict(),
    show_bunching_smear: bool = True,
    size_inches: typing.Tuple[float, float] = (3.5, 2.5),
    xlabel: typing.Optional[str] = None,
    xlim: typing.Optional[typing.Tuple[float, float]] = None,
    ylabel: typing.Optional[str] = None,
    ylim: typing.Optional[typing.Tuple[float, float]] = None,
) -> mpl_Figure:
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

    jointgrid = sns.JointGrid(data=data, x=x, y=y, hue=hue, ratio=8, height=8)

    if hue is not None and legend != "only":
        sns.kdeplot(
            ax=jointgrid.ax_joint,
            data=data,
            x=x,
            y=y,
            alpha=0.4,
            color=palette[0],
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

        # set linestyles of marginal KDE outlines
        # adapted from https://stackoverflow.com/a/70089200
        for path_collection, linestyle in zip(
            jointgrid.ax_joint.collections,
            it.cycle(linestyles),
        ):
            # need faux fill for set_kde_lims. set
            color = path_collection.get_facecolor()
            path_collection.set_facecolor("none")
            path_collection.set_edgecolor(color)
            path_collection.set_linestyle(linestyle)
            path_collection.set_linewidth(2)

    # PLOT JOINT DISTRIBUTION
    ###########################################################################
    def scatter_batch(data, zorder):
        # PLOT BUNCHING SMEAR ON JOINT DISTRIBUTION
        ###########################################################################
        # due to unwanted side effects, must come after legend plot
        smear_duration_updates = (
            data["Num Messages Per Laden Pull"] / data["Num Messages Per Pull"]
        ) - 1  # noqa: fmt
        if legend != "only" and show_bunching_smear:
            jointgrid.ax_joint.errorbar(
                data=data,
                x=x,
                y=y,
                fmt="none",  # don't draw dat points
                alpha=bunching_smear_alpha,
                ecolor=bunching_smear_color,
                yerr=np.vstack(
                    [
                        # below-error height
                        smear_duration_updates / 2,
                        # above-error height
                        smear_duration_updates / 2,
                    ],
                ),
                # doesn't work, need outer conditional
                # visible=(legend != "only"),
                # zorder=-5,
                # legend=False,
                zorder=zorder,
            )

        # PLOT P/S Scatter ON JOINT DISTRIBUTION
        #######################################################################
        sns.scatterplot(
            ax=jointgrid.ax_joint,
            data=data,
            x=x,
            y=y,
            hue=heat,
            legend=False,  # use dummy below instead
            hue_norm=heat_norm,
            **{
                "alpha": 0.4,
                "markers": "D",
                "palette": "flare",
                "visible": (legend != "only"),
                **scatter_kwargs,
            },
            zorder=zorder,
        )

    DrawBatched(
        scatter_batch,
        data,
        **{
            "sort_by": heat,
            **batch_kwargs,
        },
    )(
        data=DrawBatched.DataPlaceholder,
        zorder=DrawBatched.ZorderPlaceholder,
    )

    # set up joint plot axes...
    # ... annotate latency simsteps inlet values
    jointgrid.refline(y=1, ls="-", visible=(legend != "only"), zorder=-5)
    jointgrid.refline(y=2, ls="--", visible=(legend != "only"), zorder=-5)
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
        alpha=0.4 if hue is not None else 1.0,
        color=palette[0],
        fill=True,
        legend=False,
        lw=3,
        palette=palette,
        visible=(legend != "only"),
    )

    # now that we have marginals, set figure dimensions
    jointgrid.fig.set_size_inches(*size_inches)

    # set linestyles of marginal KDE outlines
    # https://stackoverflow.com/a/70089200
    for line, linestyle in it.chain(
        zip(jointgrid.ax_marg_x.collections, it.cycle(linestyles)),
        zip(jointgrid.ax_marg_y.collections, it.cycle(linestyles)),
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
            hue_norm=heat_norm,
            palette="flare",
            legend=False,
            alpha=0.03,
            expand_margins=False,
            clip_on=False,
            height=-0.8,
            visible=(legend != "only"),
            **kwarg,
        )

    # SET UP LEGEND
    ###########################################################################
    # sns.move_legend(
    #     jointgrid.ax_joint,
    #     "upper right",
    #     bbox_to_anchor=(-0.4, -0.1, 0.1, 1.2),  # place legend to left of plot
    #     frameon=False,
    # )
    # manually register category splits into legend...
    # ... add quasi-title to legend entry by providing key but no value

    prefix_labels, prefix_handles = [], []
    postfix_labels, postfix_handles = [], []
    empty_patch = mpl_patches.Patch(alpha=0.0, color="white", label="")

    if hue_order is not None:
        # prefix_handles.append(empty_patch)
        # prefix_labels.append(hue)
        # ... then add legend entries manually
        for ls, color, label in zip(linestyles, palette, hue_order):
            example_patch = mpl_patches.Patch(
                facecolor=color,
                linestyle=ls,
                label=label,
                lw=2,
                edgecolor=color,
                alpha=0.4,
            )
            prefix_handles.append(example_patch)
            prefix_labels.append(label)

        prefix_handles.append(empty_patch)
        prefix_labels.append("")

    # this title is now strung on the end of reference points below
    # if heat is not None:
    #     # don't know why, but have to manually add heat title
    #     # when scatter style is disabled
    #     prefix_handles.append(empty_patch)
    #     prefix_labels.append(heat)

    # add the bunching smear to legend
    if show_bunching_smear:
        bunching_smear_label = "Bunching Smear"
        bunching_smear_handle = mpl_lines.Line2D(
            [0],
            [0],
            color=bunching_smear_color,
            # color="white",
            # marker="_",
            # markersize=20,
            # markeredgewidth=2,
            # markeredgecolor=bunching_smear_color,
            # linestyle=None,
            linewidth=4,
            label=bunching_smear_label,
            path_effects=[
                mpl_patheffects.Stroke(linewidth=6, foreground="w"),
                mpl_patheffects.Normal(),
            ],
        )
        postfix_labels.extend([" ", bunching_smear_label])
        postfix_handles.extend([empty_patch, bunching_smear_handle])

    with open_dummy_axis() as dummy_ax:
        if heat is not None:
            # use dummy data to force full legend representation of heat
            num_legend_samples = 4
            sns.scatterplot(
                ax=dummy_ax,
                x=np.full(num_legend_samples, 0),
                y=np.full(num_legend_samples, 0),
                hue=np.linspace(*heat_norm, num_legend_samples, dtype=int),
                hue_norm=heat_norm,
                alpha=0.0,
                legend="full",
                markers="D",
                palette="flare",
            )

        (
            handles,
            labels,
        ) = dummy_ax.get_legend_handles_labels()

    if labels:
        first, *rest = map("{} %".format, labels)
        labels = [f"{first} Msgs Lost", *rest]

    if legend_contents_pad:
        pad_handles = [empty_patch]
        pad_labels = [""]
    else:
        pad_handles = []
        pad_labels = []

    handles = [
        *pad_handles,
        *prefix_handles,
        *handles,
        *postfix_handles,
        *pad_handles,
    ]
    labels = [
        *pad_labels,
        *prefix_labels,
        *labels,
        *postfix_labels,
        *pad_labels,
    ]

    # helper function to disable transparency on legend entries
    # adapted from https://stackoverflow.com/a/59629242
    def remove_alpha(handle, orig) -> None:
        handle.update_from(orig)
        handle.set_alpha(1.0)

    # create legend
    bbox_to_anchor = (
        (-0.4, -0.15, 0.1, 1.25)  # place legend to left of plot
        if legend != "only"
        else None
    )
    legend_ = jointgrid.ax_joint.legend(
        handles,
        labels,
        handler_map={
            mpl_PathCollection: mpl_HandlerPathCollection(
                update_func=remove_alpha
            ),
            plt.Line2D: mpl_HandlerLine2D(update_func=remove_alpha),
        },  # apply helper function
        borderaxespad=0,
        bbox_to_anchor=bbox_to_anchor,
        prop=legend_prop,
    )  # apply new handles and labels to plot
    # legend_._legend_box.set_height(legend_.get_bbox_to_anchor().height)

    # title background adapted from https://stackoverflow.com/a/76103441

    mpl_rc("text", usetex=False)
    thinspace = '"'
    legend_.set_title(
        f"{thinspace}{title}{thinspace}",
        # use custom font with monochrome emojis
        # because vanilla matplotlib backend doesn't support color bitmap fonts
        prop=mpl_FontProperties(fname=legend_font_name, size=30),
    )
    legend_._legend_title_box._text.set_bbox(
        {"facecolor": "gray", "alpha": 0.3, "lw": 3},
    )
    legend_._legend_title_box._text.width = 10
    legend_._legend_box.sep = 20

    if legend == "hide":
        legend_.set_visible(False)

    # FINALIZE
    ###########################################################################
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

    if xlabel is not None:
        jointgrid.ax_joint.set_xlabel(xlabel)
    if ylabel is not None:
        jointgrid.ax_joint.set_ylabel(ylabel)

        # patchworklib strips out background colors...
        # jointgrid.fig.patch.set_facecolor(background_color)
        # jointgrid.fig.patch.set_alpha(0.2)
        # ... so just set axis colors instead
    for ax, key in [
        (jointgrid.ax_joint, "bottom"),
        (jointgrid.ax_joint, "left"),
        (jointgrid.ax_marg_x, "bottom"),
        (jointgrid.ax_marg_y, "left"),
    ]:
        spine = ax.spines[key]
        spine.set_visible(True)
        spine.set_color(opyt.or_value(background_color, "black"))
        spine.set(zorder=-100)
        if background_color is not None:
            spine.set_lw(5)
        # 'both' refers to minor and major axes
        ax.tick_params(
            colors=opyt.or_value(background_color, "black"),
            length=6,
            width=2,
            which="both",
        )

    compact_xaxis_units(ax=jointgrid.ax_joint, base_unit="s")

    if legend == "only":
        # outfig = plt.figure()
        # outax = outfig.add_subplot(111)
        # outax.legend()
        # outax.legend_ = legend
        # plt.close(jointgrid.fig)
        # return outfig
        jointgrid.ax_joint.set_axis_off()
        jointgrid.ax_marg_x.set_axis_off()
        jointgrid.ax_marg_y.set_axis_off()
        jointgrid.fig.set_figwidth(1)
        jointgrid.fig.tight_layout()
        return jointgrid
    else:
        return jointgrid
