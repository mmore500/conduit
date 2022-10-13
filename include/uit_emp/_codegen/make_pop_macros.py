#!/usr/bin/python3

import textwrap

from pylib import iter_macros

content = "#ifdef UIT_VENDORIZE_EMP\n"
content_assert = "#ifdef UIT_VENDORIZE_EMP\n"
for macro in iter_macros():
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

        #endif // #ifdef {macro}
    """)
    content += new_content
    if "assert" in macro:
        content_assert += new_content

content += "\n"
content += "#endif // #ifdef UIT_VENDORIZE_EMP"
content += "\n"

with open(f"vendorization/pop_macros.hh", "w") as f:
    f.write(content)

content_assert += "\n"
content_assert += "#endif // #ifdef UIT_VENDORIZE_EMP"
content_assert += "\n"

with open(f"vendorization/pop_assert_macros.hh", "w") as f:
    f.write(content_assert)
