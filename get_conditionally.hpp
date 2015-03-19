#ifndef _GET_CONDITIONALLY_HPP_
#define _GET_CONDITIONALLY_HPP_

#include "arg_category.hpp"
#include "int_list.hpp"
#include "placeholder.hpp"
#include <tuple>
#include <type_traits>

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<int ArgsCount, typename ArgPosition, typename Item, typename A>
constexpr decltype(auto) get_conditionally(std::integral_constant<ArgCategory, ArgCategory::Regular>, Item&& item, A&&) {
   return std::forward<Item>(item);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<int I, ArgCategory SomethingElse, typename A>
constexpr decltype(auto) decorate_arg(std::integral_constant<ArgCategory, SomethingElse>, A&& a) {
   return std::forward<A>(a);
}

template<int I, typename A>
constexpr decltype(auto) decorate_arg(std::integral_constant<ArgCategory, ArgCategory::Function>, A&& a) {
   return std::make_tuple(int_list<I>(), a);
}

template<int ArgsCount, int I, int ArgPosition, typename A>
constexpr decltype(auto) get_conditionally_impl1(std::true_type, int_list<ArgPosition>, A&& a) {
   return decorate_arg<I>(arg_category<decltype(std::get<ArgPosition + I - 1>(a))>(), std::get<ArgPosition + I - 1>(std::forward<A>(a)));
}

template<int ArgsCount, int I, int ArgPosition, typename A>
constexpr decltype(auto) get_conditionally_impl1(std::false_type, int_list<ArgPosition>, A&&) {
   return ph<I - ArgsCount>();
}

template<int ArgsCount, typename ArgPosition, int I, typename A>
constexpr decltype(auto) get_conditionally(std::integral_constant<ArgCategory, ArgCategory::Placeholder>, ph<I> item, A&& a) {
   return get_conditionally_impl1<ArgsCount, I>(std::integral_constant<bool, (ArgsCount >= I)>(), ArgPosition(), std::forward<A>(a));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<int I, int ArgPosition, int ...J, typename F, typename A>
constexpr decltype(auto) add_arguments_to_function(int_list<ArgPosition>, int_list<J...>, F&& f, A&& a) {
   return std::make_tuple(f, std::get<ArgPosition + J>(std::forward<A>(a))...);
}

template<int ArgsCount, typename ArgPosition, int I, typename F, typename A>
constexpr decltype(auto) get_conditionally_impl2(std::true_type, std::tuple<int_list<I>, F>&& item, A&& a) {
   return add_arguments_to_function<I>(ArgPosition(), typename make_int_list<function_traits<typename std::remove_reference<F>::type>::arity>::type(), std::get<1>(std::forward<std::tuple<int_list<I>, F>>(item)), std::forward<A>(a));
}

template<int ArgsCount, typename ArgPosition, int I, typename F, typename A>
constexpr decltype(auto) get_conditionally_impl2(std::false_type, std::tuple<int_list<I>, F>&& item, A&&) {
   return std::make_tuple(int_list<I - ArgsCount>(), std::get<1>(std::forward<std::tuple<int_list<I>, F>>(item)));
}

template<int ArgsCount, typename ArgPosition, int I, typename F, typename A>
constexpr decltype(auto) get_conditionally(std::integral_constant<ArgCategory, ArgCategory::AppliedFunction>, std::tuple<int_list<I>, F> item, A&& a) {
   return get_conditionally_impl2<ArgsCount, ArgPosition, I>(std::integral_constant<bool, ArgsCount >= I >(), std::forward<std::tuple<int_list<I>, F>>(item), std::forward<A>(a));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif//_GET_CONDITIONALLY_HPP_