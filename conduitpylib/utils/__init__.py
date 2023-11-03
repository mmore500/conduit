"""Miscellaneous utilities."""

from .consolidate_merge import consolidate_merge
from .count_outliers import (
    count_nonoutliers,
    count_outliers,
    count_proportion_outliers,
)
from .hash_with_lax_fallback import hash_with_lax_fallback
from .strip_end import strip_end

# adapted from https://stackoverflow.com/a/31079085
__all__ = [
    'consolidate_merge',
    'count_nonoutliers',
    'count_outliers',
    'count_proportion_outliers',
    'hash_with_lax_fallback',
    'strip_end',
]
