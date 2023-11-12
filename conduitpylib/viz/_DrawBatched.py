import typing
import sys

from matplotlib import pyplot as plt
import more_itertools as mit
import numpy as np
import pandas as pd


class DrawBatched:
    DataPlaceholder = object()
    ZorderPlaceholder = object()

    _batches: typing.Any
    _draw_func: typing.Callable

    def __init__(
        self: "DrawBatched",
        draw_func: typing.Callable,
        data: pd.DataFrame,
        batch_size: int = 1,
        batch_limit: int = sys.maxsize,
        mutate: bool = False,
        progress_apply: typing.Callable = lambda x: x,
        random_state: typing.Optional[int] = 1,
        sort_by: typing.Optional[str] = None,
    ) -> None:
        if not mutate:
            data = data.copy()

        num_batches = min(len(data) // batch_size, batch_limit)

        data = data.sample(frac=1, random_state=random_state)
        if sort_by is not None:
            data = data.sort_values(sort_by)
        data.reset_index(drop=True, inplace=True)

        self._draw_func = draw_func
        batches = np.array_split(data, num_batches)
        self._batches = progress_apply(batches)

    def __call__(self: "DrawBatched", *args, **kwargs) -> typing.Any:
        for i, batch in enumerate(self._batches):
            def replace(obj):
                return {
                    id(self.DataPlaceholder): batch,
                    id(self.ZorderPlaceholder): i,
                }.get(id(obj), obj)

            kwargs_ = {key: replace(value) for key, value in kwargs.items()}
            self._draw_func(
                *map(replace, args),
                **{"zorder": i, **kwargs_},
            )
