#!/usr/bin/python3.9

import textwrap

from pylib import iter_macros

content = "\n#ifdef UIT_VENDORIZE_EMP"
for macro in sorted(iter_macros()):
    new_content = textwrap.dedent(f"""\

        #ifdef {macro}

          #ifndef UIT_EXTERNAL_HAS_{macro}
            #define UIT_EXTERNAL_HAS_{macro}
            #ifndef UIT_INTERNAL_HAS_{macro}
              #define UIT_EXTERNAL_1ST_{macro}
            #endif
          #endif

          #ifndef UIT_INTERNAL_HAS_{macro}
            #pragma push_macro("{macro}")
            #undef {macro}
          #endif

          #if defined(\\
            UIT_INTERNAL_HAS_{macro}) && defined(UIT_EXTERNAL_1ST_{macro}\\
          )
            #ifndef UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
            #ifndef UIT_{macro}_INSEEP_WARNING
              #define UIT_{macro}_INSEEP_WARNING
              #pragma message(\\
                "{macro} defined first outside vendorized header,"\\
                " so outside definition will be used inside vendorized header"\\
                " on subsequent includes of the vendorized header"\\
                " --- if causes problems, try changing header include order"\\
              )
            #endif
            #endif
          #endif

        #else // #ifdef {macro}

          #ifdef UIT_INTERNAL_HAS_{macro}
            #pragma pop_macro("{macro}")
          #endif // #ifdef UIT_INTERNAL_HAS_{macro}

        #endif // #ifdef {macro}
    """)
    content += new_content

content += "\n"
content += "#endif // #ifdef UIT_VENDORIZE_EMP"
content += "\n"

with open(f"vendorization/push_macros.hh", "w") as f:
    f.write(content)
