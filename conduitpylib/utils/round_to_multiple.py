def round_to_multiple(number: float, multiple: int) -> int:
    """Round to the nearest multiple of an integer.

    Parameters
    ----------
    number : float
        The number to be rounded.
    multiple : float
        The multiple to which the number will be rounded.

    Returns
    -------
    float
        The number rounded to the nearest multiple.
    """
    res = int(round(number / multiple) * multiple)
    assert res % multiple == 0
    return res
