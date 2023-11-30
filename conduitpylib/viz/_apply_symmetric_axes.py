import itertools as it
import operator

from matplotlib import pyplot as plt
from matplotlib import ticker as mpl_ticker


def apply_symmetric_axes(ax: plt.Axes) -> None:
    """
    Configures a Matplotlib axis to have a square aspect ratio,
    the same axis limits on X and Y, and uses the default Matplotlib ticker
    to set the ticks on X and Y.

    Parameters:
    ax (matplotlib.axes.Axes): The axis to configure.
    """
    # 1) Setting the aspect of the axis to be equal (square plot)
    ax.set_aspect("equal", adjustable="box")

    # 2) Setting the same limits for x and y axes
    xlim = ax.get_xlim()
    ylim = ax.get_ylim()

    shared_lim = (min(*xlim, *ylim), max(*xlim, *ylim))
    lim_width = operator.sub(*shared_lim)

    padded_lim = (
        shared_lim[0] - lim_width * 0.05,
        shared_lim[1] + lim_width * 0.05,
    )

    ax.set_xlim(*padded_lim)
    ax.set_ylim(*padded_lim)

    # 3) Using the default Matplotlib ticker for setting the ticks
    ax.xaxis.set_major_locator(mpl_ticker.AutoLocator())
    ax.yaxis.set_major_locator(mpl_ticker.AutoLocator())
