import itertools as it
import typing

import numpy as np
import matplotlib as mpl
import matplotlib.patches as mpl_patches
import matplotlib.lines as mpl_lines
import pandas as pd


def frame_scatter_subsets(
    data: pd.DataFrame,
    x: str,
    y: str,
    frame: str,
    ax: typing.Optional[mpl.axes.Axes],
    frame_order: typing.Optional[typing.Iterable] = None,
    palette: typing.Optional[typing.Iterable] = None,
    pad_cm: float = 0.5,
    log_y: bool = False,
    **kwargs,
) -> mpl.axes.Axes:
    """Draw rectangles bounding points associated with each value of `frame`
    column.

    Extra kwargs are forwarded to Rectangle initializer.
    """
    if ax is None:
        ax = mpl.pyplot.gca()

    if palette is None:
        palette = [None]

    # Calculate expansion in plot space
    x_lb, x_ub = ax.get_xlim()
    y_lb, y_ub = ax.get_ylim()
    fig_inches_x, fig_inches_y = ax.figure.get_size_inches()

    pad_inches = pad_cm / 2.54
    pad_fraction_x = pad_inches / fig_inches_x
    pad_fraction_y = pad_inches / fig_inches_y

    x_padding = (x_ub - x_lb) * pad_fraction_x
    if log_y:
        y_padding = (np.log10(y_ub) - np.log10(y_lb)) * pad_fraction_y
    else:
        y_padding = (y_ub - y_lb) * pad_fraction_y

    if frame_order is None:
        frame_order = data[frame].unique()
    linestyles = it.cycle(["-", "--", ":"])
    hues = it.cycle(palette)
    for frame_value, linestyle, hue in zip(frame_order, linestyles, hues):
        frame_data = data[data[frame] == frame_value]

        frame_min_x, frame_max_x = (
            frame_data[x].min() - x_padding,
            frame_data[x].max() + x_padding,
        )

        frame_min_y, frame_max_y = (
            (
                frame_data[y].min(),
                frame_data[y].max(),
            )
            if log_y
            else (
                frame_data[y].min(),
                frame_data[y].max(),
            )
        )

        if log_y:
            frame_min_y, frame_max_y = (
                10 ** (np.log10(frame_min_y) - y_padding),
                10 ** (np.log10(frame_max_y) + y_padding),
            )
        else:
            frame_min_y, frame_max_y = (
                frame_min_y - y_padding,
                frame_max_y + y_padding,
            )

        height = frame_max_y - frame_min_y
        width = frame_max_x - frame_min_x
        rect = mpl_patches.Rectangle(
            (frame_min_x, frame_min_y),
            width,
            height,
            linewidth=1,
            edgecolor=hue,
            facecolor="none",
            linestyle=linestyle,
            alpha=0.4,
            lw=3,
            **kwargs,
        )

        ax.add_patch(rect)

    return ax
