#ifndef _APPLY_TUPLE_TO_FUNCTION_HPP_
#define _APPLY_TUPLE_TO_FUNCTION_HPP_

#include "int_list.hpp"
#include "placeholder.hpp"
#include <tuple>
#include <type_traits>

template<typename A>
constexpr decltype(auto) get_value(A&& a) {
   return std::forward<A>(a);
}

template<int ...I, typename F, typename A>
constexpr decltype(auto) get_value_impl(int_list<I...>, F&& f, A&& a) {
   return f(std::get<I + 1>(std::forward<A>(a))...);
}

template<typename F, typename ...A>
constexpr decltype(auto) get_value(std::tuple<F, A...>&& a) {
   return get_value_impl(typename make_int_list<sizeof...(A)>::type(), std::get<0>(std::forward<std::tuple<F, A...>>(a)), std::forward<std::tuple<F, A...>>(a));
}

template<int I, int index, typename T>
constexpr decltype(auto) selective_get(std::integral_constant<int, I>, std::integral_constant<int, index>, T&& t) {
   return std::get<I>(std::forward<T>(t));
}

template<int I, typename T>
constexpr decltype(auto) selective_get(std::integral_constant<int, I>, std::integral_constant<int, I>, T&& t) {
   return ph<1>();
}

template<int index, typename F, typename T, int ...I>
constexpr decltype(auto) apply_tuple_to_function(std::integral_constant<int, index> selected_index, int_list<I...>, F&& f, T&& t) {
   using boxtype = std::decay_t<decltype(std::get<index>(std::forward<T>(t)))>;
   return isabox<boxtype>::box_map(std::get<index>(std::forward<T>(t)), fn(f)(get_value(selective_get(std::integral_constant<int, I>(), selected_index, std::forward<T>(t)))...));
}

template<int index, typename F, typename T1, typename T2, int ...I>
constexpr decltype(auto) apply_tuple_to_function(std::integral_constant<int, index> selected_index, int_list<I...>, fn_t<F, T1>&& f, T2&& t) {
   using boxtype = std::decay_t<decltype(std::get<index>(std::forward<T2>(t)))>;
   return isabox<boxtype>::box_map(std::get<index>(std::forward<T2>(t)), f(get_value(selective_get(std::integral_constant<int, I>(), selected_index, std::forward<T2>(t)))...));
}

template<typename F, typename T, int ...I>
constexpr decltype(auto) apply_tuple_to_function(std::integral_constant<int, -1>, int_list<I...>, F&& f, T&& t) {
   return f(get_value(std::get<I>(std::forward<T>(t)))...);
}

template<typename F, typename T1, typename T2, int ...I>
constexpr decltype(auto) apply_tuple_to_function(std::integral_constant<int, -1>, int_list<I...>, fn_t<F, T1>&& f, T2&& t) {
   return f(get_value(std::get<I>(std::forward<T2>(t)))...);
}

#endif//_APPLY_TUPLE_TO_FUNCTION_HPP_
