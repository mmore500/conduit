import operator
import typing

from frozendict import frozendict
import numpy as np

from matplotlib.axes import Axes as mpl_Axes
from matplotlib.collections import PathCollection as mpl_PathCollection
from matplotlib.container import ErrorbarContainer as mpl_ErrorbarContainer

from ._align_marker import align_marker


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
    edge_x, edge_y, markers = x, y, list()

    xwidth = -operator.__sub__(*xlim)
    ywidth = -operator.__sub__(*ylim)
    xoff = xwidth * offset
    yoff = ywidth * offset
    if x < xlim[0]:
        amount = int((xlim[0] - x) / xwidth)
        backslash = "\\"
        edge_x = xlim[0] - xoff
        markers.append(f"$◅{backslash * amount}$")
    elif x > xlim[1]:
        amount = int((x - xlim[1]) / xwidth)
        m = rf"$| \!\! \leftrightarrow \!\!|{{\times}}{amount}\rangle\!\rangle\!\rangle$"
        edge_x = xlim[1] + xoff
        # markers.append(align_marker(m, halign="left"))
        markers.append(align_marker(m, halign="right", pad=1.3))

    if y < ylim[0]:
        edge_y = ylim[0] - yoff
        markers.append("^")
    elif y > ylim[1]:
        edge_y = ylim[1] + yoff
        markers.append("v")

    for marker in markers:
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
            new_offsets = []
            for x, y in collection.get_offsets():
                # Draw edge marker if the point is out of bounds
                if x < xlim[0] or x > xlim[1] or y < ylim[0] or y > ylim[1]:
                    x, y = draw_edge_marker(
                        ax,
                        x,
                        y,
                        annotation_kwargs=annotation_kwargs,
                        marker_kwargs=marker_kwargs,
                        offset=offset,
                    )
                new_offsets.append((x, y))

            collection.set_offsets(new_offsets)
            collection.set(clip_on=False)

    x_width = -operator.__sub__(*xlim)
    x_thresh = xlim[1]
    x_offset = x_thresh + x_width * offset
    for container in ax.containers:
        if isinstance(container, mpl_ErrorbarContainer):
            # Unpack the container
            (
                plotline,  # Line2D instance of x, y plot markers and/or line
                caplines,  # A tuple of Line2D instances of the error bar caps
                # A tuple of LineCollection with the horizontal and vertical
                # error ranges.
                barlinecols,
            ) = container

            # Adjust each error bar
            for barlinecol in barlinecols:
                segments = barlinecol.get_segments()
                new_segments = []
                for segment in segments:
                    (x0, y0), (x1, y1) = segment
                    if x0 > x_thresh:
                        assert x0 == x1
                        x0, x1 = x_offset, x_offset
                        print(f"{x0=}, {x1=}")
                        print(f"{y0=}, {y1=}")
                    new_segment = [(x0, y0), (x1, y1)]
                    new_segments.append(new_segment)

                barlinecol.set_segments(new_segments)
                barlinecol.set(clip_on=False)
