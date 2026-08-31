import pytest
import pandas as pd
import numpy as np

from conduitpylib.utils import hash_with_lax_fallback


# Test with hashable objects
@pytest.mark.parametrize("obj, expected", [
    (42, hash(42)),  # integer
    ("string", hash("string")),  # string
    ((1, 2), hash((1, 2))),  # tuple
])
def test_hashable_objects(obj, expected):
    assert hash_with_lax_fallback(obj) == expected

# Test with Pandas objects
def test_pandas_objects():
    df = pd.DataFrame({'col1': [1, 2], 'col2': [3, 4]})
    series = pd.Series([1, 2, 3])

    df_hash = hash_with_lax_fallback(df)
    series_hash = hash_with_lax_fallback(series)

    # The hashes should not be the default id hashes
    assert df_hash != id(df)
    assert series_hash != id(series)

    # Re-hashing should give the same result
    assert df_hash == hash_with_lax_fallback(df)
    assert series_hash == hash_with_lax_fallback(series)

# Test with Numpy arrays
def test_numpy_arrays():
    arr = np.array([1, 2, 3])

    arr_hash = hash_with_lax_fallback(arr)

    # The hash should not be the default id hash
    assert arr_hash != id(arr)

    # Re-hashing should give the same result
    assert arr_hash == hash_with_lax_fallback(arr)

# Test with unhashable and complex objects
def test_unhashable_objects():
    # Use a list as an example of unhashable object
    unhashable = [1, 2, 3]

    unhashable_hash = hash_with_lax_fallback(unhashable)

    # The hash should not be the default id hash
    assert unhashable_hash != id(unhashable)

    # The hash should be consistent for the same object content
    assert unhashable_hash == hash_with_lax_fallback([1, 2, 3])

# Test fallback for an object that fails both hash and pickle
class UnhashableAndUnpickleable:
    def __eq__(self, other):
        return isinstance(other, UnhashableAndUnpickleable)

    def __hash__(self):
        raise TypeError

    def __reduce__(self):
        raise TypeError

def test_unhashable_unpickleable_fallback():
    obj = UnhashableAndUnpickleable()
    fallback_hash = hash_with_lax_fallback(obj)

    # Should fall back to using id for hashing
    assert fallback_hash == id(obj)
