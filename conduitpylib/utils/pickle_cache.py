import inspect
import shelve
import hashlib
import pickle
from pathlib import Path
from functools import wraps
import typing
from collections import OrderedDict

from .equals_by_value import equals_by_value


def _raises(callable: typing.Callable) -> bool:
    try:
        callable()
    except Exception:
        return True
    else:
        return False


def pickle_cache(maxsize: int = 256) -> typing.Callable:
    def decorator(func: typing.Callable) -> typing.Callable:
        info = {
            "hits": 0,
            "misses": 0,
        }

        @wraps(func)
        def wrapped(*args, **kwargs) -> typing.Any:
            # Serialize the arguments for hashing
            own_source = Path(__file__).read_text()
            try:
                func_source = Path(inspect.getfile(func)).read_text()
            except TypeError:
                func_source = inspect.getfile(func)
            context = (func.__qualname__, func_source, args, kwargs, own_source)
            serialized_context = str(context)
            hash_value = hashlib.sha256(serialized_context.encode()).hexdigest()

            # Define the shelve file path
            cache_dir = (
                Path.home() / ".cache" / "conduitpylib.utils.pickle_cache"
            )
            cache_dir.mkdir(exist_ok=True)
            shelve_path = cache_dir / f"{func.__qualname__}.shlv"

            with shelve.open(str(shelve_path), "c") as shelf:
                # Check if cache size limit is reached
                if len(shelf) >= maxsize:
                    # Remove least recently used item
                    oldest, *_rest = shelf.keys()
                    del shelf[oldest]

                if (
                    hash_value not in shelf
                    or _raises(lambda: shelf[hash_value])
                    or not equals_by_value(shelf[hash_value][0], context)
                ):
                    info["misses"] += 1
                    result = func(*args, **kwargs)
                    shelf[hash_value] = (context, result)
                    return result
                else:
                    info["hits"] += 1
                    # optimization: could mark item as most recently used here
                    return shelf[hash_value][1]

        wrapped.info = info
        return wrapped

    return decorator
