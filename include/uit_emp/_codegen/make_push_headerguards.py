#!/usr/bin/python3

import textwrap

from pylib import iter_headerguards

content = "#ifdef UIT_VENDORIZE_EMP\n"
for headerguard in iter_headerguards():


    content += textwrap.dedent(f"""\

        #pragma push_macro("{ headerguard }")
        #ifndef UIT_{ headerguard }_HEADERGUARD
        #undef { headerguard }
        #endif // #ifndef UIT_{ headerguard }_HEADERGUARD
    """)

content += "\n"
content += "#endif // #ifdef UIT_VENDORIZE_EMP"
content += "\n"

with open(f"vendorization/push_headerguards.hh", "w") as f:
    f.write(content)
