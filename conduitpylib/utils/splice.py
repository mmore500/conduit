import typing


def splice(string: str, span: typing.Tuple[int, int], insert: str = "") -> str:
    """Remove index span from string and optionally insert new content.

    Parameters
    ----------
    string : str
        The original string to be spliced.
    span : tuple[int, int]
        A tuple indicating the start and end indices for the splice operation.

        The start index is inclusive and the end index is exclusive.
    insert : str, default ""
        The string to be inserted in place of the removed span.

        Defaults to an empty string, i.e., simple removal.

    Returns
    -------
    str
        The spliced string after removal and insert operation.

    Examples
    --------
    >>> splice("Hello world", (1, 6))
    'Hworld'
    >>> splice("Hello world", (0, 5), "Goodbye")
    'Goodbye world'
    """
    start, end = span
    return string[:start] + insert + string[end:]
