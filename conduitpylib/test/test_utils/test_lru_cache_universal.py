import copy
import pickle

import pytest

from conduitpylib.utils import lru_cache_universal


# testing helper
class MutableObj:
    def __init__(self, value):
        self.value = value

    def __eq__(self, other):
        return self.value == other.value

    def __hash__(self):
        return hash(self.value)

    def __add__(self, other):
        return MutableObj(self.value + other.value)


class UnhashUnpickle:
    def __add__(self, other):
        return UnhashUnpickle()

    def __eq__(self, other):
        return isinstance(other, UnhashUnpickle)

    def __reduce__(self):
        raise pickle.PicklingError()


@pytest.mark.parametrize(
    "args, kwargs, maxsize, expected_result",
    [
        ## ARGS
        # Test with hashable arguments
        ((1, 2), {}, None, 3),
        # Test with mutable but hashable arguments
        ((MutableObj(1), MutableObj(2)), {}, None, MutableObj(3)),
        # Test cache with a maxsize of 1, expecting eviction of the first key
        ((1, 2), {}, 1, 3),
        # Test cache with None as maxsize, which means unlimited
        ((1, 2), {}, None, 3),
        # Test with unhashable arguments
        (([1], [2]), {}, None, [1, 2]),
        # Test with unhashble and unpickleable arguments
        ((UnhashUnpickle(), UnhashUnpickle()), {}, None, UnhashUnpickle()),
        ## KWARGS
        # Test with hashable arguments
        (tuple(), {"a": 1, "b": 2}, None, 3),
        # Test with mutable but hashable arguments
        (tuple(), {"a": MutableObj(1), "b": MutableObj(2)}, None, MutableObj(3)),
        # Test cache with a maxsize of 1, expecting eviction of the first key
        (tuple(), {"a": 1, "b": 2}, 1, 3),
        # Test cache with None as maxsize, which means unlimited
        (tuple(), {"a": 1, "b": 2}, None, 3),
        # Test with unhashable arguments
        (tuple(), {"a": [1], "b": [2]}, None, [1, 2]),
        # Test with unhashble and unpickleable arguments
        (
            tuple(),
            {"a": UnhashUnpickle(), "b": UnhashUnpickle()},
            None,
            UnhashUnpickle(),
        ),
        ## ARGS AND KWARGS
        # Test with hashable arguments
        ((1,), {"b": 2}, None, 3),
        # Test with mutable but hashable arguments
        ((MutableObj(1),), {"b": MutableObj(2)}, None, MutableObj(3)),
        # Test cache with a maxsize of 1, expecting eviction of the first key
        ((1,), {"b": 2}, 1, 3),
        # Test cache with None as maxsize, which means unlimited
        ((1,), {"b": 2}, None, 3),
        # Test with unhashable arguments
        (([1],), {"b": [2]}, None, [1, 2]),
        # Test with unhashble and unpickleable arguments
        ((UnhashUnpickle(),), {"b": UnhashUnpickle()}, None, UnhashUnpickle()),
    ],
)
def test_lru_cache_universal(
    args,
    kwargs,
    maxsize,
    expected_result,
):
    def test_function(a, b):
        return a + b

    cached_test_function = lru_cache_universal(maxsize)(test_function)
    for __ in range(2):
        assert cached_test_function(*args, **kwargs) == expected_result

    try:
        args = copy.deepcopy(args)
        kwargs = copy.deepcopy(kwargs)
        assert cached_test_function(*args, **kwargs) == expected_result
    except Exception:
        uutup = (UnhashUnpickle(), UnhashUnpickle())
        if args == uutup:
            assert cached_test_function(*uutup) == expected_result


def test_cache_eviction_policy():
    call_log = []

    def logged_test_function(*args, **kwargs):
        call_log.append((args, kwargs))

    cached_test_function = lru_cache_universal(2)(logged_test_function)

    assert len(call_log) == 0
    cached_test_function(1, 2)
    assert len(call_log) == 1
    cached_test_function(2, 3)
    assert len(call_log) == 2
    cached_test_function(1, 2)
    assert len(call_log) == 2
    cached_test_function(2, 3)
    assert len(call_log) == 2
    cached_test_function(3, 4)
    assert len(call_log) == 3
    cached_test_function(2, 3)
    assert len(call_log) == 3
    cached_test_function(1, 2)
    assert len(call_log) == 4


def test_cache_clear():
    def test_function(*args, **kwargs):
        pass

    cached_test_function = lru_cache_universal(2)(test_function)

    cached_test_function(1, 2)
    cached_test_function(2, 3)
    assert len(cached_test_function.cache) == 2
    cached_test_function.cache_clear()
    assert len(cached_test_function.cache) == 0
