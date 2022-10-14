#!/usr/bin/python3.10

import os
import re
import textwrap
import typing

from pylib import iter_header_paths

def setup_one_facade(header_path: str) -> None:
    header_guard = (
        "UIT_EMP_"
        + header_path.upper().replace("/", "_").replace(".", "_")
        + "_INCLUDE"
    )
    relpath = lambda x: os.path.relpath(x, os.path.dirname(header_path))

    content = textwrap.dedent(f"""\
        #pragma once
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
