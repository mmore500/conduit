import typing

from matplotlib import pyplot as plt
import pandas as pd

from ._calc_performance_semantics_axis_lims import (
    calc_performance_semantics_axis_limits,
)


def set_performance_semantics_axis_limits(
    ax: plt.Axes,
    data: pd.DataFrame,
    x: str = "Simstep Period Inlet (ns)",
    y: str = "Latency Simsteps Inlet",
    hue: typing.Optional[str] = None,
) -> None:

    xmin, xmax, ymin, ymax = calc_performance_semantics_axis_limits(
        data=data,
        x=x,
        y=y,
        hue=hue,
    )
    ax.set_xlim(left=xmin, right=xmax)
    ax.set_ylim(bottom=ymin, top=ymax)
