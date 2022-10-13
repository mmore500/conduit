
#ifdef UIT_VENDORIZE_EMP
#ifdef emp_assert

  #ifndef UIT_EXTERNAL_HAS_emp_assert
    #define UIT_EXTERNAL_HAS_emp_assert
    #ifndef UIT_INTERNAL_HAS_emp_assert
      #define UIT_EXTERNAL_1ST_emp_assert
    #endif
  #endif

  #ifndef UIT_INTERNAL_HAS_emp_assert
    #pragma push_macro("emp_assert")
    #undef emp_assert
  #endif

  #if defined(\
    UIT_INTERNAL_HAS_emp_assert) && defined(UIT_EXTERNAL_1ST_emp_assert\
  )
    #ifndef UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
    #ifndef UIT_emp_assert_INSEEP_WARNING
      #define UIT_emp_assert_INSEEP_WARNING
      #pragma message(\
        "emp_assert defined first outside vendorized header,"\
        " so outside definition will be used inside vendorized header"\
        " on subsequent includes of the vendorized header"\
        " --- if causes problems, try changing header include order"\
      )
    #endif
    #endif
  #endif

#else // #ifdef emp_assert

  #ifdef UIT_INTERNAL_HAS_emp_assert
    #pragma pop_macro("emp_assert")
  #endif // #ifdef UIT_INTERNAL_HAS_emp_assert

#endif // #ifdef emp_assert

#ifdef emp_always_assert_impl

  #ifndef UIT_EXTERNAL_HAS_emp_always_assert_impl
    #define UIT_EXTERNAL_HAS_emp_always_assert_impl
    #ifndef UIT_INTERNAL_HAS_emp_always_assert_impl
      #define UIT_EXTERNAL_1ST_emp_always_assert_impl
    #endif
  #endif

  #ifndef UIT_INTERNAL_HAS_emp_always_assert_impl
    #pragma push_macro("emp_always_assert_impl")
    #undef emp_always_assert_impl
  #endif

  #if defined(\
    UIT_INTERNAL_HAS_emp_always_assert_impl) && defined(UIT_EXTERNAL_1ST_emp_always_assert_impl\
  )
    #ifndef UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
    #ifndef UIT_emp_always_assert_impl_INSEEP_WARNING
      #define UIT_emp_always_assert_impl_INSEEP_WARNING
      #pragma message(\
        "emp_always_assert_impl defined first outside vendorized header,"\
        " so outside definition will be used inside vendorized header"\
        " on subsequent includes of the vendorized header"\
        " --- if causes problems, try changing header include order"\
      )
    #endif
    #endif
  #endif

#else // #ifdef emp_always_assert_impl

  #ifdef UIT_INTERNAL_HAS_emp_always_assert_impl
    #pragma pop_macro("emp_always_assert_impl")
  #endif // #ifdef UIT_INTERNAL_HAS_emp_always_assert_impl

#endif // #ifdef emp_always_assert_impl

#ifdef emp_always_assert

  #ifndef UIT_EXTERNAL_HAS_emp_always_assert
    #define UIT_EXTERNAL_HAS_emp_always_assert
    #ifndef UIT_INTERNAL_HAS_emp_always_assert
      #define UIT_EXTERNAL_1ST_emp_always_assert
    #endif
  #endif

  #ifndef UIT_INTERNAL_HAS_emp_always_assert
    #pragma push_macro("emp_always_assert")
    #undef emp_always_assert
  #endif

  #if defined(\
    UIT_INTERNAL_HAS_emp_always_assert) && defined(UIT_EXTERNAL_1ST_emp_always_assert\
  )
    #ifndef UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
    #ifndef UIT_emp_always_assert_INSEEP_WARNING
      #define UIT_emp_always_assert_INSEEP_WARNING
      #pragma message(\
        "emp_always_assert defined first outside vendorized header,"\
        " so outside definition will be used inside vendorized header"\
        " on subsequent includes of the vendorized header"\
        " --- if causes problems, try changing header include order"\
      )
    #endif
    #endif
  #endif

#else // #ifdef emp_always_assert

  #ifdef UIT_INTERNAL_HAS_emp_always_assert
    #pragma pop_macro("emp_always_assert")
  #endif // #ifdef UIT_INTERNAL_HAS_emp_always_assert

#endif // #ifdef emp_always_assert

#ifdef emp_assert_TO_PAIR

  #ifndef UIT_EXTERNAL_HAS_emp_assert_TO_PAIR
    #define UIT_EXTERNAL_HAS_emp_assert_TO_PAIR
    #ifndef UIT_INTERNAL_HAS_emp_assert_TO_PAIR
      #define UIT_EXTERNAL_1ST_emp_assert_TO_PAIR
    #endif
  #endif

  #ifndef UIT_INTERNAL_HAS_emp_assert_TO_PAIR
    #pragma push_macro("emp_assert_TO_PAIR")
    #undef emp_assert_TO_PAIR
  #endif

  #if defined(\
    UIT_INTERNAL_HAS_emp_assert_TO_PAIR) && defined(UIT_EXTERNAL_1ST_emp_assert_TO_PAIR\
  )
    #ifndef UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
    #ifndef UIT_emp_assert_TO_PAIR_INSEEP_WARNING
      #define UIT_emp_assert_TO_PAIR_INSEEP_WARNING
      #pragma message(\
        "emp_assert_TO_PAIR defined first outside vendorized header,"\
        " so outside definition will be used inside vendorized header"\
        " on subsequent includes of the vendorized header"\
        " --- if causes problems, try changing header include order"\
      )
    #endif
    #endif
  #endif

#else // #ifdef emp_assert_TO_PAIR

  #ifdef UIT_INTERNAL_HAS_emp_assert_TO_PAIR
    #pragma pop_macro("emp_assert_TO_PAIR")
  #endif // #ifdef UIT_INTERNAL_HAS_emp_assert_TO_PAIR

#endif // #ifdef emp_assert_TO_PAIR

#ifdef emp_always_assert_msvc_impl

  #ifndef UIT_EXTERNAL_HAS_emp_always_assert_msvc_impl
    #define UIT_EXTERNAL_HAS_emp_always_assert_msvc_impl
    #ifndef UIT_INTERNAL_HAS_emp_always_assert_msvc_impl
      #define UIT_EXTERNAL_1ST_emp_always_assert_msvc_impl
    #endif
  #endif

  #ifndef UIT_INTERNAL_HAS_emp_always_assert_msvc_impl
    #pragma push_macro("emp_always_assert_msvc_impl")
    #undef emp_always_assert_msvc_impl
  #endif

  #if defined(\
    UIT_INTERNAL_HAS_emp_always_assert_msvc_impl) && defined(UIT_EXTERNAL_1ST_emp_always_assert_msvc_impl\
  )
    #ifndef UIT_SUPPRESS_MACRO_INSEEP_WARNINGS
    #ifndef UIT_emp_always_assert_msvc_impl_INSEEP_WARNING
      #define UIT_emp_always_assert_msvc_impl_INSEEP_WARNING
      #pragma message(\
        "emp_always_assert_msvc_impl defined first outside vendorized header,"\
        " so outside definition will be used inside vendorized header"\
        " on subsequent includes of the vendorized header"\
        " --- if causes problems, try changing header include order"\
      )
    #endif
    #endif
  #endif

#else // #ifdef emp_always_assert_msvc_impl

  #ifdef UIT_INTERNAL_HAS_emp_always_assert_msvc_impl
    #pragma pop_macro("emp_always_assert_msvc_impl")
  #endif // #ifdef UIT_INTERNAL_HAS_emp_always_assert_msvc_impl

#endif // #ifdef emp_always_assert_msvc_impl

#endif // #ifdef UIT_VENDORIZE_EMP
