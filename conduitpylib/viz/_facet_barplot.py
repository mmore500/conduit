import seaborn as sns


def facet_barplot(*, data, col=None, row=None, x, y, hue=None):
    g = sns.FacetGrid(
        data,
        col=col if col is not None and data[col].nunique() > 1 else None,
        row=row if row is not None and data[row].nunique() > 1 else None,
        margin_titles=True,
        sharey="row",
    )
    g.map_dataframe(
        sns.barplot,
        x=x,
        y=y,
        hue=hue,
    )

    # adapted from https://stackoverflow.com/a/48208266
    g.set_axis_labels(x_var=x, y_var=y)
