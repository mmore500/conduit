import contextlib
import hashlib
import pickle
import typing

import numpy as np
import pandas as pd


def hash_with_lax_fallback(obj: typing.Any) -> int:
    """Hash an object by value, falling back to an arbitrary hash by instance.

    Parameters
    ----------
    obj : Any
        The object to hash.

    Returns
    -------
    int
        The computed hash.
    """
    with contextlib.suppress(TypeError):
        return hash(obj)

    if isinstance(obj, (pd.DataFrame, pd.Series)):
        return hash(tuple(pd.util.hash_pandas_object(obj)))

    elif isinstance(obj, np.ndarray):
        return hash(hashlib.sha256(obj).hexdigest())

    with contextlib.suppress(Exception):
        return hashlib.sha256(pickle.dumps(obj)).hexdigest()

    return id(obj)
