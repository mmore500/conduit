import matplotlib
from matplotlib import pyplot as plt
import seaborn as sns

from ._quantile_regplot import quantile_regplot


def unsplit_regression(*args, regplot, **kwargs):
    del kwargs["color"]
    regplot(
        *args,
        **kwargs,
        color="black",
        fit_reg=False,
    )
    regplot(
        *args,
        **kwargs,
        color="purple",
        scatter=False,
    )

    # adapted from https://www.scivision.dev/matplotlib-force-integer-labeling-of-axis/
    plt.gca().xaxis.set_major_locator(
        matplotlib.ticker.MaxNLocator(
            integer=True,
        ),
    )


def facet_unsplit_regression(
    *, data, col=None, row=None, x, y, regression, **kwargs
):
    g = sns.FacetGrid(
        data,
        col=col if col is not None and data[col].nunique() > 1 else None,
        row=row if row is not None and data[row].nunique() > 1 else None,
        margin_titles=True,
        sharey=False,
    )
    g.map_dataframe(
        unsplit_regression,
        regplot={
            "Ordinary Least Squares Regression": quantile_regplot,
            "Quantile Regression": quantile_regplot,
        }[regression],
        x=x,
        y=y,
        **kwargs,
    )

    # adapted from https://stackoverflow.com/a/48208266
    g.set_axis_labels(x_var=x, y_var=y)

    # adapted from https://stackoverflow.com/a/29814281
    plt.gcf().subplots_adjust(top=0.8)
    plt.gcf().suptitle(
        {
            "Quantile Regression": "Quantile Regression over Medians",
            "Ordinary Least Squares Regression": "Quantile Regression over Means",
        }[regression],
    )
