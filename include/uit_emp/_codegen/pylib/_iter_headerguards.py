import re
import typing

from ._iter_header_content import iter_header_content

def iter_headerguards() -> typing.Iterator[str]:
    headerguards = set()
    for header_content in iter_header_content():
        headerguards.update(re.findall(
            r"^#ifndef (EMP_[A-Z0-9_]+_H)$",
            header_content,
            re.MULTILINE,
        ))
        headerguards.update(re.findall(
            r"^#ifndef (EMP_[A-Z0-9_]+_HPP)$",
            header_content,
            re.MULTILINE,
        ))

    yield from headerguards
