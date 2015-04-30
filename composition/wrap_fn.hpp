#ifndef _WRAP_FN_HPP_
#define _WRAP_FN_HPP_

#include "..\util\fn_fwd.hpp"
#include "..\arity\function_operator_exists.hpp"
#include <type_traits>

namespace curry {

template<typename F>
typename std::enable_if<function_traits<F>::is_function, cfn_t<F>>::type wrap_fn(F&& f) {
   return cfn_t<F>(std::forward<F>(f));
}

template<typename T>
typename std::enable_if<!function_traits<T>::is_function, T&&>::type wrap_fn(T&& t) {
   return std::forward<T>(t);
}

}

#endif//_WRAP_FN_HPP_