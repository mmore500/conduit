import typing

import numpy as np
import pytest

from conduitpylib.utils import UnequalSentinel


def test_self():
    s = UnequalSentinel()
    assert s != s and not s == s


@pytest.mark.parametrize(
    "other", [1, 0, "asdf", "", np.nan, None, True, False, UnequalSentinel()]
)
def test_other(other: typing.Any):
    s = UnequalSentinel()
    assert s != other and not s == other
    assert other != s and not other == s
