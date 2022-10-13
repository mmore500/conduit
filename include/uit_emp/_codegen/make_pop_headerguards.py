#!/usr/bin/python3

import textwrap

from pylib import iter_headerguards

content = "#ifdef UIT_VENDORIZE_EXT\n"
for headerguard in iter_headerguards():

    content += textwrap.dedent(f"""\

        #ifdef {headerguard}
        #define UIT_{headerguard}_HEADERGUARD
        #endif // #ifdef {headerguard}
        #pragma pop_macro("{headerguard}")
    """)

content += "\n"
content += "#endif // #ifdef UIT_VENDORIZE_EXT"
content += "\n"

with open(f"vendorization/pop_headerguards.hh", "w") as f:
    f.write(content)
