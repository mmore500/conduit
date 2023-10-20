import itertools as it
import re
import typing

from ._iter_header_content import iter_header_content

def iter_macros() -> typing.Iterator[str]:
    macros = set()
    for header_content in iter_header_content():
        for candidate in it.chain(
            re.findall(
                r"^ *#define +(EMP_[a-zA-Z0-9_]+)[ (]",
                header_content,
                re.MULTILINE,
            ),
            re.findall(
                r"^ *#define +(emp_[a-zA-Z0-9_]+)[ (]",
                header_content,
                re.MULTILINE,
            ),
        ):
            if not (
                candidate.endswith("_HPP")
                or candidate.endswith("_H")
            ):
                if "EMP_NAMESPACE" not in candidate:
                    macros.add(candidate)

    yield from macros
