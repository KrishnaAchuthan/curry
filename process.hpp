#ifndef _PROCESS_HPP_
#define _PROCESS_HPP_

#include "invoke.hpp"
#include "arity_matches.hpp"
#include "apply_arguments.hpp"
#include "add_anonymous_placeholder_support.hpp"
#include <tuple>
#include <type_traits>

template<typename F, typename T, typename ...A>
auto process(F&& f, T&& t, A&& ...a) {
   return invoke(std::forward<F>(f), std::integral_constant<bool, arity_matches<T, A...>::value>(), apply_arguments<sizeof...(A)>(indices_for<T>(), std::forward<T>(t), add_anonymous_placeholder_support(std::forward_as_tuple(std::forward<A>(a)...))));
}

#endif//_PROCESS_HPP_
