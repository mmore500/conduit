"""Visualization tools."""

from ._annotate_spearman import annotate_spearman
from ._beleaguerment_regplot import beleaguerment_regplot
from ._compact_xaxis_units import compact_xaxis_units
from ._frame_scatter_subsets import frame_scatter_subsets
from ._performance_semantics_facetplot import performance_semantics_facetplot
from ._performance_semantics_kdeplot import performance_semantics_kdeplot
from ._performance_semantics_scatterplot import (
    performance_semantics_scatterplot,
)
from ._set_kde_lims import set_kde_lims


# adapted from https://stackoverflow.com/a/31079085
__all__ = [
    "annotate_spearman",
    "beleaguerment_regplot",
    "compact_xaxis_units",
    "frame_scatter_subsets",
    "performance_semantics_facetplot",
    "performance_semantics_kdeplot",
    "performance_semantics_scatterplot",
    "set_kde_lims",
]
