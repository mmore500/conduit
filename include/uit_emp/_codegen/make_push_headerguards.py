#!/usr/bin/python3.9

import textwrap

from pylib import iter_headerguards

content = "#ifdef UIT_VENDORIZE_EMP\n"
for headerguard in sorted(iter_headerguards()):


    content += textwrap.dedent(f"""\

        #pragma push_macro("{ headerguard }")
        #undef {headerguard}
        #ifdef UIT_{headerguard}_HEADERGUARD
        #define {headerguard}
        #endif
    """)

content += "\n"
content += "#endif // #ifdef UIT_VENDORIZE_EMP"
content += "\n"

with open(f"vendorization/push_headerguards.hh", "w") as f:
    f.write(content)
