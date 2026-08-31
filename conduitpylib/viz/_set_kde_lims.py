import itertools as it

from matplotlib import axes as mpl_axes
import numpy as np


def _iter_path_collection(path_collection):
    yield from path_collection.get_paths()[0].vertices


def set_kde_lims(
    ax: mpl_axes.Axes,
    log_x: bool = False,
    log_y: bool = False,
    pad_cm: float = 0.0,
) -> None:
    fig_inches_x, fig_inches_y = ax.figure.get_size_inches()
    pad_inches = pad_cm / 2.54
    pad_fraction_x = pad_inches / fig_inches_x
    pad_fraction_y = pad_inches / fig_inches_y
    assert 0 <= pad_fraction_x <= 1 and 0 <= pad_fraction_y <= 1

    assert len(ax.collections)
    vertices = lambda: it.chain(*map(_iter_path_collection, ax.collections))
    frame_xmin = min(vertex[0] for vertex in vertices())
    frame_xmax = max(vertex[0] for vertex in vertices())
    frame_ymin = min(vertex[1] for vertex in vertices())
    frame_ymax = max(vertex[1] for vertex in vertices())
    assert frame_xmin <= frame_xmax and frame_ymin <= frame_ymax

    x_lb, x_ub = ax.get_xlim()
    if log_x:
        x_padding = (np.log10(x_ub) - np.log10(x_lb)) * pad_fraction_x
        assert x_padding >= 0
        frame_xmin, frame_xmax = (
            10 ** (np.log10(frame_xmin) - x_padding),
            10 ** (np.log10(frame_xmax) + x_padding),
        )
        assert frame_xmin > 0
    else:
        x_padding = (x_ub - x_lb) * pad_fraction_x
        assert x_padding >= 0
        frame_xmin -= x_padding
        frame_xmax += x_padding

    y_lb, y_ub = ax.get_ylim()
    if log_y:
        y_padding = (np.log10(y_ub) - np.log10(y_lb)) * pad_fraction_y
        assert y_padding >= 0
        frame_ymin, frame_ymax = (
            10 ** (np.log10(frame_ymin) - y_padding),
            10 ** (np.log10(frame_ymax) + y_padding),
        )
        assert frame_ymin > 0
    else:
        y_padding = (y_ub - y_lb) * pad_fraction_y
        assert y_padding >= 0
        frame_ymin -= y_padding
        frame_ymax += (y_padding,)

    # Set the plot limits
    assert frame_xmin <= frame_xmax
    assert frame_ymin <= frame_ymax

    # don't shrink viewport
    ax.set_xlim(
        min(frame_xmin, ax.get_xlim()[0]),
        max(frame_xmax, ax.get_xlim()[1]),
    )
    ax.set_ylim(
        min(frame_ymin, ax.get_ylim()[0]),
        max(frame_ymax, ax.get_ylim()[1]),
    )
