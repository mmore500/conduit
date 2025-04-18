import contextlib

import seaborn as sns

from .equals_by_value import equals_by_value
from .lru_cache_universal import lru_cache_universal


original_eq_func = sns._statistics.KDE.__eq__
original_kde_func = sns._statistics.KDE.__call__


def patched_eq_func(self, other):
    if type(self) == type(other):
        sd, od = self.__dict__, other.__dict__
        if len(sd) != len(od):
            return False
        for key in sd:
            if key in od and equals_by_value(sd[key], od[key]):
                continue
            else:
                return False

        return True


patched_kde_func = lru_cache_universal()(original_kde_func)


def seaborn_monkeypatch_kdecache() -> None:
    sns._statistics.KDE.__call__ = patched_kde_func
    sns._statistics.KDE.__eq__ = patched_eq_func


def seaborn_unmonkeypatch_kdecache() -> None:
    sns._statistics.KDE.__call__ = original_kde_func
    sns._statistics.KDE.__eq__ = original_eq_func


@contextlib.contextmanager
def seaborn_monkeypatched_kdecache():
    seaborn_monkeypatch_kdecache()
    yield
    seaborn_unmonkeypatch_kdecache()
