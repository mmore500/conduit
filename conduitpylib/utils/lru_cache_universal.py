import contextlib
import copy
import functools
import typing

from .hash_with_lax_fallback import hash_with_lax_fallback
from .UnequalSentinel import UnequalSentinel


def _robust_copy(obj: typing.Any) -> typing.Any:
    with contextlib.suppress(Exception):
        return copy.deepcopy(obj)
    with contextlib.suppress(Exception):
        return copy.copy(obj)
    with contextlib.suppress(Exception):
        return obj


def lru_cache_universal(maxsize: typing.Optional[int] = 64) -> typing.Callable:
    """Memoizing decorator that supports calls with mutable and unhashable
    arguments.

    Mutable arguments will be hashed by value and unhashable arguments will be
    hashed by instance id. (As such, calls with unhashable arguments that
    compare equal may result in a cache miss.)

    Parameters
    ----------
    maxsize : int or None, default 64
        The cache size limit, if any.

    Returns
    -------
    function
        Memoizing decorator.
    """

    def decorator(func: typing.Callable) -> typing.Callable:
        cache = dict()

        @functools.wraps(func)
        def wrapper(*args, **kwargs) -> typing.Any:
            # prepare keys
            hashable_args = tuple(map(hash_with_lax_fallback, args))
            hashable_kwargs = frozenset(
                zip(
                    kwargs.keys(),
                    map(hash_with_lax_fallback, kwargs.values()),
                ),
            )
            query_lookup_key = (hashable_args, hashable_kwargs)
            query_full_key = (args, kwargs)

            # try cache lookup
            cached_full_key, cached_result = cache.get(
                query_lookup_key,
                (UnequalSentinel(), None),
            )

            # cache hit!
            if cached_full_key == query_full_key:
                return _robust_copy(cached_result)

            # ... cache miss!
            # trim cache if at max size
            if maxsize is not None and len(cache) >= maxsize:
                # python iterates dicts in insertion order
                del cache[next(iter(cache))]

            # compute result and cache it
            call_result = func(*args, **kwargs)
            cache[query_lookup_key] = (query_full_key, call_result)

            return _robust_copy(call_result)

        wrapper.cache = cache
        wrapper.cache_clear = cache.clear

        return wrapper

    return decorator
