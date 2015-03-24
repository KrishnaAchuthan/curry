#ifndef _WRAP_FN_HPP_
#define _WRAP_FN_HPP_

#include "fn_fwd.hpp"
#include "function_operator_exists.hpp"
#include <type_traits>

template<typename F, typename Enabler = typename std::enable_if<function_traits<std::decay_t<F>>::is_function>::type>
cfn_t<F> wrap_fn(F&& f) {
   return cfn_t<F>(std::forward<F>(f));
}

//template<typename T, typename Enabler = typename std::enable_if<!function_traits<std::decay_t<F>>::is_function>::type>
//T&& wrap_fn(T&& t) {
//   return std::forward<T>(t);
//}

#endif//_WRAP_FN_HPP_