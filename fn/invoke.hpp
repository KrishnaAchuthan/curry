#ifndef _INVOKE_HPP_
#define _INVOKE_HPP_

#include "..\fn\apply_tuple_to_function.hpp"
#include "..\monads\get_first_box_argument_index.hpp"
#include "..\util\fn_fwd.hpp"
#include "..\util\int_list.hpp"
#include <type_traits>

namespace curry {

template<typename F, typename T>
CONSTEXPR decltype(auto) invoke(F&& f, std::false_type, T&& t) {
   return fn(fn_t<F, T>(std::forward<F>(f), std::forward<T>(t)));
}

template<typename F, typename T>
CONSTEXPR decltype(auto) invoke(F&& f, std::true_type, T&& t) {
   return apply_tuple_to_function(std::integral_constant<int, get_first_box_argument_index<T>::value>(), indices_for<T>(), std::forward<F>(f), std::forward<T>(t));
}

}

#endif//_INVOKE_HPP_
