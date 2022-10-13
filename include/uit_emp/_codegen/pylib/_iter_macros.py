from glob import glob
import re
import typing

def iter_macros() -> typing.Iterator[str]:
    header_paths = glob(
        "../../third-party/Empirical/include/emp/**/*.hpp",
        recursive=True,
    )

    macros = set()
    for header_path in header_paths:
        with open(header_path) as header_file:
            for candidate in re.findall(
                r"^ *#define +([a-zA-Z0-9_]+)[ (]",
                header_file.read(),
                re.MULTILINE,
            ):
                if not candidate.endswith("_HPP"):
                    macros.add(candidate)


    yield from macros
