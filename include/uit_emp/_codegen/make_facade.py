#!/usr/bin/python3.10

from glob import glob
import os
import re
import textwrap
import typing

def iter_header_paths() -> typing.Iterator[str]:
    # yield from glob(
    #     "**/*.hpp",
    #     recursive=True,
    #     root_dir="../../third-party/Empirical/include/emp/",
    # )
    yield from (
        "base/assert.hpp",
        "base/always_assert.hpp",
        "base/error.hpp",
        "base/errors.hpp",
        "data/DataNode.hpp",
        "data/DataFile.hpp",
        "datastructs/QueueCache.hpp",
        "datastructs/hash_utils.hpp",
        "datastructs/tuple_utils.hpp",
        "io/ContiguousStream.hpp",
        "io/MemoryIStream.hpp",
        "math/math.hpp",
        "math/Random.hpp",
        "math/random_utils.hpp",
        "meta/meta.hpp",
        "meta/TypePack.hpp",
        "polyfill/span.hpp",
        "tools/string_utils.hpp",
        "tools/keyname_utils.hpp",
    )

def setup_one_facade(header_path: str) -> None:
    header_guard = (
        "UIT_EMP_"
        + header_path.upper().replace("/", "_").replace(".", "_")
    )
    relpath = lambda x: os.path.relpath(x, os.path.dirname(header_path))

    content = textwrap.dedent(f"""\
        #ifndef {header_guard}
        #define {header_guard}

        #ifdef UIT_VENDORIZE_EMP
        #include "{relpath('vendorization/push_uit_emp.hh')}"
        #endif // #ifdef UIT_VENDORIZE_EMP

        #include "{
            relpath('../../third-party/Empirical/include/emp/')
        }/{header_path}"

        #ifndef UIT_VENDORIZE_EMP
        namespace emp {{}}
        namespace uit_emp = emp;
        #endif // #ifndef UIT_VENDORIZE_EMP

        #ifdef UIT_VENDORIZE_EMP
        #include "{relpath('vendorization/pop_uit_emp.hh')}"
        #endif // #ifdef UIT_VENDORIZE_EMP

        #endif // #ifndef {header_guard}
    """)

    if os.path.dirname(header_path):
        os.makedirs(os.path.dirname(header_path), exist_ok=True)
    with open(header_path, "w") as header_file:
        header_file.write(content)

for header_path in iter_header_paths():
    setup_one_facade(header_path)
