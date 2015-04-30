#ifndef _INT_LIST_HPP_
#define _INT_LIST_HPP_

#include <tuple>

template<int ...I>
struct int_list {
};

template<typename I, int Count>
struct make_int_list_impl;

template<int ...I, int Count>
struct make_int_list_impl<int_list<I...>, Count> {
   using type = typename make_int_list_impl<int_list<Count - 1, I...>, Count - 1>::type;
};

template<int ...I>
struct make_int_list_impl<int_list<I...>, 0> {
   using type = int_list<I...>;
};

template<int Count>
struct make_int_list {
   using type = typename make_int_list_impl<int_list<>, Count>::type;
};

template<typename T>
using indices_for = typename make_int_list<std::tuple_size<T>::value>::type;

#endif//_INT_LIST_HPP_