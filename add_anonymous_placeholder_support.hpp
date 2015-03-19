#ifndef _ADD_ANONYMOUS_PLACEHOLDER_SUPPORT_HPP_
#define _ADD_ANONYMOUS_PLACEHOLDER_SUPPORT_HPP_

#include "real_arguments_before_i.hpp"
#include "placeholder.hpp"
#include "int_list.hpp"
#include <tuple>
#include <type_traits>

template<int I, typename T, typename Item>
constexpr decltype(auto) correct_anonymous_placeholder_number(Item&& item) {
   return std::forward<Item>(item);
}

template<int I, typename T>
constexpr decltype(auto) correct_anonymous_placeholder_number(ph<0>) {
   return ph<I + 1 - real_arguments_before_I<T, I>::value>();
}

template<int ...I, typename T>
constexpr decltype(auto) add_anonymous_placeholder_support_impl(int_list<I...>, T&& t) {
   return std::forward_as_tuple(correct_anonymous_placeholder_number<I, T>(std::get<I>(std::forward<T>(t)))...);
}

template<typename T>
constexpr decltype(auto) add_anonymous_placeholder_support(T&& t) {
   return add_anonymous_placeholder_support_impl(indices_for<T>(), std::forward<T>(t));
}

#endif//_ADD_ANONYMOUS_PLACEHOLDER_SUPPORT_HPP_
