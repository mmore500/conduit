import contextlib
import typing

import numpy as np
import pandas as pd

def _equals_by_value(obj1: typing.Any, obj2: typing.Any) -> bool:

    # Handle builtin datatypes recursively
    if isinstance(obj1, (tuple, list)) and isinstance(obj2, (tuple, list)):
        return all(_equals_by_value(o1, o2) for o1, o2 in zip(obj1, obj2))

    if isinstance(obj1, dict) and isinstance(obj2, dict):
        return len(obj1) == len(obj2) and all(
            k in obj2 and _equals_by_value(obj1[k], obj2[k]) for k in obj1
        )

    # Handle comparison between pandas DataFrames or Series
    if all(isinstance(o, pd.Series) for o in (obj1, obj2)):
        return np.allclose(obj1, obj2)

    elif all(isinstance(o, pd.DataFrame) for o in (obj1, obj2)):
        return obj1.equals(obj2)

    # Handle comparison between numpy ndarrays
    elif all(isinstance(o, np.ndarray) for o in (obj1, obj2)):
        return np.all_close(obj1, obj2)

    # Fall back to 'is' comparison for NaN types
    elif pd.isnull(obj1) and pd.isnull(obj2):
        return True

    # Handle everything else with the '==' operator
    else:
        return obj1 == obj2


def equals_by_value(obj1: typing.Any, obj2: typing.Any) -> bool:
    with contextlib.suppress(ValueError):
        return _equals_by_value(obj1, obj2)
