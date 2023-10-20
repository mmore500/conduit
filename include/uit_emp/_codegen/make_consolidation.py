#!/usr/bin/python3.10

import os
import textwrap

from pylib import iter_header_paths

def setup_one_entry(header_path: str) -> str:
    return (
        f'#include "../../../third-party/Empirical/include/emp/{header_path}"'
    )

header_guard = "UIT_EMP_VENDORIZATION_CONSOLIDATE_CONTEXT_SWITCHES_HPP_INCLUDE"
content = textwrap.dedent(f"""\
#pragma once
#ifndef {header_guard}
#define {header_guard}

// this header can be included by end users at the top of the translation unit
// to prevent macro inseep into the vendorized implementation

#ifdef UIT_VENDORIZE_EMP
#include "push_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

{os.linesep.join((setup_one_entry(h) for h in iter_header_paths()))}

#ifndef UIT_VENDORIZE_EMP
namespace emp {{}}
namespace uit_emp = emp;
#endif // #ifndef UIT_VENDORIZE_EMP

#ifdef UIT_VENDORIZE_EMP
#include "pop_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#endif // #ifndef {header_guard}
""")

with open("vendorization/consolidate_context_switches.hpp", "w") as header_file:
    header_file.write(content)
