#!/usr/bin/python3.9

import textwrap

from pylib import iter_macros

content = "#ifdef UIT_VENDORIZE_EMP\n"
for macro in sorted(iter_macros()):
    new_content = textwrap.dedent(f"""\

        #ifdef {macro}

          #ifndef UIT_INTERNAL_HAS_{macro}
            #define UIT_INTERNAL_HAS_{macro}

            #ifdef UIT_EXTERNAL_1ST_{macro}
              #pragma pop_macro("{macro}")
            #endif

            #ifndef UIT_EXTERNAL_HAS_{macro}
              #ifndef UIT_INTERNAL_1ST_{macro}
                #define UIT_INTERNAL_1ST_{macro}
              #endif
            #endif

          #endif // #ifndef UIT_INTERNAL_HAS_{macro}

          #ifndef UIT_EXTERNAL_HAS_{macro}
            #pragma push_macro("{macro}")
            #undef {macro}
          #endif

        #else // #ifdef {macro}
            #ifdef UIT_EXTERNAL_HAS_{macro}
                #pragma pop_macro("{macro}")
            #endif
        #endif // #ifdef {macro}
    """)
    content += new_content

content += "\n"
content += "#endif // #ifdef UIT_VENDORIZE_EMP"
content += "\n"

with open(f"vendorization/pop_macros.hh", "w") as f:
    f.write(content)
