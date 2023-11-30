from matplotlib import pyplot as plt
import numpy as np
import seaborn as sns

# adapted from https://stackoverflow.com/questions/30385975/seaborn-factor-plot-custom-error-bars
# and https://pandas.pydata.org/pandas-docs/stable/user_guide/visualization.html#visualization-errorbars
def errplot(x, y, err_lb, err_ub, **kwargs):
    ax = plt.gca()
    data = kwargs.pop("data")
    yerr = np.abs(
        data[[err_lb, err_ub]].to_numpy() - data[[y, y]].to_numpy()
    ).transpose()
    plt.axhline(
        y=0,
        zorder=1,
        color="black",
        linewidth=2,
    )
    data.plot(
        x=x,
        y=y,
        yerr=yerr,
        kind="bar",
        ax=ax,
        zorder=3,
        **kwargs,
    ).grid(
        axis="y",
        zorder=0,
    )

    if x is None:
        # adapted from https://stackoverflow.com/a/12998531
        plt.tick_params(
            axis="x",  # changes apply to the x-axis
            which="both",  # both major and minor ticks are affected
            bottom=False,  # ticks along the bottom edge are off
            top=False,  # ticks along the top edge are off
            labelbottom=False,
        )


def facet_errplot(
    *,
    data,
    x=None,
    y,
    err_lb,
    err_ub,
    estimated_statistic,
    col=None,
    row=None,
    size_inches=None,
    **kwargs,
):
    g = sns.FacetGrid(
        data,
        col=col if col is not None and data[col].nunique() > 1 else None,
        row=row if row is not None and data[row].nunique() > 1 else None,
        margin_titles=True,
        sharey=False,
    )
    g.map_dataframe(
        errplot,
        x,
        y,
        err_lb,
        err_ub,
        **kwargs,
    )

    if size_inches is not None:
        plt.gcf().set_size_inches(*size_inches)

    # adapted from https://stackoverflow.com/a/29814281
    plt.gcf().subplots_adjust(top=0.9)
    plt.gcf().suptitle(
        f"Estimated Statistic = {estimated_statistic}",
    )
