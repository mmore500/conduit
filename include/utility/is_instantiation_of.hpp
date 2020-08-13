#pragma once

#include <type_traits>

namespace uit {

// adapted from https://stackoverflow.com/a/11251408
template < template <typename...> class Template, typename T >
struct is_instantiation_of : std::false_type {};

template < template <typename...> class Template, typename... Args >
struct is_instantiation_of< Template, Template<Args...> > : std::true_type {};

}
