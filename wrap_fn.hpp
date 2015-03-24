#ifndef _WRAP_FN_HPP_
#define _WRAP_FN_HPP_

#include "fn_fwd.hpp"
#include "function_operator_exists.hpp"
#include <type_traits>

////template<typename F, typename Enabler = typename std::enable_if<function_traits<std::decay_t<F>>::is_function>::type>
//template<typename F, typename Enabler = typename std::enable_if<function_traits<F>::is_function>::type>
//auto wrap_fn(F&& f) {
//   return cfn_t<F>(std::forward<F>(f));
//}
//
////template<typename T, typename Enabler = typename std::enable_if<!function_traits<std::decay_t<F>>::is_function>::type>
//template<typename T, typename Enabler = typename std::enable_if<!function_traits<F>::is_function>::type>
//T&& wrap_fn(T&& t) {
//   return std::forward<T>(t);
//}

template<typename F>
typename std::enable_if<function_traits<F>::is_function, cfn_t<F>>::type wrap_fn(F&& f) {
   return cfn_t<F>(std::forward<F>(f));
}

template<typename T>
typename std::enable_if<!function_traits<T>::is_function, T&&>::type wrap_fn(T&& t) {
   return std::forward<T>(t);
}

#endif//_WRAP_FN_HPP_