#ifndef _INVOKE_HPP_
#define _INVOKE_HPP_

#include "fn_fwd.hpp"
#include "apply_tuple_to_function.hpp"
#include "int_list.hpp"
#include <type_traits>

template<typename F, typename T>
auto invoke(F&& f, std::false_type, T&& t) {
   return fn(fn_t<F, T>(std::forward<F>(f), std::forward<T>(t)));
}

template<typename F, typename T>
auto invoke(F&& f, std::true_type, T&& t) {
   return apply_tuple_to_function(indices_for<T>(), std::forward<F>(f), std::forward<T>(t));
}

#endif//_INVOKE_HPP_
