"""Visualization tools."""

from ._annotate_spearman import annotate_spearman
from ._apply_symmetric_axes import apply_symmetric_axes
from ._beleaguerment_regplot import beleaguerment_regplot
from ._beleaguerment_facetplot import beleaguerment_facetplot
from ._compact_xaxis_units import compact_xaxis_units
from ._frame_scatter_subsets import frame_scatter_subsets
from ._performance_semantics_facetplot import performance_semantics_facetplot
from ._performance_semantics_kdeplot import performance_semantics_kdeplot
from ._performance_semantics_scatterplot import (
    performance_semantics_scatterplot,
)
from ._runtime_indicator_regplot import runtime_indicator_regplot
from ._runtime_indicators_concat_regplot_pairplot import (
    runtime_indicators_concat_regplot_pairplot,
)
from ._runtime_indicators_pairplot import runtime_indicators_pairplot
from ._runtime_indicators_regplot import runtime_indicators_regplot
from ._set_kde_lims import set_kde_lims


# adapted from https://stackoverflow.com/a/31079085
__all__ = [
    "apply_symmetric_axes",
    "annotate_spearman",
    "beleaguerment_facetplot",
    "beleaguerment_regplot",
    "compact_xaxis_units",
    "frame_scatter_subsets",
    "performance_semantics_facetplot",
    "performance_semantics_kdeplot",
    "performance_semantics_scatterplot",
    "runtime_indicator_regplot",
    "runtime_indicators_concat_regplot_pairplot",
    "runtime_indicators_pairplot",
    "runtime_indicators_regplot",
    "set_kde_lims",
]
