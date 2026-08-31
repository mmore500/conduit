import seaborn as sns


def facet_boxplot(*, data, col=None, row=None, x, y, showfliers=False):
    g = sns.FacetGrid(
        data,
        col=col if col is not None and data[col].nunique() > 1 else None,
        row=row if row is not None and data[row].nunique() > 1 else None,
        margin_titles=True,
        sharey="row",
    )
    g.map_dataframe(
        sns.boxplot,
        x,
        y,
        showfliers=showfliers,
    )


def facet_boxplot_withfliers(*, data, col=None, row=None, x, y, hue=None):
    g = sns.FacetGrid(
        data,
        col=col if col is not None and data[col].nunique() > 1 else None,
        row=row if row is not None and data[row].nunique() > 1 else None,
        margin_titles=True,
        sharey="row",
    )
    g.map_dataframe(
        sns.boxplot,
        x=x,
        y=y,
        hue=hue,
        showfliers=True,
    )

    # adapted from https://stackoverflow.com/a/48208266
    g.set_axis_labels(x_var=x, y_var=y)


def facet_boxplot_nofliers(*, data, col=None, row=None, x, y, hue=None):
    g = sns.FacetGrid(
        data,
        col=col if col is not None and data[col].nunique() > 1 else None,
        row=row if row is not None and data[row].nunique() > 1 else None,
        margin_titles=True,
        sharey="row",
    )
    g.map_dataframe(
        sns.boxplot,
        x=x,
        y=y,
        hue=hue,
        showfliers=False,
    )

    # adapted from https://stackoverflow.com/a/48208266
    g.set_axis_labels(x_var=x, y_var=y)
