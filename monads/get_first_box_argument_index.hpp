#ifndef _GET_FIRST_BOX_ARGUMENT_INDEX_HPP_
#define _GET_FIRST_BOX_ARGUMENT_INDEX_HPP_

#include "..\monads\isabox.hpp"
#include <tuple>
#include <type_traits>

namespace curry {

template<int current_index, typename T, typename Enabler=void>
struct get_first_box_argument_index_impl;

template<int current_index, typename A, typename ...Rest>
struct get_first_box_argument_index_impl<current_index, std::tuple<A, Rest...>, typename std::enable_if<isabox<A>::value>::type> {
   static const int value = current_index;
};

template<int current_index, typename A, typename ...Rest>
struct get_first_box_argument_index_impl<current_index, std::tuple<A, Rest...>, typename std::enable_if<!isabox<A>::value>::type> {
   static const int value = get_first_box_argument_index_impl<current_index + 1, std::tuple<Rest...>>::value;
};

template<int current_index>
struct get_first_box_argument_index_impl<current_index, std::tuple<>, void> {
   static const int value = -1;
};

template<typename T>
struct get_first_box_argument_index : get_first_box_argument_index_impl<0, T> {
};

}

#endif//_GET_FIRST_BOX_ARGUMENT_INDEX_HPP_