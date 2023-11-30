from unittest.mock import patch

import patchworklib as pw
import seaborn as sns


@patch.dict("patchworklib.param", {"margin": 0})
def runtime_indicators_concat_regplot_pairplot(
    regplot_facetgrid: sns.FacetGrid,
    pairplot_grid: sns.PairGrid,
) -> pw.Brick:
    f = regplot_facetgrid
    g = pairplot_grid

    f.fig.set_size_inches(*g.fig.get_size_inches())

    for gax, fax in zip(g.axes.flat, f.axes.flat):
        # Set the position of the corresponding subplot
        fax.set_position(gax.get_position())
        axlim = gax.get_xlim()
        fax.set_ylim(*axlim)
        fax.set_xlim(*axlim)

    fbrick = pw.load_seaborngrid(f)
    gbrick = pw.load_seaborngrid(g)

    return fbrick / gbrick
