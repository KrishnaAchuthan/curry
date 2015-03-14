#ifndef _APPLY_TUPLE_TO_FUNCTION_HPP_
#define _APPLY_TUPLE_TO_FUNCTION_HPP_

#include "int_list.hpp"
#include <tuple>
#include <type_traits>

template<typename A>
auto get_value(A&& a) {
	return std::forward<A>(a);
}

template<int ...I, typename F, typename A>
auto get_value_impl(int_list<I...>, F&& f, A&& a) {
	return f(std::get<I + 1>(std::forward<A>(a))...);
}

template<typename F, typename ...A>
auto get_value(std::tuple<F, A...>&& a) {
	return get_value_impl(typename make_int_list<sizeof...(A)>::type(), std::get<0>(std::forward<std::tuple<F, A...>>(a)), std::forward<std::tuple<F, A...>>(a));
}

template<typename F, typename T, int ...I>
decltype(auto) apply_tuple_to_function(int_list<I...>, F&& f, T&& t) {
	return f(get_value(std::get<I>(std::forward<T>(t)))...);
}

#endif//_APPLY_TUPLE_TO_FUNCTION_HPP_
