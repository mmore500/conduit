import operator
import typing

from frozendict import frozendict

from matplotlib.axes import Axes as mpl_Axes
from matplotlib.collections import PathCollection as mpl_PathCollection


def draw_edge_marker(
    ax: mpl_Axes,
    x: float,
    y: float,
    annotation_kwargs: typing.Dict,
    marker_kwargs: typing.Dict,
    offset: float = 0.10,
) -> mpl_Axes:
    """
    Draw an edge marker for a single point if it's out of bounds.

    Parameters
    ----------
    ax : matplotlib.axes.Axes
        The axes object to draw markers on.
    x, y : float
        The x and y coordinates of the point.
    """
    xlim, ylim = ax.get_xlim(), ax.get_ylim()
    edge_x, edge_y, marker, annotation = x, y, None, ""
    ha, va, xytext = "left", "center", (5, 0)  # default alignment and offset

    xwidth = -operator.__sub__(*xlim)
    ywidth = -operator.__sub__(*ylim)
    xoff = xwidth * offset
    yoff = ywidth * offset
    if x < xlim[0]:
        amount = int((xlim[0] - x) / xwidth)
        backslash = "\\"
        edge_x, marker, annotation, ha = (
            xlim[0] - xoff,
            f"$◅{backslash * amount}$",
            f"{x:.0f}",
            "right",
        )
    elif x > xlim[1]:
        amount = int((x - xlim[1]) / xwidth)
        edge_x, marker, annotation, ha = (
            xlim[1] + xoff,
            rf"$| \!\! \leftrightarrow \!\!|{{\times}}{amount}\rangle\!\rangle\!\rangle$",
            f"{x:.0f}",
            "left",
        )

    if y < ylim[0]:
        edge_y, marker, annotation, va, xytext = (
            ylim[0] - yoff,
            "^",
            f"{y:.2f}" if not annotation else f"({annotation}, {y:.2f})",
            "bottom",
            (0, 5),
        )
    elif y > ylim[1]:
        edge_y, marker, annotation, va, xytext = (
            ylim[1] + yoff,
            "v",
            f"{y:.2f}" if not annotation else f"({annotation}, {y:.2f})",
            "top",
            (0, -5),
        )

    if marker:
        ax.plot(
            edge_x,
            edge_y,
            **{
                "color": "none",
                "clip_on": False,
                "linewidth": 1,
                "color": "red",
                "marker": marker,
                "markersize": 20,
                "markeredgewidth": 0.5,
                "zorder": 10000,
                **marker_kwargs,
            },
        )
        ax.annotate(
            annotation,
            (edge_x, edge_y),
            **{
                "ha": ha,
                "va": va,
                "textcoords": "offset points",
                "xytext": xytext,
                **annotation_kwargs,
            },
        )

    return edge_x, edge_y


def draw_edge_markers(
    ax: mpl_Axes,
    annotation_kwargs: typing.Dict = frozendict(),
    marker_kwargs: typing.Dict = frozendict(),
    offset: float = 0.10,
) -> mpl_Axes:
    """
    Draw edge markers for points lying outside the plot's bounds.

    Parameters
    ----------
    ax : matplotlib.axes.Axes
        The axes object to draw markers on.
    """
    xlim, ylim = ax.get_xlim(), ax.get_ylim()

    for collection in ax.collections:
        if isinstance(collection, mpl_PathCollection):
            # Extract points from the line
            points = list(collection.get_offsets())

            new_offsets = []
            for x, y in points:
                # Draw edge marker if the point is out of bounds
                if x < xlim[0] or x > xlim[1] or y < ylim[0] or y > ylim[1]:
                    x_, y_ = draw_edge_marker(
                        ax,
                        x,
                        y,
                        annotation_kwargs=annotation_kwargs,
                        marker_kwargs=marker_kwargs,
                        offset=offset,
                    )
                else:
                    new_offsets.append((x, y))
