#ifdef UIT_VENDORIZE_EMP

#define emp uit_emp
#pragma push_macro("EMP_NAMESPACE")
#undef EMP_NAMESPACE
#define EMP_NAMESPACE() emp

#include "push_headerguards.hh"
#include "push_macros.hh"

#endif // #ifdef UIT_VENDORIZE_EMP
