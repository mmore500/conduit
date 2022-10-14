import subprocess
import typing

from ._get_header_content import get_header_content

def iter_header_content() -> typing.Iterator[str]:
    yield from get_header_content()
