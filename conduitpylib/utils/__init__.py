"""Miscellaneous utilities."""

from .consolidate_merge import consolidate_merge
from .count_outliers import (
    count_nonoutliers,
    count_outliers,
    count_proportion_outliers,
)
from .get_signif_asterisks import get_signif_asterisks
from .hash_with_lax_fallback import hash_with_lax_fallback
from .lru_cache_universal import lru_cache_universal
from .open_dummy_axis import open_dummy_axis
from .round_to_multiple import round_to_multiple
from .pickle_cache import pickle_cache
from .seaborn_monkeypatch_kdecache import (
    seaborn_monkeypatch_kdecache,
    seaborn_monkeypatched_kdecache,
    seaborn_unmonkeypatch_kdecache,
)
from .splice import splice
from .strip_end import strip_end
from .UnequalSentinel import UnequalSentinel


# adapted from https://stackoverflow.com/a/31079085
__all__ = [
    'consolidate_merge',
    'count_nonoutliers',
    'count_outliers',
    'count_proportion_outliers',
    'hash_with_lax_fallback',
    'lru_cache_universal',
    'get_signif_asterisks',
    'open_dummy_axis',
    'round_to_multiple',
    'pickle_cache',
    'seaborn_monkeypatch_kdecache',
    'seaborn_monkeypatched_kdecache',
    'seaborn_unmonkeypatch_kdecache',
    'splice',
    'strip_end',
    'UnequalSentinel',
]
