"""Visualization tools."""

from ._annotate_spearman import annotate_spearman
from ._apply_symmetric_axes import apply_symmetric_axes
from ._beleaguerment_regplot import beleaguerment_regplot
from ._beleaguerment_facetplot import beleaguerment_facetplot
from ._compact_xaxis_units import compact_xaxis_units
from ._DrawBatched import DrawBatched
from ._errplot import errplot, facet_errplot
from ._facet_barplot import facet_barplot
from ._facet_boxplot import (
    facet_boxplot,
    facet_boxplot_nofliers,
    facet_boxplot_withfliers,
)
from ._frame_scatter_subsets import frame_scatter_subsets
from ._get_notomoji_font_path import get_notomoji_font_path
from ._performance_semantics_facetplot import performance_semantics_facetplot
from ._performance_semantics_kdeplot import performance_semantics_kdeplot
from ._performance_semantics_scatterplot import (
    performance_semantics_scatterplot,
)
from ._quantile_regplot import quantile_regplot
from ._runtime_indicator_regplot import runtime_indicator_regplot
from ._runtime_indicators_concat_regplot_pairplot import (
    runtime_indicators_concat_regplot_pairplot,
)
from ._runtime_indicators_pairplot import runtime_indicators_pairplot
from ._runtime_indicators_regplot import runtime_indicators_regplot
from ._set_kde_lims import set_kde_lims
from ._unsplit_regression import facet_unsplit_regression, unsplit_regression


# adapted from https://stackoverflow.com/a/31079085
__all__ = [
    "apply_symmetric_axes",
    "annotate_spearman",
    "beleaguerment_facetplot",
    "beleaguerment_regplot",
    "compact_xaxis_units",
    "DrawBatched",
    "errplot",
    "facet_barplot",
    "facet_errplot",
    "frame_scatter_subsets",
    "facet_barplot",
    "facet_boxplot_nofliers",
    "facet_boxplot_withfliers",
    "facet_unsplit_regression",
    "get_notomoji_font_path",
    "performance_semantics_facetplot",
    "performance_semantics_kdeplot",
    "performance_semantics_scatterplot",
    "quantile_regplot",
    "runtime_indicator_regplot",
    "runtime_indicators_concat_regplot_pairplot",
    "runtime_indicators_pairplot",
    "runtime_indicators_regplot",
    "set_kde_lims",
    "unsplit_regression",
]
