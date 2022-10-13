#ifdef UIT_VENDORIZE_EMP

#ifdef emp_always_assert_impl

  #ifndef UIT_INTERNAL_HAS_emp_always_assert_impl
    #define UIT_INTERNAL_HAS_emp_always_assert_impl

    #ifdef UIT_EXTERNAL_1ST_emp_always_assert_impl
      #pragma pop_macro("emp_always_assert_impl")
    #endif

    #ifndef UIT_EXTERNAL_HAS_emp_always_assert_impl
      #ifndef UIT_INTERNAL_1ST_emp_always_assert_impl
        #define UIT_INTERNAL_1ST_emp_always_assert_impl
      #endif
    #endif

  #endif // #ifndef UIT_INTERNAL_HAS_emp_always_assert_impl

  #ifndef UIT_EXTERNAL_HAS_emp_always_assert_impl
    #pragma push_macro("emp_always_assert_impl")
    #undef emp_always_assert_impl
  #endif

#endif // #ifdef emp_always_assert_impl

#ifdef emp_assert

  #ifndef UIT_INTERNAL_HAS_emp_assert
    #define UIT_INTERNAL_HAS_emp_assert

    #ifdef UIT_EXTERNAL_1ST_emp_assert
      #pragma pop_macro("emp_assert")
    #endif

    #ifndef UIT_EXTERNAL_HAS_emp_assert
      #ifndef UIT_INTERNAL_1ST_emp_assert
        #define UIT_INTERNAL_1ST_emp_assert
      #endif
    #endif

  #endif // #ifndef UIT_INTERNAL_HAS_emp_assert

  #ifndef UIT_EXTERNAL_HAS_emp_assert
    #pragma push_macro("emp_assert")
    #undef emp_assert
  #endif

#endif // #ifdef emp_assert

#ifdef emp_always_assert_msvc_impl

  #ifndef UIT_INTERNAL_HAS_emp_always_assert_msvc_impl
    #define UIT_INTERNAL_HAS_emp_always_assert_msvc_impl

    #ifdef UIT_EXTERNAL_1ST_emp_always_assert_msvc_impl
      #pragma pop_macro("emp_always_assert_msvc_impl")
    #endif

    #ifndef UIT_EXTERNAL_HAS_emp_always_assert_msvc_impl
      #ifndef UIT_INTERNAL_1ST_emp_always_assert_msvc_impl
        #define UIT_INTERNAL_1ST_emp_always_assert_msvc_impl
      #endif
    #endif

  #endif // #ifndef UIT_INTERNAL_HAS_emp_always_assert_msvc_impl

  #ifndef UIT_EXTERNAL_HAS_emp_always_assert_msvc_impl
    #pragma push_macro("emp_always_assert_msvc_impl")
    #undef emp_always_assert_msvc_impl
  #endif

#endif // #ifdef emp_always_assert_msvc_impl

#ifdef emp_assert_TO_PAIR

  #ifndef UIT_INTERNAL_HAS_emp_assert_TO_PAIR
    #define UIT_INTERNAL_HAS_emp_assert_TO_PAIR

    #ifdef UIT_EXTERNAL_1ST_emp_assert_TO_PAIR
      #pragma pop_macro("emp_assert_TO_PAIR")
    #endif

    #ifndef UIT_EXTERNAL_HAS_emp_assert_TO_PAIR
      #ifndef UIT_INTERNAL_1ST_emp_assert_TO_PAIR
        #define UIT_INTERNAL_1ST_emp_assert_TO_PAIR
      #endif
    #endif

  #endif // #ifndef UIT_INTERNAL_HAS_emp_assert_TO_PAIR

  #ifndef UIT_EXTERNAL_HAS_emp_assert_TO_PAIR
    #pragma push_macro("emp_assert_TO_PAIR")
    #undef emp_assert_TO_PAIR
  #endif

#endif // #ifdef emp_assert_TO_PAIR

#ifdef emp_always_assert

  #ifndef UIT_INTERNAL_HAS_emp_always_assert
    #define UIT_INTERNAL_HAS_emp_always_assert

    #ifdef UIT_EXTERNAL_1ST_emp_always_assert
      #pragma pop_macro("emp_always_assert")
    #endif

    #ifndef UIT_EXTERNAL_HAS_emp_always_assert
      #ifndef UIT_INTERNAL_1ST_emp_always_assert
        #define UIT_INTERNAL_1ST_emp_always_assert
      #endif
    #endif

  #endif // #ifndef UIT_INTERNAL_HAS_emp_always_assert

  #ifndef UIT_EXTERNAL_HAS_emp_always_assert
    #pragma push_macro("emp_always_assert")
    #undef emp_always_assert
  #endif

#endif // #ifdef emp_always_assert

#endif // #ifdef UIT_VENDORIZE_EMP
