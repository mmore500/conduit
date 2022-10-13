#ifndef UIT_EMP_POLYFILL_SPAN_HPP
#define UIT_EMP_POLYFILL_SPAN_HPP

#ifdef UIT_VENDORIZE_EMP
#include "../vendorization/push_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#include "../../../third-party/Empirical/include/emp/polyfill/span.hpp"

#ifndef UIT_VENDORIZE_EMP
namespace emp {}
namespace uit_emp = emp;
#endif // #ifndef UIT_VENDORIZE_EMP

#ifdef UIT_VENDORIZE_EMP
#include "../vendorization/pop_uit_emp.hh"
#endif // #ifdef UIT_VENDORIZE_EMP

#endif // #ifndef UIT_EMP_POLYFILL_SPAN_HPP
